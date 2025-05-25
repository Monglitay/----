#include "encoder.h"
#include "tb6612.h"

// ���������ȫ�ֱ���
volatile uint32_t Encoder_L_Port, Encoder_R_Port;      // ���ұ������˿�״̬
volatile int32_t Encoder_L_CNT, Encoder_R_CNT;         // ���ұ���������ֵ
volatile int32_t Encoder_L_VEL, Encoder_R_VEL;         // ��������ٶ� 

/**
 * @brief ��ʼ��������
 * 
 * �ú����ر����е���������ʹ�ܱ�������ص��ж�
 */
void Encoder_Init(void)
{
    /* �ر����е����� */
    AIN1_OUT(0);
    AIN2_OUT(0);
    BIN1_OUT(0);
    BIN2_OUT(0);
    
    /* ʹ�ܱ���������ж� */
    NVIC_EnableIRQ(GPIO_Encoder_R_INT_IRQN);
    NVIC_EnableIRQ(GPIO_Encoder_L_INT_IRQN);
    NVIC_EnableIRQ(TIMER_Encoder_Read_INST_INT_IRQN);
    
    /* ������������ȡ��ʱ�� */
    DL_Timer_startCounter(TIMER_Encoder_Read_INST);
}

/**
 * @brief ��������ȡ�жϷ�����
 * 
 * �ú����ڶ�ʱ���жϴ���ʱ���ã����ڶ�ȡ�����ñ���������ֵ
 */
void TIMER_Encoder_Read_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_Encoder_Read_INST)){
        case DL_TIMER_IIDX_ZERO:
            // �����ٶ�ֵ�����ü�����
            Encoder_L_VEL = Encoder_L_CNT;
            Encoder_L_CNT = 0;
            Encoder_R_VEL = Encoder_R_CNT;
            Encoder_R_CNT = 0;
            break;
        default:
            break;
    }
}

/**
 * @brief ������GPIO�жϴ�����
 * 
 * �ú������������A��B��GPIO�жϣ����±���������ֵ
 */
void GROUP1_IRQHandler(void)
{
    // ��ȡ���ұ��������ж�״̬
    Encoder_L_Port = DL_GPIO_getEnabledInterruptStatus(GPIO_Encoder_L_PORT, GPIO_Encoder_L_L_A_PIN | GPIO_Encoder_L_L_B_PIN);
    Encoder_R_Port = DL_GPIO_getEnabledInterruptStatus(GPIO_Encoder_R_PORT, GPIO_Encoder_R_R_A_PIN | GPIO_Encoder_R_R_B_PIN);
    
    /* ����������� */
    if((Encoder_L_Port & GPIO_Encoder_L_L_A_PIN) == GPIO_Encoder_L_L_A_PIN)
    {
        // ����B��״̬�ж���ת����
        if(!DL_GPIO_readPins(GPIO_Encoder_L_PORT,GPIO_Encoder_L_L_B_PIN))   Encoder_L_CNT--;
        else                                                                Encoder_L_CNT++;
    }
    else if((Encoder_L_Port & GPIO_Encoder_L_L_B_PIN) == GPIO_Encoder_L_L_B_PIN)
    {
        // ����A��״̬�ж���ת����
        if(!DL_GPIO_readPins(GPIO_Encoder_L_PORT,GPIO_Encoder_L_L_A_PIN))   Encoder_L_CNT++;
        else                                                                Encoder_L_CNT--;
    }
    // �����������жϱ�־
    DL_GPIO_clearInterruptStatus(GPIO_Encoder_L_PORT, GPIO_Encoder_L_L_A_PIN|GPIO_Encoder_L_L_B_PIN);

    /* �����ұ����� */
    if((Encoder_R_Port & GPIO_Encoder_R_R_A_PIN) == GPIO_Encoder_R_R_A_PIN)
    {
        // ����B��״̬�ж���ת����
        if(!DL_GPIO_readPins(GPIO_Encoder_R_PORT,GPIO_Encoder_R_R_B_PIN))   Encoder_R_CNT--;
        else                                                                Encoder_R_CNT++;
    }
    else if((Encoder_R_Port & GPIO_Encoder_R_R_B_PIN) == GPIO_Encoder_R_R_B_PIN)
    {
        // ����A��״̬�ж���ת����
        if(!DL_GPIO_readPins(GPIO_Encoder_R_PORT,GPIO_Encoder_R_R_A_PIN))   Encoder_R_CNT++;
        else                                                                Encoder_R_CNT--;
    }
    // ����ұ������жϱ�־
    DL_GPIO_clearInterruptStatus(GPIO_Encoder_R_PORT, GPIO_Encoder_R_R_A_PIN|GPIO_Encoder_R_R_B_PIN);
}

/**
 * @brief ��ȡ�����ٶ�
 * @return �����ٶ�ֵ
 */
int32_t Encoder_Get_L_Speed()
{
    return Encoder_L_VEL;
}

/**
 * @brief ��ȡ�����ٶ�
 * @return �����ٶ�ֵ��ȡ����
 */
int32_t Encoder_Get_R_Speed()
{
    return -Encoder_R_VEL;
}
