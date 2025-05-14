#include "tb6612.h"


void SetSpeed_R(int dir, uint32_t speed)
{
        if( dir == 1 )
        {
                AIN1_OUT(0);
                AIN2_OUT(1);
        }
        else if (dir == -1)
        {
                AIN1_OUT(1);
                AIN2_OUT(0);
        }
        else if (dir == 0)
        {
                AIN1_OUT(0);
                AIN2_OUT(0);
        }
        DL_TimerG_setCaptureCompareValue(PWM_R_INST,speed,GPIO_PWM_R_C0_IDX);
}
void SetSpeed_L(int dir, uint32_t speed)
{
        if( dir == 1 )
        {
                BIN1_OUT(1);
                BIN2_OUT(0);
        }
        else if (dir == -1)
        {
                BIN1_OUT(0);
                BIN2_OUT(1);
        }
        else if (dir == 0)
        {
                BIN1_OUT(0);
                BIN2_OUT(0);
        }
        DL_TimerG_setCaptureCompareValue(PWM_L_INST,speed,GPIO_PWM_L_C1_IDX);
}