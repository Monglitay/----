#ifndef __PID_H_
#define __PID_H_
#include "ti_msp_dl_config.h"  
#include "pid.h"
// ����PID�ṹ��
typedef struct
{
    float kp; // ����ϵ��
    float ki; // ����ϵ��
    float kd; // ΢��ϵ��
    float sp; // �趨ֵ
    float err; // ���
    float co; // ������
    float last_err; // ��һ�����
    float last2_err; // ����һ�����

}PID_Tpyedef;
// ��ʼ��PID
void PID_Init(PID_Tpyedef *PIDx,float kp,float ki,float kd);
// ���ƿ�����
void PID_Limmit(float *co,float Upper,float Lower);

// �ı��趨ֵ
void PID_ChangeSP(PID_Tpyedef *PIDx,float sp);
// ���������
float PID_Compute(PID_Tpyedef *PIDx,float fb);
// ����PID
void PID_Reset(PID_Tpyedef *PIDx);

#endif