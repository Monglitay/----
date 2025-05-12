#ifndef __USART_H_  // ���δ�����__USART_H_����������´��룬��ֹͷ�ļ�����ΰ���
#define __USART_H_  // �����__USART_H_����ʾ��ͷ�ļ��ѱ�����

#include "ti_msp_dl_config.h"  // ����TI MSP430����������ͷ�ļ�

// ����USART��ʼ���ṹ��
typedef struct
{
    UART_Regs * usart_inst;  // ָ��USARTʵ����ָ��
    int usart_inst_int_irqn; // USARTʵ�����жϺ�
}USART_InitTypedef;

// ����USART���ͽṹ��
typedef struct
{
    UART_Regs * usart_inst;  // ָ��USARTʵ����ָ��
    int usart_inst_int_irqn; // USARTʵ�����жϺ�
}USART_Typedef;

// ��ʼ��USART
void USART_Init(USART_Typedef *USARTx,USART_InitTypedef *USART_InitStruct);
// ��USART����һ���ֽ�
char USART_ReceieveByte(USART_Typedef *USARTx);
// ��USART�����ַ���
void USART_ReceieveString(USART_Typedef *USART, char *pstr);
// ͨ��USART�����ַ���
void USART_SendString(USART_Typedef *USARTx,char *str);
// ͨ��USART����һ���ַ�
void USART_SendChar(USART_Typedef *USARTx,char ch);
void USART_Printf(USART_Typedef *USARTx, const char *Format, ...);

#endif /* __USART_H_ */  // ����#ifndef __USART_H_����������