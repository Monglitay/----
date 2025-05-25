#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "ti_msp_dl_config.h"
#include "pid.h"
PID_Tpyedef pid_l_high_speed;
PID_Tpyedef pid_r_high_speed;
PID_Tpyedef pid_l_low_speed;
PID_Tpyedef pid_r_low_speed;
PID_Tpyedef pid_direction;
PID_Tpyedef pid_tracking;
float duty_l;
float duty_r;
float duty_diff;
int tracking_flag;
void Motor_Init();
void Motor_Proc();
void Motor_SetSpeed_R(float speed);
void Motor_SetSpeed_L(float speed);
void Motor_direction(float direction);
#endif