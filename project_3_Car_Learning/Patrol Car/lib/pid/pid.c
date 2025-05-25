#include "pid.h"
#include "delay.h"
#include "math.h"

// 定义误差死区，小于此值的误差将被视为零
#define ERROR_DEADZONE 2

/**
 * @brief 初始化PID控制器
 * @param PIDx PID控制器结构体指针
 * @param kp 比例系数
 * @param ki 积分系数
 * @param kd 微分系数
 */
void PID_Init(PID_Tpyedef *PIDx, float kp, float ki, float kd)
{
    PIDx->kp = kp;
    PIDx->ki = ki;
    PIDx->kd = kd;
    PIDx->sp = 0.0f;     // 设定值初始化为0
    PIDx->err = 0;       // 当前误差
    PIDx->last_err = 0;  // 上一次误差
    PIDx->last2_err = 0; // 上上次误差
    PIDx->co = 0;        // 控制输出初始化为0
}

/**
 * @brief 限制PID输出值在指定范围内
 * @param co 控制输出指针
 * @param Upper 上限
 * @param Lower 下限
 */
void PID_Limmit(float *co, float Upper, float Lower)
{
    if (*co > Upper) *co = Upper;
    if (*co < Lower) *co = Lower;
}

/**
 * @brief 改变PID控制器的设定值
 * @param PIDx PID控制器结构体指针
 * @param sp 新的设定值
 */
void PID_ChangeSP(PID_Tpyedef *PIDx, float sp)
{
    PIDx->sp = sp;
}

/**
 * @brief 计算PID控制输出
 * @param PIDx PID控制器结构体指针
 * @param fb 反馈值
 * @return 返回计算后的控制输出
 */
float PID_Compute(PID_Tpyedef *PIDx, float fb)
{
    PIDx->err = PIDx->sp - fb;  // 计算当前误差
    
    // 增量式PID算法
    PIDx->co += PIDx->kp * (PIDx->err - PIDx->last_err) +
                PIDx->ki * PIDx->err +
                PIDx->kd * (PIDx->err - 2 * PIDx->last_err + PIDx->last2_err);
    
    PIDx->last2_err = PIDx->last_err;  // 更新上上次误差
    PIDx->last_err = PIDx->err;        // 更新上次误差
    
    // 误差死区处理
    if (fabs(PIDx->err) < ERROR_DEADZONE) 
    {
        PIDx->err = 0;
    }

    return PIDx->co;
}

/**
 * @brief 重置PID控制器
 * @param PIDx PID控制器结构体指针
 */
void PID_Reset(PID_Tpyedef *PIDx)
{
    PIDx->err = 0;
    PIDx->last2_err = 0;
    PIDx->last_err = 0;
}
