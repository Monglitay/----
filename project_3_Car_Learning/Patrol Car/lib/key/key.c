#include "key.h"
#include "delay.h"

// ������ص�ʱ�䳣������
#define KEY_SETTLING_TIME             10   // ��ť�����ӳ٣���λ�����룩
#define KEY_CLICK_INTERVAL            200  // ��ť���ʱÿ�ε�������ʱ��������λ�����룩
#define KEY_LONG_PRESS_THRESHOLD      1000 // ��ť������Сʱ�䣨��λ�����룩
#define KEY_LONG_PRESS_TICK_INTERNVAL 100  // ���������������ʱ��������λ�����룩

// ��ť�¼��ص�����ԭ��
static void OnKeyPressed(Key_TypeDef *Key);
static void OnKeyReleased(Key_TypeDef *Key);
static void OnKeyEveryPolled(Key_TypeDef *Key, uint8_t State, uint32_t currentTime);

/**
 * @brief ��ʼ����ť�ṹ��
 * @param Key ��ť�ṹ��ָ��
 * @param Key_Initstruct ��ť��ʼ���ṹ��ָ��
 */
void Key_Init(Key_TypeDef *Key, Key_InitTypedef *Key_Initstruct)
{
    // ���ð�ťӲ������
    Key->key_port = Key_Initstruct->key_port;
    Key->key_pin = Key_Initstruct->key_pin;
    
    // ���ûص�����
    Key->key_pressed_cb = Key_Initstruct->key_pressed_cb;
    Key->key_released_cb = Key_Initstruct->key_released_cb;
    Key->key_long_pressed_cb = Key_Initstruct->key_long_pressed_cb;
    
    // ����Ĭ��ʱ�����
    Key->LongPressThreshold = Key_Initstruct->LongPressThreshold ?: KEY_LONG_PRESS_THRESHOLD;
    Key->LongPressTickInterval = Key_Initstruct->LongPressTickInterval ?: KEY_LONG_PRESS_TICK_INTERNVAL;
    Key->ClickInterval = Key_Initstruct->ClickInterval ?: KEY_CLICK_INTERVAL;

    // ��ʼ����ť״̬
    Key->LastState = 0;
    Key->ChangePending = 0;
    Key->PendingTime = 0;
    Key->LastPressedTime = 0;
    Key->LastReleasedTime = 0;
    Key->LongPressTicks = 0;
    Key->ClickCnt = 0;
}

/**
 * @brief ����ť״̬
 * @param Key ��ť�ṹ��ָ��
 */
void Key_Proc(Key_TypeDef *Key)
{
    uint8_t currentState;
    uint32_t currentTime = get_ticks();

    if (Key->ChangePending)
    {
        // ��������
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
 * @brief ��ȡ��ť��ǰ״̬
 * @param Key ��ť�ṹ��ָ��
 * @return ��ť״̬��0���ɿ���1�����£�
 */
uint8_t Key_GetState(Key_TypeDef *Key)
{
    return Key->LastState;
}

/**
 * @brief ��ť�����¼�����
 * @param Key ��ť�ṹ��ָ��
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
 * @brief ��ť�ɿ��¼�����
 * @param Key ��ť�ṹ��ָ��
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
 * @brief ��ť��ѯ����
 * @param Key ��ť�ṹ��ָ��
 * @param State ��ǰ��ť״̬
 * @param CurrentTime ��ǰʱ��
 */
static void OnKeyEveryPolled(Key_TypeDef *Key, uint8_t State, uint32_t CurrentTime)
{
    // ������
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
    
    // ������
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