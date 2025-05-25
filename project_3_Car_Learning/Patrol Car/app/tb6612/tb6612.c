#include "tb6612.h"

/**
 * @brief �����ҵ��PWM
 * @param duty PWMռ�ձȣ���Χ-100��100
 * 
 * �ú�������duty���������õ�����򣬲�������Ӧ��PWMֵ
 */
void SetPWM_R(float duty)
{
    if (duty > 0)
    {
        // ������ת
        AIN1_OUT(0);
        AIN2_OUT(1);
    }
    else if (duty < 0)
    {
        // ������ת
        duty = -duty;
        AIN1_OUT(1);
        AIN2_OUT(0);
    }
    else
    {
        // ֹͣ
        AIN1_OUT(0);
        AIN2_OUT(0);
    }
    
    // ��ռ�ձ�ת��Ϊ��ʱ���ıȽ�ֵ
    uint32_t ccr = duty / 100.0f * 1000;
    // ����PWMֵ
    DL_TimerG_setCaptureCompareValue(PWM_R_INST, ccr, GPIO_PWM_R_C0_IDX);
}

/**
 * @brief ��������PWM
 * @param duty PWMռ�ձȣ���Χ-100��100
 * 
 * �ú�������duty���������õ�����򣬲�������Ӧ��PWMֵ
 */
void SetPWM_L(float duty)
{
    if (duty > 0)
    {
        // ������ת
        BIN1_OUT(1);
        BIN2_OUT(0);
    }
    else if (duty < 0)
    {
        // ������ת
        duty = -duty;
        BIN1_OUT(0);
        BIN2_OUT(1);
    }
    else
    {
        // ֹͣ
        BIN1_OUT(0);
        BIN2_OUT(0);
    }
    
    // ��ռ�ձ�ת��Ϊ��ʱ���ıȽ�ֵ
    uint32_t ccr = duty / 100.0f * 1000;
    // ����PWMֵ
    DL_TimerG_setCaptureCompareValue(PWM_L_INST, ccr, GPIO_PWM_L_C1_IDX);
}

/**
 * @brief ʹ��TB6612���������
 * 
 * ����STBY����Ϊ�ߵ�ƽ��ʹTB6612���빤��״̬
 */
void Enable_TB6612()
{
    DL_GPIO_setPins(TB6612_STBY_PORT, TB6612_STBY_PIN);
}

/**
 * @brief ����TB6612���������
 * 
 * ����STBY����Ϊ�͵�ƽ��ʹTB6612�������״̬
 */
void Disable_TB6612()
{
    DL_GPIO_clearPins(TB6612_STBY_PORT, TB6612_STBY_PIN);
}
