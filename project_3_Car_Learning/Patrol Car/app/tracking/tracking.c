#include "tracking.h"

// ����꣺��ȡ����ѭ����������״̬
#define P1 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_1_PIN)
#define P2 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_2_PIN)
#define P3 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_3_PIN)
#define P4 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_4_PIN)
#define P5 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_5_PIN)

// ѭ���������Ķ�������ʾƫ�������ߵĳ̶�
static volatile int32_t Tracking_value;  

/**
 * @brief ��ʼ��ѭ��ģ��
 * 
 * ���ö�ʱ���жϣ����������Զ�ȡѭ��������
 */
void Tracking_Init()
{
    NVIC_EnableIRQ(TIMER_Tracking_Read_INST_INT_IRQN);
    Tracking_value = 0;
}

/**
 * @brief ���ѭ��������״̬������ƫ��ֵ
 * 
 * ���ݸ�����������״̬���㳵��������ߵ�λ��
 */
void HW_Detect(void)
{
    Tracking_value = 0;
    if(P1) // ��⵽���Ҷ�
        Tracking_value -= 6;
    if(P2)
        Tracking_value -= 4;
    if(P4)
        Tracking_value += 4;
    if(P5) // ��⵽�����
        Tracking_value += 6;
}

/**
 * @brief ��ȡѭ���Ƕȣ�ʵ������ƫ��ֵ��
 * @return ���ؼ���õ���ƫ��ֵ
 */
int Tracking_Angle()
{
    return Tracking_value;
}

/**
 * @brief ��ʱ���жϴ�����
 * 
 * �����Ե���HW_Detect����������ѭ��״̬
 */
void TIMA0_IRQHandler(void)
{
    HW_Detect();
}

/**
 * @brief ����Ƿ��⵽����
 * @return ����κ�һ����������⵽���ߣ�����1�����򷵻�0
 */
int IS_INBLACK()
{
    int p1 = DL_GPIO_readPins(Tracking_PORT, Tracking_HW_1_PIN);
    int p2 = DL_GPIO_readPins(Tracking_PORT, Tracking_HW_2_PIN);
    int p3 = DL_GPIO_readPins(Tracking_PORT, Tracking_HW_3_PIN);
    int p4 = DL_GPIO_readPins(Tracking_PORT, Tracking_HW_4_PIN);
    int p5 = DL_GPIO_readPins(Tracking_PORT, Tracking_HW_5_PIN);
    return p1 || p2 || p3 || p4 || p5;
}
