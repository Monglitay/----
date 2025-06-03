#ifndef __BUZZER_H
#define __BUZZER_H

#include <stdint.h>

// 蜂鸣器控制结构体
typedef struct {
    uint8_t  is_active;      // 是否处于活动状态
    uint32_t start_tick;     // 开始时间
    uint32_t duration;       // 持续时间
    uint32_t interval;       // 间隔时间
    uint8_t  repeat_count;   // 重复次数
    uint8_t  current_count;  // 当前重复计数
} BuzzerControl;

// 蜂鸣器初始化
void Buzzer_Init(void);

// 蜂鸣器开启
void Buzzer_On(void);

// 蜂鸣器关闭
void Buzzer_Off(void);

/**
 * @brief 设置蜂鸣器鸣叫模式
 * @param duration 鸣叫时长(ms)
 * @param interval 间隔时长(ms)
 * @param repeat_count 重复次数
 */
void Buzzer_SetPattern(uint32_t duration, uint32_t interval, uint8_t repeat_count);

// 蜂鸣器处理函数，需要在主循环或定时器中周期性调用
void Buzzer_Proc(void);

// 快速鸣叫模式定义
#define BUZZER_SHORT_BEEP     Buzzer_SetPattern(100, 50, 1)    // 短促蜂鸣
#define BUZZER_LONG_BEEP      Buzzer_SetPattern(500, 0, 1)     // 长鸣

#endif // __BUZZER_H
