#ifndef __BUZZER_H_
#define __BUZZER_H_

#include "ti_msp_dl_config.h"
void Buzzer_Init();
void Buzzer_On();
void Buzzer_Off();
void Buzzer_Proc();
int buzzer_flag;

#endif