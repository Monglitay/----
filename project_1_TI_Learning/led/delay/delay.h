#ifndef _DELAY_H_
#define _DELAY_H_


#include "ti_msp_dl_config.h"

static int ticks = 0;
void delay_ms(unsigned int sx);
void delay_us(unsigned int sx);
int get_ticks(void);


#endif


















