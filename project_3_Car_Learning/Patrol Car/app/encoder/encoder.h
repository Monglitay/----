#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "ti_msp_dl_config.h"

void Encoder_Init(void);
float Encoder_Get_L(void);
float Encoder_Get_R(void);
int32_t Encoder_Get_L_Speed(void);
int32_t Encoder_Get_R_Speed(void);

#endif