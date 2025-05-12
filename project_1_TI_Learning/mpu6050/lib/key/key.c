#include "key.h"
#include"mspm0_clock.h"

#define KEY_SETTLING_TIME             10   // 按钮消抖延迟
#define KEY_CLICK_INTERVAL            200  // 按钮多击时每次点击的时间最大时间间隔
#define KEY_LONG_PRESS_THRESHOLD      1000 // 按钮长按最小时间
#define KEY_LONG_PRESS_TICK_INTERNVAL 100  // 长按后持续触发的时间间隔

// 按钮按下时的回调函数
static void OnKeyPressed(Key_TypeDef *Key);
// 按钮松开时的回调函数
static void OnKeyReleased(Key_TypeDef *Key);
// 每次轮询按钮状态时的回调函数
static void OnKeyEveryPolled(Key_TypeDef *Key, uint8_t State, uint32_t currentTime);

// 初始化按钮结构体
void Key_Init(Key_TypeDef *Key,Key_InitTypedef *Key_Initstruct)
{
    Key->key_port = Key_Initstruct->key_port; // 设置按钮端口
    Key->key_pin = Key_Initstruct->key_pin;   // 设置按钮引脚
    Key->key_pressed_cb = Key_Initstruct->key_pressed_cb; // 设置按钮按下回调函数
    Key->key_released_cb = Key_Initstruct->key_released_cb; // 设置按钮松开回调函数
    Key->key_long_pressed_cb = Key_Initstruct->key_long_pressed_cb; // 设置按钮长按回调函数
    // 如果未设置长按阈值，则使用默认值
    if(Key->LongPressThreshold == 0)
	{
		Key->LongPressThreshold = KEY_LONG_PRESS_THRESHOLD;
	}
	
    // 如果未设置长按触发间隔，则使用默认值
	if(Key->LongPressTickInterval == 0)
	{
		Key->LongPressTickInterval = KEY_LONG_PRESS_TICK_INTERNVAL;
	}
	
    // 如果未设置点击间隔，则使用默认值
	if(Key->ClickInterval == 0)
	{
		Key->ClickInterval = KEY_CLICK_INTERVAL;
	}

    // 初始化按钮状态
    Key->LastState = 0; // 初始状态下假设按钮是松开的
	Key->ChangePending = 0; 
	Key->PendingTime = 0;
	Key->LastPressedTime = 0;
	Key->LastReleasedTime = 0;
	Key->LongPressTicks = 0;
	Key->ClickCnt = 0;
}

// 处理按钮状态
void Key_Proc(Key_TypeDef *Key)
{
    uint8_t currentState;
    uint32_t currentTime = get_ticks();

    // 如果有状态变化待处理
    if(Key->ChangePending)
    {
        // 如果已经过了消抖时间
        if(currentTime - Key->PendingTime + KEY_SETTLING_TIME)
        {
            // 读取当前按钮状态
            currentState = DL_GPIO_readPins(Key->key_port, Key->key_pin) > 0 ? 1 : 0;
            // 如果状态与上一次状态不同
            if(currentState != Key->LastState)
            {
                if(currentState == 1) 
					OnKeyPressed(Key); // #1. 按钮按下
				else 
					OnKeyReleased(Key); // #2. 按钮松开
            }
        }
        Key->LastState = currentState;
        Key->ChangePending = 0;
    }
    
    else 
    {
        currentState = DL_GPIO_readPins(Key->key_port, Key->key_pin) > 0 ? 1 : 0;
        if(currentState != Key->LastState)
        {
            Key->ChangePending = 1;
            Key->PendingTime = currentTime;
        }
    }
    OnKeyEveryPolled(Key, Key->LastState, currentTime);
}
uint8_t Key_GetState(Key_TypeDef *Key)
{
	return Key->LastState;
}
static void OnKeyPressed(Key_TypeDef *Key)
{
	Key->LastPressedTime = get_ticks();
	
	// 调用按钮按下的回调函数
	if(Key->key_pressed_cb != 0)
	{
		Key->key_pressed_cb();
	}
}
static void OnKeyReleased(Key_TypeDef *Key)
{
	Key->LastReleasedTime = get_ticks();
	
	// 调用按钮松开的回调函数
	if(Key->key_released_cb != 0)
	{
		Key->key_released_cb();
	}
	
	// 松开后长按计数清零
	Key->LongPressTicks = 0;
	
	if(Key->LastReleasedTime - Key->LastPressedTime < Key->LongPressThreshold)
	{
		Key->ClickCnt++;
	}
	else
	{
		Key->ClickCnt = 0;
	}
}

//
// @简介：处理每一次按钮轮询的动作
//
static void OnKeyEveryPolled(Key_TypeDef *Key, uint8_t State, uint32_t CurrentTime)
{
	/* 处理按钮长按的动作 */
	
	if(Key->LastState == 1)
	{
		if(Key->LongPressTicks == 0) // 如果长按未被触发
		{
			if(Key->LastPressedTime!= 0 
				&& CurrentTime - Key->LastPressedTime > Key->LongPressThreshold) // 且已超过触发时间
			{
				Key->LongPressTicks = 1;
			
				if(Key->key_long_pressed_cb)
				{
					Key->key_long_pressed_cb(Key->LongPressTicks); // 触发长按回调函数
				}
				
				Key->LastLongPressTickTime =  get_ticks();
			}
		}
		else
		{
			if(CurrentTime - Key->LastLongPressTickTime > Key->LongPressTickInterval) // 超过Tick间隔
			{
				Key->LastLongPressTickTime =  get_ticks();
				
				Key->LongPressTicks++;
				
				if(Key->key_long_pressed_cb)
				{
					Key->key_long_pressed_cb(Key->LongPressTicks); // 触发长按回调函数
				}
			}
		}
	}
	
	/* 处理按钮连击动作 */
	
	if(Key->ClickCnt > 0 && Key->LastState == 0 && (get_ticks() - Key->LastReleasedTime) > Key->ClickInterval)
	{
		if(Key->key_clicked_cb)
		{
			Key->key_clicked_cb(Key->ClickCnt);
		}
		
		Key->ClickCnt = 0; // 清除连击记录
	}
}