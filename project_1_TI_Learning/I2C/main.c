
#include "ti_msp_dl_config.h"
#include "mpu6050.h"
#include "usart.h"
int main(void)
{
    SYSCFG_DL_init();
    USART_InitTypedef USART_InitStructure;
    USART_InitStructure.usart_inst = HC06_INST;
    USART_InitStructure.usart_inst_int_irqn = HC06_INST_INT_IRQN;
    USART_Typedef HC06;
    USART_Init(&HC06,&USART_InitStructure);

    MPU6050_Init();
    if(MPU6050ReadID())
    {
        USART_Printf(&HC06, "hello");
    }
    else
    {
        USART_Printf(&HC06, "no");
    }
    while (1) 
    {
        MPU6050_Proc();
        uint32_t ticks=get_ticks();
        float yaw = Get_yaw();
        float pitch = Get_pitch();
        float roll = Get_roll();
        float gz = Get_gz();
        USART_Printf(&HC06, "%f,%f,%f,%f\n",yaw,pitch,roll,gz);
    }
}