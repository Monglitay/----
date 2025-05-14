#include "delay.h"
volatile static uint64_t systicks = 0;
void delay_ms(unsigned int sx)
{
		sx=sx*32000;
		delay_cycles(sx);
}

void delay_us(unsigned int sx)
{
		sx=sx*32;
		delay_cycles(sx);
}

void SysTick_Handler(void)
{
	uint32_t val = SysTick->VAL;

	systicks++;
}
uint32_t get_ticks(void)
{
	return systicks;
}
uint64_t get_ticks_us(void)
{
    __disable_irq(); // 禁用中断避免竞态条件
    
    uint32_t ticks = SysTick->VAL;       // 当前计数值（递减）
    uint32_t ticks_ms = systicks;         // 当前毫秒数
    uint32_t reload = SysTick->LOAD + 1; // 重装载值
    
    // 如果发生中断但未更新msTicks，需修正
    while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
        ticks = SysTick->VAL;
        ticks_ms = systicks+1;
        systicks++;
    }
    
    __enable_irq(); // 启用中断
    
    // 计算微秒
    uint32_t us = (reload - ticks) / 32;
    return ticks_ms * 1000 + us;
}