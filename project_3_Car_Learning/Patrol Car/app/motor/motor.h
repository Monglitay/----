#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "ti_msp_dl_config.h"
#include "pid.h"
PID_Tpyedef pid_l_speed;
PID_Tpyedef pid_r_speed;
void Motor_Init();
#endif