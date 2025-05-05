#include "usart.h"



// 定义USART0接收的字符变量
volatile unsigned char usart0_ch = 0;
// 定义USART0接收的字符串数组
volatile unsigned char usart0_str[256] = {0};
// 定义USART0接收的字符串长度
volatile unsigned int usart0_str_received = 0;
// 定义USART0接收的字符串索引
volatile unsigned int usart0_str_index = 0;

// 定义USART1接收的字符变量
volatile unsigned char usart1_ch = 0;
// 定义USART1接收的字符串数组
volatile unsigned char usart1_str[256] = {0};
// 定义USART1接收的字符串长度
volatile unsigned int usart1_str_received = 0;
// 定义USART1接收的字符串索引
volatile unsigned int usart1_str_index = 0;

// 定义USART2接收的字符变量
volatile unsigned char usart2_ch = 0;
// 定义USART2接收的字符串数组
volatile unsigned char usart2_str[256] = {0};
// 定义USART2接收的字符串长度
volatile unsigned int usart2_str_received = 0;
// 定义USART2接收的字符串索引
volatile unsigned int usart2_str_index = 0;

// 定义USART3接收的字符变量
volatile unsigned char usart3_ch = 0;
// 定义USART3接收的字符串数组
volatile unsigned char usart3_str[256] = {0};
// 定义USART3接收的字符串长度
volatile unsigned int usart3_str_received = 0;
// 定义USART3接收的字符串索引
volatile unsigned int usart3_str_index = 0;


// USART初始化函数
void USART_Init(USART_Typedef *USARTx,USART_InitTypedef *USART_InitStruct)
{
    // 清除USART中断标志
    NVIC_ClearPendingIRQ(USART_InitStruct->usart_inst_int_irqn);
    // 使能USART中断
    NVIC_EnableIRQ(USART_InitStruct->usart_inst_int_irqn);
    
    // 设置USART中断号
    USARTx->usart_inst_int_irqn = USART_InitStruct->usart_inst_int_irqn;
    // 设置USART实例
    USARTx->usart_inst = USART_InitStruct->usart_inst;
}


// 发送一个字符
void USART_SendChar(USART_Typedef *USARTx,char ch)
{
    // 等待USART空闲
    while( DL_UART_isBusy(USARTx->usart_inst) == true );
    // 发送字符
    DL_UART_Main_transmitData(USARTx->usart_inst, ch);
}
// 发送一个字符串
void USART_SendString(USART_Typedef *USARTx,char *str)
{
    
    // 遍历字符串，发送每个字符
    while(*str!=0&&str!=0)
    {
        USART_SendChar(USARTx, *str++);
    }
}

// 接收一个字节
char USART_ReceieveByte(USART_Typedef *USART)
{
    // 根据USART实例返回相应的接收字符
    if(USART->usart_inst == UART1)
    {
        return usart1_ch;
    }
    else if(USART->usart_inst == UART2)
    {
        return usart2_ch;
    }
    else if(USART->usart_inst == UART3)
    {
        return usart3_ch;
    }
    else if(USART->usart_inst == UART0)
    {
        return usart0_ch;
    }
    return 0;
}

// 接收一个字符串
void USART_ReceieveString(USART_Typedef *USART, char pstr[256])
{
    // 根据USART实例接收相应的字符串
    if(USART->usart_inst == UART0)
    {
        // 遍历接收的字符串数组
        for (int i = 0; i <256; i++)
        {
            if(*pstr == 0 && usart0_str[i]==0)break;
            *pstr++ = usart0_str[i];
            usart0_str[i]=0;
            // 如果接收到的字符串为空且当前字符为空，则结束接收
            
        }
        // 重置字符串索引和接收标志
        usart0_str_index = 0;
        usart0_str_received = 0;
    }
    else if(USART->usart_inst == UART1)
    {
        // 遍历接收的字符串数组
        for (int i = 0; i <256; i++)
        {
            if(*pstr == 0 && usart1_str[i]==0)break;
            *pstr++ = usart1_str[i];
            usart1_str[i]=0;
            // 如果接收到的字符串为空且当前字符为空，则结束接收
            
        }
        // 重置字符串索引和接收标志
        usart1_str_index = 0;
        usart1_str_received = 0;
    }
    else if(USART->usart_inst == UART2)
    {
        // 遍历接收的字符串数组
        for (int i = 0; i <256; i++)
        {
            if(*pstr == 0 && usart2_str[i]==0)break;
            *pstr++ = usart2_str[i];
            usart2_str[i]=0;
            // 如果接收到的字符串为空且当前字符为空，则结束接收
            
        }
        
        // 重置接收标志
        usart2_str_index = 0;
        usart2_str_received = 0;
    }
    else if(USART->usart_inst == UART3)
    {
        // 遍历接收的字符串数组
        for (int i = 0; i <256; i++)
        {
            if(*pstr == 0 && usart3_str[i]==0)break;
            *pstr++ = usart3_str[i];
            usart3_str[i]=0;
            // 如果接收到的字符串为空且当前字符为空，则结束接收
            
        }
        // 重置字符串索引和接收标志
        usart3_str_index = 0;
        usart3_str_received = 0;
    }
}

