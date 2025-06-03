#ifndef __MOTOR_H
#define __MOTOR_H

#include "stdint.h"
#include "pid.h"

extern uint8_t tracking_flag;

// 电机控制初始化
void Motor_Init(void);

// 设置左电机目标速度
void Motor_SetSpeed_L(float speed);

// 设置右电机目标速度
void Motor_SetSpeed_R(float speed);

// 设置方向控制目标值
void Motor_direction(float direction);

// 电机周期性控制处理
void Motor_Proc(void);

#endif
