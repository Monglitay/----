#ifndef __TB6612_H_
#define __TB6612_H_

#include "ti_msp_dl_config.h"


// ����AIN1�����������
#define AIN1_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_AIN1_PORT ,TB6612_AIN1_PIN)) : (DL_GPIO_clearPins(TB6612_AIN1_PORT,TB6612_AIN1_PIN)) )
// ����AIN2�����������
#define AIN2_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN)) : (DL_GPIO_clearPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN)) )
// ����BIN1�����������
#define BIN1_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_BIN1_PORT ,TB6612_BIN1_PIN)) : (DL_GPIO_clearPins(TB6612_BIN1_PORT,TB6612_BIN1_PIN)) )
// ����BIN2�����������
#define BIN2_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN)) : (DL_GPIO_clearPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN)) )
// ��������PWMռ�ձȺ���
void SetPWM_R(float duty);
// ��������PWMռ�ձȺ���
void SetPWM_L(float duty);
// ʹ��TB6612����������
void Enable_TB6612();
// ����TB6612����������
void Disable_TB6612();

#endif