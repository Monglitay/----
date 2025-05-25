#ifndef __USART_H_  
#define __USART_H_  
#include "ti_msp_dl_config.h"

// 定义USART初始化结构体
typedef struct
{
    UART_Regs * usart_inst;  // 指向USART实例的指针
    int usart_inst_int_irqn; // USART实例的中断号
}USART_InitTypedef;

// 定义USART类型结构体
typedef struct
{
    UART_Regs * usart_inst;  // 指向USART实例的指针
    int usart_inst_int_irqn; // USART实例的中断号
}USART_Typedef;

// 初始化USART
void USART_Init(USART_Typedef *USARTx,USART_InitTypedef *USART_InitStruct);
// 从USART接收一个字节
char USART_ReceieveByte(USART_Typedef *USARTx);
// 从USART接收字符串
void USART_ReceieveString(USART_Typedef *USART, char *pstr);
// 通过USART发送字符串
void USART_SendString(USART_Typedef *USARTx,char *str);
// 通过USART发送一个字符
void USART_SendChar(USART_Typedef *USARTx,char ch);
// 声明一个函数，用于通过USART发送数据
void USART_Printf(USART_Typedef *USARTx,const char *Format, ...);
// 声明一个函数，用于处理UART中断
void UART_IRQHandler(UART_Regs *UART, volatile unsigned char usart_ch, volatile unsigned char usart_str[256],
                     volatile unsigned int *usart_str_index, volatile unsigned int *usart_str_received);
#endif