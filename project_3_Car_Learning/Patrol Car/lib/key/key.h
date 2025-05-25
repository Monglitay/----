#ifndef __KEY_H_
#define __KEY_H_

#include "ti_msp_dl_config.h"

// ���尴����ʼ���ṹ��
typedef struct 
{
    GPIO_Regs *key_port; // �����˿�
    unsigned int key_pin; // ��������
    void (*key_pressed_cb)(void); // �������»ص�����
    void (*key_released_cb)(void); // �����ɿ��ص�����
    void (*key_clicked_cb)(uint8_t clicks); // ���������ص�����
    void (*key_long_pressed_cb)(uint8_t ticks); // ���������ص�����
    
    uint32_t LongPressThreshold;     // ������ֵ
    uint32_t LongPressTickInterval;  // �����������
    uint32_t ClickInterval;          // �������
}Key_InitTypedef;

// ԭ�е� Key_TypeDef �ṹ�屣�ֲ���
typedef struct 
{
    /* ��ʼ������ */
    GPIO_Regs *key_port;
    unsigned int key_pin;
    uint32_t  key_iidx;
    int key_irqn;
    uint32_t LongPressTime; // ��ť����ʱ����ֵ����λ����
    uint32_t LongPressTickInterval; // ��������������ļ��
    uint32_t ClickInterval; // ����ʱ��������λ����
    void (*key_pressed_cb)(void);
    void (*key_released_cb)(void);
    void (*key_clicked_cb)(uint8_t clicks);
    void (*key_long_pressed_cb)(uint8_t ticks);
    
    uint8_t  LastState;     // ��ť�ϴε�״̬��0 - �ɿ���1 - ����
    uint8_t  ChangePending; // ��ť��״̬�Ƿ����ڷ����ı�
    uint32_t PendingTime;   // ��ť״̬��ʼ�仯��ʱ��
    uint32_t LongPressThreshold;

    uint32_t LastPressedTime;  // ��ť�ϴΰ��µ�ʱ��
    uint32_t LastReleasedTime; // ��ť�ϴ��ɿ���ʱ��
    
    // ��������
    uint8_t LongPressTicks;
    // �ϴγ���ʱ��
    uint32_t LastLongPressTickTime; 
    
    // �������
    uint8_t ClickCnt;
    
}Key_TypeDef;

// ��ʼ������
void Key_Init(Key_TypeDef *Key,Key_InitTypedef *Key_Initstruct);
// ������
void Key_Proc(Key_TypeDef *Key);
// ��ȡ����״̬
uint8_t Key_GetState(Key_TypeDef *Key);
#endif 