// USART0中断处理函数
void UART0_IRQHandler(void)
{
    // 根据中断类型进行处理
    switch(DL_UART_getPendingInterrupt(UART0))
    {
        case DL_UART_IIDX_RX:
            // 接收字符
            usart0_ch = DL_UART_Main_receiveData(UART0);
            // 如果接收到的字符不是换行符且接收标志为0，则继续接收
            if (usart0_str_index < 255 && usart0_str_received == 0)
            {
                usart0_str[usart0_str_index]=usart0_ch;
                usart0_str_index++;
                // 如果接收到的字符是换行符，则设置接收标志为1，重置索引
                if(usart0_ch == '\n')
                {
                    usart0_str_received = 1;
                    usart0_str_index = 0;
                }
            }

            break;

        default:
            break;
    }
}


// USART1中断处理函数
void UART1_IRQHandler(void)
{
    // 根据中断类型进行处理
    switch(DL_UART_getPendingInterrupt(UART1))
    {
        case DL_UART_IIDX_RX:
            // 接收字符
            usart1_ch = DL_UART_Main_receiveData(UART1);
            // 如果接收到的字符不是换行符且接收标志为0，则继续接收
            if (usart1_str_index < 255 && usart1_str_received == 0)
            {
                usart1_str[usart1_str_index] = usart1_ch;
                usart1_str_index++;
                // 如果接收到的字符是换行符，则设置接收标志为1，重置索引
                if(usart1_ch == '\n')
                {
                    usart1_str_received = 1;
                    usart1_str_index = 0;
                }
            }

            break;

        default:
            break;
    }
}

// USART2中断处理函数
void UART2_IRQHandler(void)
{
    // 根据中断类型进行处理
    switch(DL_UART_getPendingInterrupt(UART2))
    {
        case DL_UART_IIDX_RX:
            // 接收字符
            usart2_ch = DL_UART_Main_receiveData(UART2);
            // 如果接收到的字符不是换行符且接收标志为0，则继续接收
            if (usart2_str_index < 255 && usart2_str_received == 0)
            {
                usart2_str[usart2_str_index] = usart2_ch;
                usart2_str_index++;
                // 如果接收到的字符是换行符，则设置接收标志为1，重置索引
                if(usart2_ch == '\n')
                {
                    usart2_str_received = 1;
                    usart2_str_index = 0;
                }
            }

            break;

        default:
            break;
    }
}

// USART3中断处理函数
void UART3_IRQHandler(void)
{
    // 根据中断类型进行处理
    switch(DL_UART_getPendingInterrupt(UART3))
    {
        case DL_UART_IIDX_RX:
            // 接收字符
            usart3_ch = DL_UART_Main_receiveData(UART3);
            // 如果接收到的字符不是换行符且接收标志为0，则继续接收
            if (usart3_str_index < 255 && usart3_str_received == 0)
            {
                usart3_str[usart3_str_index] = usart3_ch;
                usart3_str_index++;
                // 如果接收到的字符是换行符，则设置接收标志为1，重置索引
                if(usart3_ch == '\n')
                {
                    usart3_str_received = 1;
                    usart3_str_index = 0;
                }
            }

            break;

        default:
            break;
    }
}


