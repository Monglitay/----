#include "delay.h"
volatile static uint32_t systicks = 0;
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
	systicks++;
}
uint32_t get_ticks(void)
{
	return systicks;
}