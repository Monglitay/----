#ifndef __BUZZER_H_
#define __BUZZER_H_

#include "ti_msp_dl_config.h"
// ��ʼ��������
void Buzzer_Init();
// �򿪷�����
void Buzzer_On();
// �رշ�����
void Buzzer_Off();
// ���������
void Buzzer_Proc();
// ��������־λ
int buzzer_flag;

#endif