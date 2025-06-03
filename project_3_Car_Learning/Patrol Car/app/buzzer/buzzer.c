#include "buzzer.h"
#include "delay.h"
#include "string.h"

static BuzzerControl buzzer = {0};

/**
 * @brief 初始化蜂鸣器
 */
void Buzzer_Init()
{
    Buzzer_Off();
    memset(&buzzer, 0, sizeof(BuzzerControl));
}

/**
 * @brief 开启蜂鸣器
 */
void Buzzer_On()
{
    DL_TimerG_setCaptureCompareValue(PWM_Buzzer_INST, 80, GPIO_PWM_Buzzer_C0_IDX);
}

/**
 * @brief 关闭蜂鸣器
 */
void Buzzer_Off()
{
    DL_TimerG_setCaptureCompareValue(PWM_Buzzer_INST, 0, GPIO_PWM_Buzzer_C0_IDX);
}

/**
 * @brief 设置蜂鸣器鸣叫模式
 * @param duration 鸣叫时长(ms)
 * @param interval 间隔时长(ms)
 * @param repeat_count 重复次数
 */
void Buzzer_SetPattern(uint32_t duration, uint32_t interval, uint8_t repeat_count)
{
    buzzer.duration = duration;
    buzzer.interval = interval;
    buzzer.repeat_count = repeat_count;
    buzzer.is_active = 1;
    buzzer.current_count = 0;
    buzzer.start_tick = get_ticks();
}
/**
 * @brief 蜂鸣器处理函数
 */
void Buzzer_Proc()
{
    uint32_t current_tick = get_ticks();
    // 如果未激活，直接返回
    if (!buzzer.is_active) {
        Buzzer_Off();
        return;
    }
    // 计算经过的时间
    uint32_t elapsed = current_tick - buzzer.start_tick;
    // 完成所有重复
    if (buzzer.current_count >= buzzer.repeat_count) {
        buzzer.is_active = 0;
        Buzzer_Off();
        return;
    }
    // 判断当前是鸣叫还是静音状态
    if (elapsed % (buzzer.duration + buzzer.interval) < buzzer.duration) {
        Buzzer_On();
    } else {
        Buzzer_Off();
        
        // 如果一个周期结束，增加重复计数
        if (elapsed % (buzzer.duration + buzzer.interval) == 0) {
            buzzer.current_count++;
        }
    }
}
