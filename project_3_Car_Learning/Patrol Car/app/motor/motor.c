#include "motor.h"
#include "task.h"
#include "encoder.h"
#include "jy62.h"
#include "tb6612.h"
#include "tracking.h"

// 宏定义：判断速度是否接近零（-1到1）
#define IF_SPEED_ZERO(speed) ((speed) >= -1 && (speed) <= 1)

// 静态变量：左右电机的目标速度和实际速度
static float target_speed_l;
static float target_speed_r;
static int32_t omeg_l;
static int32_t omeg_r;
static float duty_l;
static float duty_r;
uint8_t tracking_flag;
static float duty_diff;
PID_Typedef pid_l_speed;
PID_Typedef pid_r_speed;
PID_Typedef pid_direction;
PID_Typedef pid_tracking;

/**
 * @brief 初始化电机控制系统
 */
void Motor_Init()
{
    // 初始化各PID控制器
    PID_Init(&pid_l_speed, 4.11, 0.01, 0.11);
    PID_Init(&pid_r_speed, 4.11, 0.01, 0.11);
    PID_Init(&pid_direction, 0.4, 0, 0.01);
    PID_Init(&pid_tracking, 0.45, 0, 0);
    
    Encoder_Init();    // 初始化编码器
    JY62_Init();    // 初始化JY62
    Enable_TB6612();   // 使能TB6612电机驱动
    
    tracking_flag = 0; // 初始化跟踪标志
    target_speed_l = 0;
    target_speed_r = 0;
    omeg_l = 0;
    omeg_r = 0;
}

/**
 * @brief 左电机控制函数
 * @param target_speed 目标速度
 */
void Motor_Control_L(float target_speed)
{
    if(IF_SPEED_ZERO(target_speed))
    {
        PID_Reset(&pid_l_speed);
        SetPWM_L(0);
        return;
    }
        omeg_l = Encoder_Get_L_Speed();
        PID_ChangeSP(&pid_l_speed, target_speed);
        duty_l = PID_Compute(&pid_l_speed, omeg_l);
        PID_Limmit(&duty_l, 100.0, -100.0);
        SetPWM_L(duty_l);
}

/**
 * @brief 右电机控制函数
 * @param target_speed 目标速度
 */
void Motor_Control_R(float target_speed)
{
    if(IF_SPEED_ZERO(target_speed))
    {
        PID_Reset(&pid_r_speed);
        SetPWM_R(0);
        return;
    }
        omeg_r = Encoder_Get_R_Speed();
        PID_ChangeSP(&pid_r_speed, target_speed);
        duty_r = PID_Compute(&pid_r_speed, omeg_r);
        PID_Limmit(&duty_r, 100.0, -100.0);
        SetPWM_R(duty_r);
}

/**
 * @brief 电机控制处理函数，周期性调用
 */
void Motor_Proc()
{
    float speed_l = target_speed_l;
    float speed_r = target_speed_r;
    if(tracking_flag == 1)
    {
        duty_diff = PID_Compute(&pid_tracking, Tracking_Angle());
        speed_l -= duty_diff;
        speed_r += duty_diff;
    }
    else 
    {
        duty_diff = PID_Compute_dir(&pid_direction, JY62_Get_Yaw());
        speed_l -= duty_diff;
        speed_r += duty_diff;
    }
    
    Motor_Control_L(speed_l);
    Motor_Control_R(speed_r);
}

/**
 * @brief 设置右电机目标速度
 * @param speed 目标速度
 */
void Motor_SetSpeed_R(float speed)
{
    target_speed_r = speed;
}

/**
 * @brief 设置左电机目标速度
 * @param speed 目标速度
 */
void Motor_SetSpeed_L(float speed)
{
    target_speed_l = speed;
}

/**
 * @brief 设置方向控制PID的目标值
 * @param direction 目标方向
 */
void Motor_direction(float direction)
{
    pid_direction.sp = direction;
}

