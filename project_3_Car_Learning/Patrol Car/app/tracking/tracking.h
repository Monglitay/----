#ifndef __Tracking_  // 如果没有定义__Tracking_宏
#define __Tracking_  // 定义__Tracking_宏



#include "ti_msp_dl_config.h"  // 包含ti_msp_dl_config.h头文件



int Tracking_Angle();  // 声明Tracking_Angle函数
void Tracking_Init();  // 声明Tracking_Init函数
int IS_INBLACK ();  // 声明IS_INBLACK函数
int IS_INBLACK_R();
int IS_INBLACK_L();
#endif  // 结束宏定义