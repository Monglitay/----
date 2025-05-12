#ifndef __USART_H_  // 如果未定义宏__USART_H_，则编译以下代码，防止头文件被多次包含
#define __USART_H_  // 定义宏__USART_H_，表示该头文件已被包含

#include "ti_msp_dl_config.h"  // 包含TI MSP430的驱动配置头文件

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

void USART_Printf(USART_Typedef *USARTx, const char *Format, ...);

#endif /* __USART_H_ */  // 结束#ifndef __USART_H_的条件编译