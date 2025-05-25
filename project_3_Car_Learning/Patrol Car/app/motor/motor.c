#include "motor.h"
#include "task.h"
#include "encoder.h"
#include "mpu6050.h"
#include "tb6612.h"
#include "tracking.h"

// �궨�壺�ж��ٶ��Ƿ��ڵ��ٷ�Χ�ڣ�-10��10��
#define IF_SPEED_LOW(speed) ((speed) >= -10 && (speed) <= 10)
// �궨�壺�ж��ٶ��Ƿ�ӽ��㣨-1��1��
#define IF_SPEED_ZERO(speed) ((speed) >= -1 && (speed) <= 1)

// ��̬���������ҵ����Ŀ���ٶȺ�ʵ���ٶ�
static float target_speed_l;
static float target_speed_r;
static int32_t omeg_l;
static int32_t omeg_r;

/**
 * @brief ��ʼ���������ϵͳ
 */
void Motor_Init()
{
    // ��ʼ����PID������
    PID_Init(&pid_l_high_speed, 3.41, 0.01, 0.1);
    PID_Init(&pid_r_high_speed, 3.41, 0.01, 0.1);
    PID_Init(&pid_l_low_speed, 1.1, 0, 0.1);
    PID_Init(&pid_r_low_speed, 1.1, 0, 0.1);
    PID_Init(&pid_direction, 0.34, 0, 0.02);
    PID_Init(&pid_tracking, -0.9, 0, 0);
    
    Encoder_Init();    // ��ʼ��������
    MPU6050_Init();    // ��ʼ��MPU6050
    Enable_TB6612();   // ʹ��TB6612�������
    
    tracking_flag = 0; // ��ʼ�����ٱ�־
    target_speed_l = 0;
    target_speed_r = 0;
    omeg_l = 0;
    omeg_r = 0;
}

/**
 * @brief �������ƺ���
 * @param target_speed Ŀ���ٶ�
 */
void Motor_Control_L(float target_speed)
{
    if(IF_SPEED_ZERO(target_speed))
    {
        PID_Reset(&pid_l_low_speed);
        PID_Reset(&pid_l_high_speed);
        SetPWM_L(0);
        return;
    }
    if(IF_SPEED_LOW(target_speed))
    {
        omeg_l = Encoder_Get_L_Speed();
        PID_ChangeSP(&pid_l_low_speed, target_speed);
        duty_l = PID_Compute(&pid_l_low_speed, omeg_l);
        PID_Limmit(&duty_l, 100.0, -100.0);
        SetPWM_L(duty_l);
    }
    else
    {
        omeg_l = Encoder_Get_L_Speed();
        PID_ChangeSP(&pid_l_high_speed, target_speed);
        duty_l = PID_Compute(&pid_l_high_speed, omeg_l);
        PID_Limmit(&duty_l, 100.0, -100.0);
        SetPWM_L(duty_l);
    }
}

/**
 * @brief �ҵ�����ƺ���
 * @param target_speed Ŀ���ٶ�
 */
void Motor_Control_R(float target_speed)
{
    if(IF_SPEED_ZERO(target_speed))
    {
        PID_Reset(&pid_r_low_speed);
        PID_Reset(&pid_r_high_speed);
        SetPWM_R(0);
        return;
    }
    if(IF_SPEED_LOW(target_speed))
    {
        omeg_r = Encoder_Get_R_Speed();
        PID_ChangeSP(&pid_r_low_speed, target_speed_r);
        duty_r = PID_Compute(&pid_r_low_speed, omeg_r);
        PID_Limmit(&duty_r, 100.0, -100.0);
        SetPWM_R(duty_r);
    }
    else
    {
        omeg_r = Encoder_Get_R_Speed();
        PID_ChangeSP(&pid_r_high_speed, target_speed_r);
        duty_r = PID_Compute(&pid_r_high_speed, omeg_r);
        PID_Limmit(&duty_r, 100.0, -100.0);
        SetPWM_R(duty_r);
    }
}

/**
 * @brief ������ƴ������������Ե���
 */
void Motor_Proc()
{
    PERIODIC(1);
    if(tracking_flag == 1)
    {
        duty_diff = PID_Compute(&pid_tracking, Tracking_Angle());
    }
    else 
    {
        duty_diff = PID_Compute(&pid_direction, Get_yaw());
    }
    
    float speed_l = target_speed_l - duty_diff;
    float speed_r = target_speed_r + duty_diff;

    Motor_Control_L(speed_l);
    Motor_Control_R(speed_r);
}

/**
 * @brief �����ҵ��Ŀ���ٶ�
 * @param speed Ŀ���ٶ�
 */
void Motor_SetSpeed_R(float speed)
{
    target_speed_r = speed;
}

/**
 * @brief ��������Ŀ���ٶ�
 * @param speed Ŀ���ٶ�
 */
void Motor_SetSpeed_L(float speed)
{
    target_speed_l = speed;
}

/**
 * @brief ���÷������PID��Ŀ��ֵ
 * @param direction Ŀ�귽��
 */
void Motor_direction(float direction)
{
    pid_direction.sp = direction;
}
