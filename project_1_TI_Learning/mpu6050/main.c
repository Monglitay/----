#include "ti_msp_dl_config.h"
#include "mpu6050.h"
#include "usart.h"
int main(void)
{
    SYSCFG_DL_init();
    mpu6050_init();
    USART_Typedef HC06;
    USART_InitTypedef USART_InitStruct;
    USART_InitStruct.usart_inst = HC06_INST;
    USART_InitStruct.usart_inst_int_irqn = HC06_INST_INT_IRQN;
    USART_Init(&HC06,&USART_InitStruct);
    USART_Printf(&HC06, "hello");
    while (1) {
        Read_Quad();
        USART_Printf(&HC06, "%f,%f,%f", pitch,roll,yaw);
    }
}
