#ifndef _DELAY_H_
#define _DELAY_H_

#include "ti_msp_dl_config.h"

// ��ʱ��������ʱsx����
void delay_ms(unsigned int sx);
// ��ʱ��������ʱsx΢��
void delay_us(unsigned int sx);
// ��ȡ��ǰʱ�������λΪ����
uint32_t get_ticks(void);
// ��ȡ��ǰʱ�������λΪ΢��
uint64_t get_ticks_us(void);

#endif






























