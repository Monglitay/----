#include "delay.h"

// 系统滴答计数器，用于记录从系统启动开始经过的毫秒数
volatile static uint64_t systicks = 0;

/**
 * @brief 毫秒级延时函数
 * @param sx 延时的毫秒数
 * 
 * 该函数使用延时循环实现毫秒级延时
 */
void delay_ms(unsigned int sx)
{
    sx = sx * 32000;  // 将毫秒转换为延时循环次数
    delay_cycles(sx); // 调用延时循环函数
}

/**
 * @brief 微秒级延时函数
 * @param sx 延时的微秒数
 * 
 * 该函数使用延时循环实现微秒级延时
 */
void delay_us(unsigned int sx)
{
    sx = sx * 32;     // 将微秒转换为延时循环次数
    delay_cycles(sx); // 调用延时循环函数
}

/**
 * @brief SysTick中断处理函数
 * 
 * 该函数在每个SysTick中断触发时被调用，用于更新系统滴答计数器
 */
void SysTick_Handler(void)
{
    uint32_t val = SysTick->VAL; // 读取当前SysTick计数值（未使用）

    systicks++; // 增加系统滴答计数器
}

/**
 * @brief 获取系统滴答计数器的值
 * @return 返回系统启动以来经过的毫秒数
 */
uint32_t get_ticks(void)
{
    return systicks;
}

/**
 * @brief 获取高精度的系统运行时间（微秒级）
 * @return 返回系统启动以来经过的微秒数
 */
uint64_t get_ticks_us(void)
{
    __disable_irq(); // 禁用中断避免竞态条件
    
    uint32_t ticks = SysTick->VAL;       // 当前SysTick计数值（递减）
    uint32_t ticks_ms = systicks;        // 当前毫秒数
    uint32_t reload = SysTick->LOAD + 1; // SysTick重装载值
    
    // 如果发生SysTick中断但未更新systicks，需要进行修正
    while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
        ticks = SysTick->VAL;
        ticks_ms = systicks + 1;
        systicks++;
    }
    
    __enable_irq(); // 重新启用中断
    
    // 计算微秒部分
    uint32_t us = (reload - ticks) / 32; // 假设系统时钟为32MHz
    return ticks_ms * 1000 + us; // 返回总微秒数
}
