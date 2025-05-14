#include "motor.h"
#include "task.h"
#include "encoder.h"
void Motor_Init()
{
    PID_Init(&pid_l_speed,0.5,7,0);
    PID_Init(&pid_r_speed,0.5,7,0);
}
void Motor_Proc()
{
    PERIODIC(1);
    float omeg_l = Encoder_Get_L_Speed();
}