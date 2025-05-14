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
    __disable_irq(); // �����жϱ��⾺̬����
    
    uint32_t ticks = SysTick->VAL;       // ��ǰ����ֵ���ݼ���
    uint32_t ticks_ms = systicks;         // ��ǰ������
    uint32_t reload = SysTick->LOAD + 1; // ��װ��ֵ
    
    // ��������жϵ�δ����msTicks��������
    while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
        ticks = SysTick->VAL;
        ticks_ms = systicks+1;
        systicks++;
    }
    
    __enable_irq(); // �����ж�
    
    // ����΢��
    uint32_t us = (reload - ticks) / 32;
    return ticks_ms * 1000 + us;
}