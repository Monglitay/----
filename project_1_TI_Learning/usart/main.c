#include "ti_msp_dl_config.h"
#include "usart.h"
int main(void)
{
    SYSCFG_DL_init();
    USART_InitTypedef USART_InitStruct;
    USART_InitStruct.usart_inst = UART_0_INST;
    USART_InitStruct.usart_inst_int_irqn = UART_0_INST_INT_IRQN;
    USART_Typedef My_USART0;

    USART_Init(&My_USART0,&USART_InitStruct);
    USART_SendString(&My_USART0,"hello");
    while (1) {
        char pstr[256]={0};
        USART_ReceieveString(&My_USART0,pstr);
        USART_SendString(&My_USART0,pstr);
    }
}
    