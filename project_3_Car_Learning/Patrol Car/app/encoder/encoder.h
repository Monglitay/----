#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "ti_msp_dl_config.h"

// 初始化编码器
void Encoder_Init(void);
// 获取左编码器的值
float Encoder_Get_L(void);
// 获取右编码器的值
float Encoder_Get_R(void);
// 获取左编码器的速度
int32_t Encoder_Get_L_Speed(void);
// 获取右编码器的速度
int32_t Encoder_Get_R_Speed(void);

#endif