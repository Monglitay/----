#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// USART0-3 接收相关变量
volatile unsigned char usart0_ch = 0, usart1_ch = 0, usart2_ch = 0, usart3_ch = 0;
volatile unsigned char usart0_str[256] = {0}, usart1_str[256] = {0}, usart2_str[256] = {0}, usart3_str[256] = {0};
volatile unsigned int usart0_str_received = 0, usart1_str_received = 0, usart2_str_received = 0, usart3_str_received = 0;
volatile unsigned int usart0_str_index = 0, usart1_str_index = 0, usart2_str_index = 0, usart3_str_index = 0;

/**
 * @brief 初始化USART
 * @param USARTx USART结构体指针
 * @param USART_InitStruct USART初始化结构体指针
 */
void USART_Init(USART_Typedef *USARTx, USART_InitTypedef *USART_InitStruct)
{
    NVIC_ClearPendingIRQ(USART_InitStruct->usart_inst_int_irqn);
    NVIC_EnableIRQ(USART_InitStruct->usart_inst_int_irqn);
    
    USARTx->usart_inst_int_irqn = USART_InitStruct->usart_inst_int_irqn;
    USARTx->usart_inst = USART_InitStruct->usart_inst;
}

/**
 * @brief 发送单个字符
 * @param USARTx USART结构体指针
 * @param ch 要发送的字符
 */
void USART_SendChar(USART_Typedef *USARTx, char ch)
{
    while(DL_UART_isBusy(USARTx->usart_inst) == true);
    DL_UART_Main_transmitData(USARTx->usart_inst, ch);
}

/**
 * @brief 发送字符串
 * @param USARTx USART结构体指针
 * @param str 要发送的字符串
 */
void USART_SendString(USART_Typedef *USARTx, char *str)
{
    while(*str != 0 && str != 0)
    {
        USART_SendChar(USARTx, *str++);
    }
}

/**
 * @brief 格式化输出到USART
 * @param USARTx USART结构体指针
 * @param Format 格式化字符串
 * @param ... 可变参数列表
 */
void USART_Printf(USART_Typedef *USARTx, const char *Format, ...)
{
    char format_buffer[128];
    va_list argptr;
    
    va_start(argptr, Format);
    vsprintf(format_buffer, Format, argptr);
    va_end(argptr);
    
    USART_SendString(USARTx, format_buffer);
}

/**
 * @brief 接收单个字节
 * @param USART USART结构体指针
 * @return 接收到的字符
 */
char USART_ReceieveByte(USART_Typedef *USART)
{
    if(USART->usart_inst == UART1) return usart1_ch;
    else if(USART->usart_inst == UART2) return usart2_ch;
    else if(USART->usart_inst == UART3) return usart3_ch;
    else if(USART->usart_inst == UART0) return usart0_ch;
    return 0;
}

/**
 * @brief 接收字符串
 * @param USART USART结构体指针
 * @param pstr 存储接收字符串的数组
 */
void USART_ReceieveString(USART_Typedef *USART, char pstr[256])
{
    volatile unsigned char *usart_str;
    volatile unsigned int *usart_str_index, *usart_str_received;

    if(USART->usart_inst == UART0) {
        usart_str = usart0_str;
        usart_str_index = &usart0_str_index;
        usart_str_received = &usart0_str_received;
    } else if(USART->usart_inst == UART1) {
        usart_str = usart1_str;
        usart_str_index = &usart1_str_index;
        usart_str_received = &usart1_str_received;
    } else if(USART->usart_inst == UART2) {
        usart_str = usart2_str;
        usart_str_index = &usart2_str_index;
        usart_str_received = &usart2_str_received;
    } else if(USART->usart_inst == UART3) {
        usart_str = usart3_str;
        usart_str_index = &usart3_str_index;
        usart_str_received = &usart3_str_received;
    } else {
        return;
    }

    for (int i = 0; i < 256; i++)
    {
        if(*pstr == 0 && usart_str[i] == 0) break;
        *pstr++ = usart_str[i];
        usart_str[i] = 0;
    }
    *usart_str_index = 0;
    *usart_str_received = 0;
}

// USART0-3 中断处理函数
void UART0_IRQHandler(void) { UART_IRQHandler(UART0, usart0_ch, usart0_str, &usart0_str_index, &usart0_str_received); }
void UART1_IRQHandler(void) { UART_IRQHandler(UART1, usart1_ch, usart1_str, &usart1_str_index, &usart1_str_received); }
void UART2_IRQHandler(void) { UART_IRQHandler(UART2, usart2_ch, usart2_str, &usart2_str_index, &usart2_str_received); }
void UART3_IRQHandler(void) { UART_IRQHandler(UART3, usart3_ch, usart3_str, &usart3_str_index, &usart3_str_received); }

/**
 * @brief 通用UART中断处理函数
 * @param UART UART实例
 * @param usart_ch 接收字符变量
 * @param usart_str 接收字符串数组
 * @param usart_str_index 接收字符串索引指针
 * @param usart_str_received 接收完成标志指针
 */
void UART_IRQHandler(UART_Regs *UART, volatile unsigned char usart_ch, volatile unsigned char usart_str[256],
                     volatile unsigned int *usart_str_index, volatile unsigned int *usart_str_received)
{
    switch(DL_UART_getPendingInterrupt(UART))
    {
        case DL_UART_IIDX_RX:
            usart_ch = DL_UART_Main_receiveData(UART);
            if (*usart_str_index < 255 && *usart_str_received == 0)
            {
                usart_str[*usart_str_index] = usart_ch;
                (*usart_str_index)++;
                if(usart_ch == '\n')
                {
                    usart_str[*usart_str_index] = '\0';
                    *usart_str_received = 1;
                    *usart_str_index = 0;
                }
            }
            break;
        default:
            break;
    }
}