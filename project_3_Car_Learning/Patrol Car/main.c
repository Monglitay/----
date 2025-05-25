#include "ti_msp_dl_config.h"
#include "encoder.h"
#include "hc_06.h"
#include "delay.h"
#include "tb6612.h"
#include "motor.h"
#include "task.h"
#include "mpu6050.h"
#include "tracking.h"
#include "buzzer.h" 

// С�������ٶ�
static float speed = 12;
// ״̬��״̬����
int state = 0;
// ������
int cnt = 0;
// ��һ�η����¼��Ԥ����
float last_direction = 0;

// ��������
void Task_1();
void Task_2();

/**
 * @brief ���������
 * @details 
 * 1. ��ʼ��ϵͳ����
 * 2. ��ʼ����ģ�飺�����������ѭ������������������
 * 3. ������ѭ����ִ��ѭ������
 */
int main(void)
{
    // ϵͳ�����ó�ʼ��
    SYSCFG_DL_init();
    
    // ����ģ���ʼ��
    HC06_Init();     // ����ģ���ʼ��
    Motor_Init();    // ������Ƴ�ʼ��
    Tracking_Init(); // ѭ��ģ���ʼ��
    Encoder_Init();  // ��������ʼ��
    Buzzer_Init();   // ��������ʼ��
    
    // ������ѭ����ִ������
    while (1) 
    {   
        Task_2(); // ִ����ѭ������
    }
}

/**
 * @brief ����1��Ԥ��/���ã�
 * @details 
 * ״̬��ʵ�֣�������
 * 1. �豸��ʼ״̬���
 * 2. �����߲��л�״̬
 * 3. �������͵������
 */
void Task_1()
{
    // �����Դ���MPU6050��������ƺͷ�����
    MPU6050_Proc();
    Motor_Proc();
    Buzzer_Proc();
    
    // ״̬��״̬0����ʼ״̬
    if(state == 0)
    {
        buzzer_flag = 1;          // ��������ʾ
        tracking_flag = 0;         // �ر�ѭ��
        Motor_SetSpeed_R(speed);   // �����ҵ���ٶ�
        Motor_SetSpeed_L(speed);   // ���������ٶ�
        Motor_direction(0);        // ���õ������
        
        // ����Ƿ��������
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > 30)             
            {
                state = 1;           
                cnt = 0;
                tracking_flag = 0;
                return;
            }
        }
    }
    // ״̬1��������
    else if(state == 1)
    {
        Motor_direction(0);
        if(IS_INBLACK())
        {
            cnt++;
            if(cnt > 30)             // ��⵽���߳���
            {
                buzzer_flag = 1;
                state = 2;            // ����״̬��
                cnt = 0;
                return;
            }
        }
        else cnt = 0;
    }
}

/**
 * @brief ������ִ�к���
 * @details 
 * ���ӵ�״̬��ʵ�֣��������״̬��
 * 0. ��ʼ״̬����ͨ��ʻ
 * 1. ���߼��״̬
 * 2. ����ѭ��ģʽ
 * 3. �˳�ѭ��ģʽ
 * 4. �ٴ�ѭ��
 * 5. ֹͣ
 */
void Task_2()
{
    // �����Դ���������ִ����
    MPU6050_Proc();
    Motor_Proc();
    Buzzer_Proc();
    
    // ״̬0����ʼ��ʻ״̬
    if(state == 0)
    {
        buzzer_flag = 1;          // ��������ʾ
        tracking_flag = 0;         // �ر�ѭ��
        Motor_SetSpeed_R(speed);   // �����ҵ���ٶ�
        Motor_SetSpeed_L(speed);   // ���������ٶ�
        Motor_direction(0);        // ���õ������
        
        // ����Ƿ��������
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > 20)             
            {
                state = 1;           
                cnt = 0;
                tracking_flag = 0;
                return;
            }
        }
    }
    // ״̬1��������
    else if(state == 1)
    {
        Motor_direction(0);
        if(IS_INBLACK())
        {
            cnt++;
            if(cnt > 20)             
            {
                buzzer_flag = 1;
                state = 2;               
                cnt = 0;
                return;
            }
        }
        else cnt = 0;
    }
    // ״̬2������ѭ��ģʽ
    else if (state == 2)
    {
        tracking_flag = 1;
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > 20)             
            {   
                buzzer_flag = 1;
                state = 3;           
                tracking_flag = 0;
                MPU6050_Init();
                return;
            }
        } else cnt = 0;
    }
    // ״̬3���˳�ѭ��
    else if (state == 3)
    {
        tracking_flag = 0;
        Motor_direction(0);
        if(IS_INBLACK())
        {
            cnt++;
            if(cnt > 20)             
            {
                buzzer_flag = 1;
                state = 4;           
                cnt = 0;
                return;
            }
        } else cnt = 0;
    }
    // ״̬4���ٴ�ѭ��
    else if (state == 4)
    {
        tracking_flag = 1;
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > 20)             
            {
                buzzer_flag = 1;
                state = 5;           
                cnt = 0;
                tracking_flag = 0;
                Disable_TB6612();
                return;
            }
        } else cnt = 0;
    }
}
