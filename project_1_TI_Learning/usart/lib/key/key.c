#include "key.h"
#include"delay.h"

#define KEY_SETTLING_TIME             10   // ��ť�����ӳ�
#define KEY_CLICK_INTERVAL            200  // ��ť���ʱÿ�ε����ʱ�����ʱ����
#define KEY_LONG_PRESS_THRESHOLD      1000 // ��ť������Сʱ��
#define KEY_LONG_PRESS_TICK_INTERNVAL 100  // ���������������ʱ����

// ��ť����ʱ�Ļص�����
static void OnKeyPressed(Key_TypeDef *Key);
// ��ť�ɿ�ʱ�Ļص�����
static void OnKeyReleased(Key_TypeDef *Key);
// ÿ����ѯ��ť״̬ʱ�Ļص�����
static void OnKeyEveryPolled(Key_TypeDef *Key, uint8_t State, uint32_t currentTime);

// ��ʼ����ť�ṹ��
void Key_Init(Key_TypeDef *Key,Key_InitTypedef *Key_Initstruct)
{
    Key->key_port = Key_Initstruct->key_port; // ���ð�ť�˿�
    Key->key_pin = Key_Initstruct->key_pin;   // ���ð�ť����
    Key->key_pressed_cb = Key_Initstruct->key_pressed_cb; // ���ð�ť���»ص�����
    Key->key_released_cb = Key_Initstruct->key_released_cb; // ���ð�ť�ɿ��ص�����
    Key->key_long_pressed_cb = Key_Initstruct->key_long_pressed_cb; // ���ð�ť�����ص�����
    // ���δ���ó�����ֵ����ʹ��Ĭ��ֵ
    if(Key->LongPressThreshold == 0)
	{
		Key->LongPressThreshold = KEY_LONG_PRESS_THRESHOLD;
	}
	
    // ���δ���ó��������������ʹ��Ĭ��ֵ
	if(Key->LongPressTickInterval == 0)
	{
		Key->LongPressTickInterval = KEY_LONG_PRESS_TICK_INTERNVAL;
	}
	
    // ���δ���õ���������ʹ��Ĭ��ֵ
	if(Key->ClickInterval == 0)
	{
		Key->ClickInterval = KEY_CLICK_INTERVAL;
	}

    // ��ʼ����ť״̬
    Key->LastState = 0; // ��ʼ״̬�¼��谴ť���ɿ���
	Key->ChangePending = 0; 
	Key->PendingTime = 0;
	Key->LastPressedTime = 0;
	Key->LastReleasedTime = 0;
	Key->LongPressTicks = 0;
	Key->ClickCnt = 0;
}

// ����ť״̬
void Key_Proc(Key_TypeDef *Key)
{
    uint8_t currentState;
    uint32_t currentTime = get_ticks();

    // �����״̬�仯������
    if(Key->ChangePending)
    {
        // ����Ѿ���������ʱ��
        if(currentTime - Key->PendingTime + KEY_SETTLING_TIME)
        {
            // ��ȡ��ǰ��ť״̬
            currentState = DL_GPIO_readPins(Key->key_port, Key->key_pin) > 0 ? 1 : 0;
            // ���״̬����һ��״̬��ͬ
            if(currentState != Key->LastState)
            {
                if(currentState == 1) 
					OnKeyPressed(Key); // #1. ��ť����
				else 
					OnKeyReleased(Key); // #2. ��ť�ɿ�
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
	
	// ���ð�ť���µĻص�����
	if(Key->key_pressed_cb != 0)
	{
		Key->key_pressed_cb();
	}
}
static void OnKeyReleased(Key_TypeDef *Key)
{
	Key->LastReleasedTime = get_ticks();
	
	// ���ð�ť�ɿ��Ļص�����
	if(Key->key_released_cb != 0)
	{
		Key->key_released_cb();
	}
	
	// �ɿ��󳤰���������
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
// @��飺����ÿһ�ΰ�ť��ѯ�Ķ���
//
static void OnKeyEveryPolled(Key_TypeDef *Key, uint8_t State, uint32_t CurrentTime)
{
	/* ����ť�����Ķ��� */
	
	if(Key->LastState == 1)
	{
		if(Key->LongPressTicks == 0) // �������δ������
		{
			if(Key->LastPressedTime!= 0 
				&& CurrentTime - Key->LastPressedTime > Key->LongPressThreshold) // ���ѳ�������ʱ��
			{
				Key->LongPressTicks = 1;
			
				if(Key->key_long_pressed_cb)
				{
					Key->key_long_pressed_cb(Key->LongPressTicks); // ���������ص�����
				}
				
				Key->LastLongPressTickTime =  get_ticks();
			}
		}
		else
		{
			if(CurrentTime - Key->LastLongPressTickTime > Key->LongPressTickInterval) // ����Tick���
			{
				Key->LastLongPressTickTime =  get_ticks();
				
				Key->LongPressTicks++;
				
				if(Key->key_long_pressed_cb)
				{
					Key->key_long_pressed_cb(Key->LongPressTicks); // ���������ص�����
				}
			}
		}
	}
	
	/* ����ť�������� */
	
	if(Key->ClickCnt > 0 && Key->LastState == 0 && (get_ticks() - Key->LastReleasedTime) > Key->ClickInterval)
	{
		if(Key->key_clicked_cb)
		{
			Key->key_clicked_cb(Key->ClickCnt);
		}
		
		Key->ClickCnt = 0; // ���������¼
	}
}