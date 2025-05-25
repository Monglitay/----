#ifndef __PID_H_
#define __PID_H_
#include "ti_msp_dl_config.h"  
#include "pid.h"
// 定义PID结构体
typedef struct
{
    float kp; // 比例系数
    float ki; // 积分系数
    float kd; // 微分系数
    float sp; // 设定值
    float err; // 误差
    float co; // 控制量
    float last_err; // 上一次误差
    float last2_err; // 上上一次误差

}PID_Tpyedef;
// 初始化PID
void PID_Init(PID_Tpyedef *PIDx,float kp,float ki,float kd);
// 限制控制量
void PID_Limmit(float *co,float Upper,float Lower);

// 改变设定值
void PID_ChangeSP(PID_Tpyedef *PIDx,float sp);
// 计算控制量
float PID_Compute(PID_Tpyedef *PIDx,float fb);
// 重置PID
void PID_Reset(PID_Tpyedef *PIDx);

#endif