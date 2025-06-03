#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "ti_msp_dl_config.h"
#include "ti_msp_dl_config.h"
#include "encoder.h"
#include "hc_06.h"
#include "delay.h"
#include "tb6612.h"
#include "motor.h"
#include "task.h"
#include "jy62.h"
#include "tracking.h"
#include "buzzer.h" 
#include "key.h"

Key_TypeDef Key1;
int mode;
void Control_AB(void);
void Control_ABCD(void);
void Control_ACBD(void);

void Modules_Init(void);

#endif