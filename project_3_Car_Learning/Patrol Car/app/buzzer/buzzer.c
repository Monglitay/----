#include "buzzer.h"
#include "delay.h"

// 记录上一次蜂鸣器开启的时间戳
int32_t last_on_tick = 0;

// 蜂鸣器状态标志，1表示需要开启，0表示不需要开启
extern int buzzer_flag;

/**
 * @brief 初始化蜂鸣器
 * 
 * 该函数将蜂鸣器状态标志设置为0，表示初始状态下蜂鸣器不工作
 */
void Buzzer_Init()
{
    buzzer_flag = 0;
}

/**
 * @brief 开启蜂鸣器
 * 
 * 通过设置PWM的捕获比较值来开启蜂鸣器
 */
void Buzzer_On()
{
    DL_TimerG_setCaptureCompareValue(PWM_Buzzer_INST, 80, GPIO_PWM_Buzzer_C0_IDX);
}

/**
 * @brief 关闭蜂鸣器
 * 
 * 通过设置PWM的捕获比较值为0来关闭蜂鸣器
 */
void Buzzer_Off()
{
    DL_TimerG_setCaptureCompareValue(PWM_Buzzer_INST, 0, GPIO_PWM_Buzzer_C0_IDX);
}

/**
 * @brief 蜂鸣器处理函数
 * 
 * 该函数需要周期性调用，用于控制蜂鸣器的开启和关闭
 */
void Buzzer_Proc()
{
    // 如果蜂鸣器标志为1，则开启蜂鸣器
    if(buzzer_flag == 1)
    {
        Buzzer_On();
        // 记录开启时间
        last_on_tick = get_ticks();
        // 重置标志
        buzzer_flag = 0;
    }
    else
    {
        // 如果距离上次开启已经超过1000个时间单位，则关闭蜂鸣器
        if(get_ticks() - last_on_tick > 1000)
            Buzzer_Off();
    }
}
