#include "hc_06.h"

void HC06_Init()
{
    USART_InitTypedef USART_InitStructure;
    USART_InitStructure.usart_inst = HC_06_INST;
    USART_InitStructure.usart_inst_int_irqn = HC_06_INST_INT_IRQN;
    USART_Init(&HC_06, &USART_InitStructure);
}
