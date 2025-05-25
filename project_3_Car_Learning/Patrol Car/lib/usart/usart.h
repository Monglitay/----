#ifndef __USART_H_  
#define __USART_H_  
#include "ti_msp_dl_config.h"

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
// ����һ������������ͨ��USART��������
void USART_Printf(USART_Typedef *USARTx,const char *Format, ...);
// ����һ�����������ڴ���UART�ж�
void UART_IRQHandler(UART_Regs *UART, volatile unsigned char usart_ch, volatile unsigned char usart_str[256],
                     volatile unsigned int *usart_str_index, volatile unsigned int *usart_str_received);
#endif