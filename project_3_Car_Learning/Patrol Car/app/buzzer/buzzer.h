#ifndef __BUZZER_H_
#define __BUZZER_H_

#include "ti_msp_dl_config.h"
// 初始化蜂鸣器
void Buzzer_Init();
// 打开蜂鸣器
void Buzzer_On();
// 关闭蜂鸣器
void Buzzer_Off();
// 处理蜂鸣器
void Buzzer_Proc();
// 蜂鸣器标志位
int buzzer_flag;

#endif