#ifndef __TB6612_H_
#define __TB6612_H_

#include "ti_msp_dl_config.h"


#define AIN1_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_AIN1_PORT ,TB6612_AIN1_PIN)) : (DL_GPIO_clearPins(TB6612_AIN1_PORT,TB6612_AIN1_PIN)) )
#define AIN2_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN)) : (DL_GPIO_clearPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN)) )
#define BIN1_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_BIN1_PORT ,TB6612_BIN1_PIN)) : (DL_GPIO_clearPins(TB6612_BIN1_PORT,TB6612_BIN1_PIN)) )
#define BIN2_OUT(X)  ( (X) ? (DL_GPIO_setPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN)) : (DL_GPIO_clearPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN)) )
void SetSpeed_R(int dir, uint32_t speed);
void SetSpeed_L(int dir, uint32_t speed);

#endif  /* _BSP_TB6612_H */