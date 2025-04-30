#ifndef __KEY_H_  // ���δ�����__KEY_H_����������´���
#define __KEY_H_  // �����__KEY_H_����ֹ�ظ�������ͷ�ļ�

#include "ti_msp_dl_config.h"  // ����TI MSPϵ��΢����������������ͷ�ļ�


// ���尴����ʼ���ṹ��
typedef struct 
{
    GPIO_Regs *key_port;  // ָ�򰴼������ӵ�GPIO�˿ڵ�ָ��
    unsigned int key_pin; // ���������ӵ�GPIO����
    void (*key_pressed_cb)(void); // ��������ʱ�Ļص�����ָ��
    void (*key_released_cb)(void); // �����ɿ�ʱ�Ļص�����ָ��
    void (*key_clicked_cb)(uint8_t clicks); // ���������������˫���ȣ�ʱ�Ļص�����ָ��
	void (*key_long_pressed_cb)(uint8_t ticks); // ��������ʱ�Ļص�����ָ��
    
}Key_InitTypedef;


// ���尴���ṹ��
typedef struct 
{
	/* ��ʼ������ */
	GPIO_Regs *key_port; // ָ�򰴼������ӵ�GPIO�˿ڵ�ָ��
    unsigned int key_pin; // ���������ӵ�GPIO����
    uint32_t  key_iidx; // �������ж�����
    int key_irqn; // �������жϺ�
    uint32_t LongPressTime; // ��ť����ʱ����ֵ����λ����
	uint32_t LongPressTickInterval; // ��������������ļ��
	uint32_t ClickInterval; // ����ʱ��������λ����
    void (*key_pressed_cb)(void); // ��������ʱ�Ļص�����ָ��
    void (*key_released_cb)(void); // �����ɿ�ʱ�Ļص�����ָ��
    void (*key_clicked_cb)(uint8_t clicks); // ���������������˫���ȣ�ʱ�Ļص�����ָ��
	void (*key_long_pressed_cb)(uint8_t ticks); // ��������ʱ�Ļص�����ָ��
	
	uint8_t  LastState;     // ��ť�ϴε�״̬��0 - �ɿ���1 - ����
	uint8_t  ChangePending; // ��ť��״̬�Ƿ����ڷ����ı�
	uint32_t PendingTime;   // ��ť״̬��ʼ�仯��ʱ��
	uint32_t LongPressThreshold; // ����ʱ����ֵ

	uint32_t LastPressedTime;  // ��ť�ϴΰ��µ�ʱ��
	uint32_t LastReleasedTime; // ��ť�ϴ��ɿ���ʱ��
	
	uint8_t LongPressTicks; // ��������
	uint32_t LastLongPressTickTime;  // �ϴγ���������ʱ��
	
	uint8_t ClickCnt; // �������
	
}Key_TypeDef;




// ��ʼ������
void Key_Init(Key_TypeDef *Key,Key_InitTypedef *Key_Initstruct);
// ������״̬
void Key_Proc(Key_TypeDef *Key);
// ��ȡ����״̬
uint8_t Key_GetState(Key_TypeDef *Key);
#endif 