#ifndef __KEY_H_
#define __KEY_H_

#include "ti_msp_dl_config.h"


typedef struct 
{
    GPIO_Regs *key_port;
    uint32_t  key_iidx;
    int key_irqn;
    void (*key_cb)(void);
    
}Key_Initstruct;

static Key_Initstruct keys[5];
static int key_counts = 0;
void Key_Init(Key_Initstruct *key);

#endif // __KEY_H_