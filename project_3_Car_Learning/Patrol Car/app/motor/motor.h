#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "ti_msp_dl_config.h"
#include "pid.h"
// 定义PID结构体变量
PID_Tpyedef pid_l_high_speed;
PID_Tpyedef pid_r_high_speed;
PID_Tpyedef pid_l_low_speed;
PID_Tpyedef pid_r_low_speed;
PID_Tpyedef pid_direction;
PID_Tpyedef pid_tracking;

// 定义电机占空比变量
float duty_l;
float duty_r;
float duty_diff;
// 定义跟踪标志变量
int tracking_flag;
// 初始化电机函数
void Motor_Init();
// 处理电机函数
void Motor_Proc();
// 设置右电机速度函数
void Motor_SetSpeed_R(float speed);
// 设置左电机速度函数
void Motor_SetSpeed_L(float speed);
// 设置电机方向函数
void Motor_direction(float direction);
#endif