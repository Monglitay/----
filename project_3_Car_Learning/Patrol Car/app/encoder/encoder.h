#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "ti_msp_dl_config.h"

// ��ʼ��������
void Encoder_Init(void);
// ��ȡ���������ֵ
float Encoder_Get_L(void);
// ��ȡ�ұ�������ֵ
float Encoder_Get_R(void);
// ��ȡ����������ٶ�
int32_t Encoder_Get_L_Speed(void);
// ��ȡ�ұ��������ٶ�
int32_t Encoder_Get_R_Speed(void);

#endif