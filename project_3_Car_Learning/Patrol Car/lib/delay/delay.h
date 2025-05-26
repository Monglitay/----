#ifndef _DELAY_H_
#define _DELAY_H_

#include "ti_msp_dl_config.h"

// 延时函数，延时sx毫秒
void delay_ms(unsigned int sx);
// 延时函数，延时sx微秒
void delay_us(unsigned int sx);
// 获取当前时间戳，单位为毫秒
uint32_t get_ticks(void);
// 获取当前时间戳，单位为微秒
uint64_t get_ticks_us(void);

#endif






























