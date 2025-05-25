#include "delay.h"

// ϵͳ�δ�����������ڼ�¼��ϵͳ������ʼ�����ĺ�����
volatile static uint64_t systicks = 0;

/**
 * @brief ���뼶��ʱ����
 * @param sx ��ʱ�ĺ�����
 * 
 * �ú���ʹ����ʱѭ��ʵ�ֺ��뼶��ʱ
 */
void delay_ms(unsigned int sx)
{
    sx = sx * 32000;  // ������ת��Ϊ��ʱѭ������
    delay_cycles(sx); // ������ʱѭ������
}

/**
 * @brief ΢�뼶��ʱ����
 * @param sx ��ʱ��΢����
 * 
 * �ú���ʹ����ʱѭ��ʵ��΢�뼶��ʱ
 */
void delay_us(unsigned int sx)
{
    sx = sx * 32;     // ��΢��ת��Ϊ��ʱѭ������
    delay_cycles(sx); // ������ʱѭ������
}

/**
 * @brief SysTick�жϴ�����
 * 
 * �ú�����ÿ��SysTick�жϴ���ʱ�����ã����ڸ���ϵͳ�δ������
 */
void SysTick_Handler(void)
{
    uint32_t val = SysTick->VAL; // ��ȡ��ǰSysTick����ֵ��δʹ�ã�

    systicks++; // ����ϵͳ�δ������
}

/**
 * @brief ��ȡϵͳ�δ��������ֵ
 * @return ����ϵͳ�������������ĺ�����
 */
uint32_t get_ticks(void)
{
    return systicks;
}

/**
 * @brief ��ȡ�߾��ȵ�ϵͳ����ʱ�䣨΢�뼶��
 * @return ����ϵͳ��������������΢����
 */
uint64_t get_ticks_us(void)
{
    __disable_irq(); // �����жϱ��⾺̬����
    
    uint32_t ticks = SysTick->VAL;       // ��ǰSysTick����ֵ���ݼ���
    uint32_t ticks_ms = systicks;        // ��ǰ������
    uint32_t reload = SysTick->LOAD + 1; // SysTick��װ��ֵ
    
    // �������SysTick�жϵ�δ����systicks����Ҫ��������
    while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
        ticks = SysTick->VAL;
        ticks_ms = systicks + 1;
        systicks++;
    }
    
    __enable_irq(); // ���������ж�
    
    // ����΢�벿��
    uint32_t us = (reload - ticks) / 32; // ����ϵͳʱ��Ϊ32MHz
    return ticks_ms * 1000 + us; // ������΢����
}
