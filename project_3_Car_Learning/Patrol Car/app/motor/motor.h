#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "ti_msp_dl_config.h"
#include "pid.h"
// ����PID�ṹ�����
PID_Tpyedef pid_l_high_speed;
PID_Tpyedef pid_r_high_speed;
PID_Tpyedef pid_l_low_speed;
PID_Tpyedef pid_r_low_speed;
PID_Tpyedef pid_direction;
PID_Tpyedef pid_tracking;

// ������ռ�ձȱ���
float duty_l;
float duty_r;
float duty_diff;
// ������ٱ�־����
int tracking_flag;
// ��ʼ���������
void Motor_Init();
// ����������
void Motor_Proc();
// �����ҵ���ٶȺ���
void Motor_SetSpeed_R(float speed);
// ���������ٶȺ���
void Motor_SetSpeed_L(float speed);
// ���õ��������
void Motor_direction(float direction);
#endif