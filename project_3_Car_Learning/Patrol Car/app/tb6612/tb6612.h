#ifndef __TB6612_H_
#define __TB6612_H_

#include "ti_msp_dl_config.h"


// 定义AIN1引脚输出函数
#define AIN1_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_AIN1_PORT ,TB6612_AIN1_PIN)) : (DL_GPIO_clearPins(TB6612_AIN1_PORT,TB6612_AIN1_PIN)) )
// 定义AIN2引脚输出函数
#define AIN2_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN)) : (DL_GPIO_clearPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN)) )
// 定义BIN1引脚输出函数
#define BIN1_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_BIN1_PORT ,TB6612_BIN1_PIN)) : (DL_GPIO_clearPins(TB6612_BIN1_PORT,TB6612_BIN1_PIN)) )
// 定义BIN2引脚输出函数
#define BIN2_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN)) : (DL_GPIO_clearPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN)) )
// 设置右轮PWM占空比函数
void SetPWM_R(float duty);
// 设置左轮PWM占空比函数
void SetPWM_L(float duty);
// 使能TB6612驱动器函数
void Enable_TB6612();
// 禁用TB6612驱动器函数
void Disable_TB6612();

#endif