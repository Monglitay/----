#ifndef __PID_H
#define __PID_H

#include "stdint.h"

#define INTEGRAL_LIMIT 100.0

typedef struct {
    float kp;           // 比例系数
    float ki;           // 积分系数
    float kd;           // 微分系数
    float sp;           // 设定值
    float err;          // 当前误差
    float last_err;     // 上一次误差
    float last2_err;    // 上上次误差
    float co;           // 控制输出

}  PID_Typedef;

// PID初始化
void PID_Init(PID_Typedef *PIDx, float kp, float ki, float kd);

// PID计算
float PID_Compute(PID_Typedef *PIDx, float fb);

// 方向PID计算（角度特殊处理）
float PID_Compute_Direction(PID_Typedef *pid, float feedback);

// 重置PID
void PID_Reset(PID_Typedef *pid);

// 修改设定值
void PID_ChangeSP(PID_Typedef *PIDx, float sp);

void PID_Limmit(float *co, float Upper, float Lower);


float PID_Compute_dir(PID_Typedef *PIDx, float fb);

#endif
