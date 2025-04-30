#include "usart.h"



// ����USART0���յ��ַ�����
volatile unsigned char usart0_ch = 0;
// ����USART0���յ��ַ�������
volatile unsigned char usart0_str[256] = {0};
// ����USART0���յ��ַ�������
volatile unsigned int usart0_str_received = 0;
// ����USART0���յ��ַ�������
volatile unsigned int usart0_str_index = 0;

// ����USART1���յ��ַ�����
volatile unsigned char usart1_ch = 0;
// ����USART1���յ��ַ�������
volatile unsigned char usart1_str[256] = {0};
// ����USART1���յ��ַ�������
volatile unsigned int usart1_str_received = 0;
// ����USART1���յ��ַ�������
volatile unsigned int usart1_str_index = 0;

// ����USART2���յ��ַ�����
volatile unsigned char usart2_ch = 0;
// ����USART2���յ��ַ�������
volatile unsigned char usart2_str[256] = {0};
// ����USART2���յ��ַ�������
volatile unsigned int usart2_str_received = 0;
// ����USART2���յ��ַ�������
volatile unsigned int usart2_str_index = 0;

// ����USART3���յ��ַ�����
volatile unsigned char usart3_ch = 0;
// ����USART3���յ��ַ�������
volatile unsigned char usart3_str[256] = {0};
// ����USART3���յ��ַ�������
volatile unsigned int usart3_str_received = 0;
// ����USART3���յ��ַ�������
volatile unsigned int usart3_str_index = 0;


// USART��ʼ������
void USART_Init(USART_Typedef *USARTx,USART_InitTypedef *USART_InitStruct)
{
    // ���USART�жϱ�־
    NVIC_ClearPendingIRQ(USART_InitStruct->usart_inst_int_irqn);
    // ʹ��USART�ж�
    NVIC_EnableIRQ(USART_InitStruct->usart_inst_int_irqn);
    
    // ����USART�жϺ�
    USARTx->usart_inst_int_irqn = USART_InitStruct->usart_inst_int_irqn;
    // ����USARTʵ��
    USARTx->usart_inst = USART_InitStruct->usart_inst;
}


// ����һ���ַ�
void USART_SendChar(USART_Typedef *USARTx,char ch)
{
    // �ȴ�USART����
    while( DL_UART_isBusy(USARTx->usart_inst) == true );
    // �����ַ�
    DL_UART_Main_transmitData(USARTx->usart_inst, ch);
}
// ����һ���ַ���
void USART_SendString(USART_Typedef *USARTx,char *str)
{
    
    // �����ַ���������ÿ���ַ�
    while(*str!=0&&str!=0)
    {
        USART_SendChar(USARTx, *str++);
    }
}

// ����һ���ֽ�
char USART_ReceieveByte(USART_Typedef *USART)
{
    // ����USARTʵ��������Ӧ�Ľ����ַ�
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

// ����һ���ַ���
void USART_ReceieveString(USART_Typedef *USART, char pstr[256])
{
    // ����USARTʵ��������Ӧ���ַ���
    if(USART->usart_inst == UART0)
    {
        // �������յ��ַ�������
        for (int i = 0; i <256; i++)
        {
            *pstr++ = usart0_str[i];
            usart0_str[i]=0;
            // ������յ����ַ���Ϊ���ҵ�ǰ�ַ�Ϊ�գ����������
            if(*pstr == 0 && usart0_str[i]==0)break;
        }
        // �����ַ��������ͽ��ձ�־
        usart0_str_index = 0;
        usart0_str_received = 0;
    }
    else if(USART->usart_inst == UART1)
    {
        // �������յ��ַ�������
        for (int i = 0; i <256; i++)
        {
            *pstr++ = usart1_str[i];
            usart1_str[i]=0;
            // ������յ����ַ���Ϊ���ҵ�ǰ�ַ�Ϊ�գ����������
            if(*pstr == 0 && usart1_str[i]==0)break;
        }
        // �����ַ��������ͽ��ձ�־
        usart1_str_index = 0;
        usart1_str_received = 0;
    }
    else if(USART->usart_inst == UART2)
    {
        // �������յ��ַ�������
        for (int i = 0; i <256; i++)
        {
            *pstr++ = usart2_str[i];
            usart2_str[i]=0;
            // ������յ����ַ���Ϊ���ҵ�ǰ�ַ�Ϊ�գ����������
            if(*pstr == 0 && usart2_str[i]==0)break;
        }
        
        // ���ý��ձ�־
        usart2_str_received = 0;
    }
    else if(USART->usart_inst == UART3)
    {
        // �������յ��ַ�������
        for (int i = 0; i <256; i++)
        {
            *pstr++ = usart3_str[i];
            usart3_str[i]=0;
            // ������յ����ַ���Ϊ���ҵ�ǰ�ַ�Ϊ�գ����������
            if(*pstr == 0 && usart3_str[i]==0)break;
        }
        // �����ַ��������ͽ��ձ�־
        usart3_str_received = 0;
    }
}

// USART0�жϴ�����
void UART0_IRQHandler(void)
{
    // �����ж����ͽ��д���
    switch(DL_UART_getPendingInterrupt(UART0))
    {
        case DL_UART_IIDX_RX:
            // �����ַ�
            usart0_ch = DL_UART_Main_receiveData(UART0);
            // ������յ����ַ����ǻ��з��ҽ��ձ�־Ϊ0�����������
            if (usart0_str_index < 255 && usart0_str_received == 0)
            {
                usart0_str[usart1_str_index]=usart0_ch;
                usart0_str_index++;
                // ������յ����ַ��ǻ��з��������ý��ձ�־Ϊ1����������
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


// USART1�жϴ�����
void UART1_IRQHandler(void)
{
    // �����ж����ͽ��д���
    switch(DL_UART_getPendingInterrupt(UART1))
    {
        case DL_UART_IIDX_RX:
            // �����ַ�
            usart1_ch = DL_UART_Main_receiveData(UART1);
            // ������յ����ַ����ǻ��з��ҽ��ձ�־Ϊ0�����������
            if (usart1_str_index < 255 && usart1_str_received == 0)
            {
                usart1_str[usart1_str_index] = usart1_ch;
                usart1_str_index++;
                // ������յ����ַ��ǻ��з��������ý��ձ�־Ϊ1����������
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

// USART2�жϴ�����
void UART2_IRQHandler(void)
{
    // �����ж����ͽ��д���
    switch(DL_UART_getPendingInterrupt(UART2))
    {
        case DL_UART_IIDX_RX:
            // �����ַ�
            usart2_ch = DL_UART_Main_receiveData(UART2);
            // ������յ����ַ����ǻ��з��ҽ��ձ�־Ϊ0�����������
            if (usart2_str_index < 255 && usart2_str_received == 0)
            {
                usart2_str[usart2_str_index] = usart2_ch;
                usart2_str_index++;
                // ������յ����ַ��ǻ��з��������ý��ձ�־Ϊ1����������
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

// USART3�жϴ�����
void UART3_IRQHandler(void)
{
    // �����ж����ͽ��д���
    switch(DL_UART_getPendingInterrupt(UART3))
    {
        case DL_UART_IIDX_RX:
            // �����ַ�
            usart3_ch = DL_UART_Main_receiveData(UART3);
            // ������յ����ַ����ǻ��з��ҽ��ձ�־Ϊ0�����������
            if (usart3_str_index < 255 && usart2_str_received == 0)
            {
                usart3_str[usart3_str_index] = usart3_ch;
                usart3_str_index++;
                // ������յ����ַ��ǻ��з��������ý��ձ�־Ϊ1����������
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


