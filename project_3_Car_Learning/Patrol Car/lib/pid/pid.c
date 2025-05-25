#include "pid.h"
#include "delay.h"
#include "math.h"

// �������������С�ڴ�ֵ��������Ϊ��
#define ERROR_DEADZONE 2

/**
 * @brief ��ʼ��PID������
 * @param PIDx PID�������ṹ��ָ��
 * @param kp ����ϵ��
 * @param ki ����ϵ��
 * @param kd ΢��ϵ��
 */
void PID_Init(PID_Tpyedef *PIDx, float kp, float ki, float kd)
{
    PIDx->kp = kp;
    PIDx->ki = ki;
    PIDx->kd = kd;
    PIDx->sp = 0.0f;     // �趨ֵ��ʼ��Ϊ0
    PIDx->err = 0;       // ��ǰ���
    PIDx->last_err = 0;  // ��һ�����
    PIDx->last2_err = 0; // ���ϴ����
    PIDx->co = 0;        // ���������ʼ��Ϊ0
}

/**
 * @brief ����PID���ֵ��ָ����Χ��
 * @param co �������ָ��
 * @param Upper ����
 * @param Lower ����
 */
void PID_Limmit(float *co, float Upper, float Lower)
{
    if (*co > Upper) *co = Upper;
    if (*co < Lower) *co = Lower;
}

/**
 * @brief �ı�PID���������趨ֵ
 * @param PIDx PID�������ṹ��ָ��
 * @param sp �µ��趨ֵ
 */
void PID_ChangeSP(PID_Tpyedef *PIDx, float sp)
{
    PIDx->sp = sp;
}

/**
 * @brief ����PID�������
 * @param PIDx PID�������ṹ��ָ��
 * @param fb ����ֵ
 * @return ���ؼ����Ŀ������
 */
float PID_Compute(PID_Tpyedef *PIDx, float fb)
{
    PIDx->err = PIDx->sp - fb;  // ���㵱ǰ���
    
    // ����ʽPID�㷨
    PIDx->co += PIDx->kp * (PIDx->err - PIDx->last_err) +
                PIDx->ki * PIDx->err +
                PIDx->kd * (PIDx->err - 2 * PIDx->last_err + PIDx->last2_err);
    
    PIDx->last2_err = PIDx->last_err;  // �������ϴ����
    PIDx->last_err = PIDx->err;        // �����ϴ����
    
    // �����������
    if (fabs(PIDx->err) < ERROR_DEADZONE) 
    {
        PIDx->err = 0;
    }

    return PIDx->co;
}

/**
 * @brief ����PID������
 * @param PIDx PID�������ṹ��ָ��
 */
void PID_Reset(PID_Tpyedef *PIDx)
{
    PIDx->err = 0;
    PIDx->last2_err = 0;
    PIDx->last_err = 0;
}
