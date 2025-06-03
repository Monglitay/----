#include "tb6612.h"

/**
 * @brief 设置右电机PWM
 * @param duty PWM占空比，范围-100到100
 * 
 * 该函数根据duty的正负设置电机方向，并设置相应的PWM值
 */
void SetPWM_R(float duty)
{
    if (duty > 0)
    {
        // 正向旋转
        AIN1_OUT(0);
        AIN2_OUT(1);
    }
    else if (duty < 0)
    {
        // 反向旋转
        duty = -duty;
        AIN1_OUT(1);
        AIN2_OUT(0);
    }
    else
    {
        // 停止
        AIN1_OUT(0);
        AIN2_OUT(0);
    }
    
    // 将占空比转换为定时器的比较值
    uint32_t ccr = duty / 100.0f * 1000;
    // 设置PWM值
    DL_TimerG_setCaptureCompareValue(PWM_R_INST, ccr, GPIO_PWM_R_C1_IDX);
}

/**
 * @brief 设置左电机PWM
 * @param duty PWM占空比，范围-100到100
 * 
 * 该函数根据duty的正负设置电机方向，并设置相应的PWM值
 */
void SetPWM_L(float duty)
{
    if (duty > 0)
    {
        // 正向旋转
        BIN1_OUT(1);
        BIN2_OUT(0);
    }
    else if (duty < 0)
    {
        // 反向旋转
        duty = -duty;
        BIN1_OUT(0);
        BIN2_OUT(1);
    }
    else
    {
        // 停止
        BIN1_OUT(0);
        BIN2_OUT(0);
    }
    
    // 将占空比转换为定时器的比较值
    uint32_t ccr = duty / 100.0f * 1000;
    // 设置PWM值
    DL_TimerG_setCaptureCompareValue(PWM_L_INST, ccr, GPIO_PWM_L_C0_IDX);
}

/**
 * @brief 使能TB6612电机驱动器
 * 
 * 设置STBY引脚为高电平，使TB6612进入工作状态
 */
void Enable_TB6612()
{
    DL_GPIO_setPins(TB6612_STBY_PORT, TB6612_STBY_PIN);
}

/**
 * @brief 禁用TB6612电机驱动器
 * 
 * 设置STBY引脚为低电平，使TB6612进入待机状态
 */
void Disable_TB6612()
{
    DL_GPIO_clearPins(TB6612_STBY_PORT, TB6612_STBY_PIN);
}
