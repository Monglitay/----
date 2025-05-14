#include "ti_msp_dl_config.h"
#include "encoder.h"
#include "hc_06.h"
#include "delay.h"
#include "tb6612.h"
int main(void)
{
    SYSCFG_DL_init();
    Encoder_Init();
    HC06_Init();
    DL_GPIO_clearPins(TB6612_STBY_PORT, TB6612_STBY_PIN);
    while (1) {
        float L=Encoder_Get_L();
        float R=Encoder_Get_R();
        float L_speed=Encoder_Get_L_Speed();
        float R_speed=Encoder_Get_R_Speed();
        USART_Printf(&HC_06, "%f,%f,%f,%f\n", L,R,L_speed,R_speed);
    }
}
