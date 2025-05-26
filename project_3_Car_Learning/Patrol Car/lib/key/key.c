#include "key.h"
#include "delay.h"

// 按键相关的时间常量定义
#define KEY_SETTLING_TIME             10   // 按钮消抖延迟（单位：毫秒）
#define KEY_CLICK_INTERVAL            200  // 按钮多击时每次点击的最大时间间隔（单位：毫秒）
#define KEY_LONG_PRESS_THRESHOLD      1000 // 按钮长按最小时间（单位：毫秒）
#define KEY_LONG_PRESS_TICK_INTERNVAL 100  // 长按后持续触发的时间间隔（单位：毫秒）

// 按钮事件回调函数原型
static void OnKeyPressed(Key_TypeDef *Key);
static void OnKeyReleased(Key_TypeDef *Key);
static void OnKeyEveryPolled(Key_TypeDef *Key, uint8_t State, uint32_t currentTime);

/**
 * @brief 初始化按钮结构体
 * @param Key 按钮结构体指针
 * @param Key_Initstruct 按钮初始化结构体指针
 */
void Key_Init(Key_TypeDef *Key, Key_InitTypedef *Key_Initstruct)
{
    // 设置按钮硬件参数
    Key->key_port = Key_Initstruct->key_port;
    Key->key_pin = Key_Initstruct->key_pin;
    
    // 设置回调函数
    Key->key_pressed_cb = Key_Initstruct->key_pressed_cb;
    Key->key_released_cb = Key_Initstruct->key_released_cb;
    Key->key_long_pressed_cb = Key_Initstruct->key_long_pressed_cb;
    
    // 设置默认时间参数
    Key->LongPressThreshold = Key_Initstruct->LongPressThreshold ?: KEY_LONG_PRESS_THRESHOLD;
    Key->LongPressTickInterval = Key_Initstruct->LongPressTickInterval ?: KEY_LONG_PRESS_TICK_INTERNVAL;
    Key->ClickInterval = Key_Initstruct->ClickInterval ?: KEY_CLICK_INTERVAL;

    // 初始化按钮状态
    Key->LastState = 0;
    Key->ChangePending = 0;
    Key->PendingTime = 0;
    Key->LastPressedTime = 0;
    Key->LastReleasedTime = 0;
    Key->LongPressTicks = 0;
    Key->ClickCnt = 0;
}

/**
 * @brief 处理按钮状态
 * @param Key 按钮结构体指针
 */
void Key_Proc(Key_TypeDef *Key)
{
    uint8_t currentState;
    uint32_t currentTime = get_ticks();

    if (Key->ChangePending)
    {
        // 消抖处理
        if (currentTime - Key->PendingTime >= KEY_SETTLING_TIME)
        {
            currentState = DL_GPIO_readPins(Key->key_port, Key->key_pin) > 0 ? 1 : 0;
            if (currentState != Key->LastState)
            {
                if (currentState == 1) 
                    OnKeyPressed(Key);
                else 
                    OnKeyReleased(Key);
            }
            Key->LastState = currentState;
            Key->ChangePending = 0;
        }
    }
    else 
    {
        currentState = DL_GPIO_readPins(Key->key_port, Key->key_pin) > 0 ? 1 : 0;
        if (currentState != Key->LastState)
        {
            Key->ChangePending = 1;
            Key->PendingTime = currentTime;
        }
    }
    OnKeyEveryPolled(Key, Key->LastState, currentTime);
}

/**
 * @brief 获取按钮当前状态
 * @param Key 按钮结构体指针
 * @return 按钮状态（0：松开，1：按下）
 */
uint8_t Key_GetState(Key_TypeDef *Key)
{
    return Key->LastState;
}

/**
 * @brief 按钮按下事件处理
 * @param Key 按钮结构体指针
 */
static void OnKeyPressed(Key_TypeDef *Key)
{
    Key->LastPressedTime = get_ticks();
    if (Key->key_pressed_cb != 0)
    {
        Key->key_pressed_cb();
    }
}

/**
 * @brief 按钮松开事件处理
 * @param Key 按钮结构体指针
 */
static void OnKeyReleased(Key_TypeDef *Key)
{
    Key->LastReleasedTime = get_ticks();
    if (Key->key_released_cb != 0)
    {
        Key->key_released_cb();
    }
    
    Key->LongPressTicks = 0;
    
    if (Key->LastReleasedTime - Key->LastPressedTime < Key->LongPressThreshold)
    {
        Key->ClickCnt++;
    }
    else
    {
        Key->ClickCnt = 0;
    }
}

/**
 * @brief 按钮轮询处理
 * @param Key 按钮结构体指针
 * @param State 当前按钮状态
 * @param CurrentTime 当前时间
 */
static void OnKeyEveryPolled(Key_TypeDef *Key, uint8_t State, uint32_t CurrentTime)
{
    // 处理长按
    if (Key->LastState == 1)
    {
        if (Key->LongPressTicks == 0)
        {
            if (Key->LastPressedTime != 0 && 
                CurrentTime - Key->LastPressedTime > Key->LongPressThreshold)
            {
                Key->LongPressTicks = 1;
                if (Key->key_long_pressed_cb)
                {
                    Key->key_long_pressed_cb(Key->LongPressTicks);
                }
                Key->LastLongPressTickTime = get_ticks();
            }
        }
        else
        {
            if (CurrentTime - Key->LastLongPressTickTime > Key->LongPressTickInterval)
            {
                Key->LastLongPressTickTime = get_ticks();
                Key->LongPressTicks++;
                if (Key->key_long_pressed_cb)
                {
                    Key->key_long_pressed_cb(Key->LongPressTicks);
                }
            }
        }
    }
    
    // 处理多击
    if (Key->ClickCnt > 0 && Key->LastState == 0 && 
        (get_ticks() - Key->LastReleasedTime) > Key->ClickInterval)
    {
        if (Key->key_clicked_cb)
        {
            Key->key_clicked_cb(Key->ClickCnt);
        }
        Key->ClickCnt = 0;
    }
}