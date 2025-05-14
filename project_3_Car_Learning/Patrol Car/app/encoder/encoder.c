#include "encoder.h"
#include "delay.h"

#define DEBOUNCE_TIME 200
static volatile int64_t encoder_l = 0; // ������������ֵ
static volatile int64_t encoder_r = 0; // �ҵ����������ֵ
static volatile int8_t direction_l = 1; // ������ת�ķ���1 - ��ת��-1 - ��ת
static volatile int8_t direction_r = 1; // �ҵ����ת�ķ���1 - ��ת��-1 - ��ת
static volatile uint64_t t0_l = 0, t1_l = 0; // ���������������仯��ʱ�䣬��λus
static volatile uint64_t t0_r = 0, t1_r = 0; // �ҵ�������������仯��ʱ�䣬��λus

void Encoder_Init(void)
{
    NVIC_EnableIRQ(ENCODER_GPIOA_INT_IIDX);
    NVIC_EnableIRQ(ENCODER_GPIOB_INT_IIDX);
}
void GROUP1_IRQHandler(void)//Group1���жϷ�����
{
    //��ȡGroup1���жϼĴ���������жϱ�־λ
    if( DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1) == ENCODER_GPIOA_INT_IIDX | ENCODER_GPIOB_INT_IIDX )
    {

        if(DL_GPIO_getEnabledInterruptStatus(ENCODER_R_B_PORT, ENCODER_R_B_PIN))
        {
            int a = DL_GPIO_readPins(ENCODER_R_A_PORT,ENCODER_R_A_PIN);
            int b = DL_GPIO_readPins(ENCODER_R_B_PORT,ENCODER_R_B_PIN);
            t1_r = t0_r;
	        t0_r = get_ticks_us();
            if ((a >0 && b >0) || (a ==0 && b ==0))
            {
                encoder_r++;
                
                if(direction_r < 0) // ֮ǰ��̥�Ƿ�ת
                {
                    direction_r = +2;
                }
                else
                {
                    direction_r = 1;
                }
	        }
            else
            {
                encoder_r--;
		
                if(direction_r > 0) // ֮ǰ��̥����ת
                {
                    direction_r = -2;
                }
                else
                {
                    direction_r = -1;
                }
            }
            DL_GPIO_clearInterruptStatus(ENCODER_R_B_PORT, ENCODER_R_B_PIN);
        }
        if(DL_GPIO_getEnabledInterruptStatus(ENCODER_L_B_PORT, ENCODER_L_B_PIN))
        {
            int a = DL_GPIO_readPins(ENCODER_L_A_PORT,ENCODER_L_A_PIN);
            int b = DL_GPIO_readPins(ENCODER_L_B_PORT,ENCODER_L_B_PIN);
            t1_l = t0_l;
	        t0_l = get_ticks_us();
            if ((a >0 && b >0) || (a ==0 && b ==0))
            {
                encoder_l--;
                
                if(direction_l > 0) // ֮ǰ��̥����ת
                {
                    direction_l = -2;
                }
                else
                {
                    direction_l = -1;
                }
            }
            else // ������̥����ת
            {
                encoder_l++;
                
                if(direction_l < 0) // ֮ǰ��̥�Ƿ�ת��������̥����ת
                {
                    direction_l = +2;
                }
                else
                {
                    direction_l = 1;
                }
            }
            DL_GPIO_clearInterruptStatus(ENCODER_L_B_PORT, ENCODER_L_B_PIN);
        }
        
    }
}
//
// @��飺��ȡ��������ĽǶ�
// @��λ����
float Encoder_Get_L(void)
{
    return encoder_l / 22.0f / (30613.0f / 1500.0f) * 360.0f;
}
//
// @��飺��ȡ����̥�ĽǶ�
// @��λ����
float Encoder_Get_R(void)
{
    return encoder_r / 22.0f / (30613.0f / 1500.0f) * 360.0f;
}





float Encoder_Get_R_Speed(void)
{
	__disable_irq(); // �رյ�Ƭ�������ж�
	
	int8_t direction_cpy = direction_r;
	uint64_t t0_cpy = t0_r;
	uint64_t t1_cpy = t1_r;
	
	__enable_irq(); // ������Ƭ�������ж�
	
	if(direction_cpy == +2 || direction_cpy == -2) return 0.0f;
	
	uint64_t now = get_ticks_us();
	
	float T;
	
	if(t0_cpy - t1_cpy > now - t0_cpy)
	{
		T = (t0_cpy - t1_cpy) * 1.0e-6f;
	}
	else
	{
		T = (now - t0_cpy) * 1.0e-6f;
	}


	return direction_cpy / T / 22.0f / (30613.0f / 1500.0f) * 360.0f;
}

float Encoder_Get_L_Speed(void)
{
    	__disable_irq(); // �رյ�Ƭ�������ж�
	
	int8_t direction_cpy = direction_l;
	uint64_t t0_cpy = t0_l;
	uint64_t t1_cpy = t1_l;
	
	__enable_irq(); // ������Ƭ�������ж�
	
	if(direction_cpy == +2 || direction_cpy == -2) return 0.0f;
	
	uint64_t now = get_ticks_us();
	
	float T;
	
	if(t0_cpy - t1_cpy > now - t0_cpy)
	{
		T = (t0_cpy - t1_cpy) * 1.0e-6f;
	}
	else
	{
		T = (now - t0_cpy) * 1.0e-6f;
	}
	
	return direction_cpy / T / 22.0f / (30613.0f / 1500.0f) * 360.0f;
}