#include "buzzer.h"
#include "delay.h"

// ��¼��һ�η�����������ʱ���
int32_t last_on_tick = 0;

// ������״̬��־��1��ʾ��Ҫ������0��ʾ����Ҫ����
extern int buzzer_flag;

/**
 * @brief ��ʼ��������
 * 
 * �ú�����������״̬��־����Ϊ0����ʾ��ʼ״̬�·�����������
 */
void Buzzer_Init()
{
    buzzer_flag = 0;
}

/**
 * @brief ����������
 * 
 * ͨ������PWM�Ĳ���Ƚ�ֵ������������
 */
void Buzzer_On()
{
    // ����PWM_Buzzer_INST�Ĳ���Ƚ�ֵΪ80��GPIO_PWM_Buzzer_C0_IDXΪͨ������
    DL_TimerG_setCaptureCompareValue(PWM_Buzzer_INST, 80, GPIO_PWM_Buzzer_C0_IDX);
}

/**
 * @brief �رշ�����
 * 
 * ͨ������PWM�Ĳ���Ƚ�ֵΪ0���رշ�����
 */
void Buzzer_Off()
{
    DL_TimerG_setCaptureCompareValue(PWM_Buzzer_INST, 0, GPIO_PWM_Buzzer_C0_IDX);
}

/**
 * @brief ������������
 * 
 * �ú�����Ҫ�����Ե��ã����ڿ��Ʒ������Ŀ����͹ر�
 */
void Buzzer_Proc()
{
    // �����������־Ϊ1������������
    if(buzzer_flag == 1)
    {
        Buzzer_On();
        // ��¼����ʱ��
        last_on_tick = get_ticks();
        // ���ñ�־
        buzzer_flag = 0;
    }
    else
    {
        // ��������ϴο����Ѿ�����1000��ʱ�䵥λ����رշ�����
        if(get_ticks() - last_on_tick > 1000)
            Buzzer_Off();
    }
}
