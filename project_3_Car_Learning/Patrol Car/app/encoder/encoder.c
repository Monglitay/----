#include "encoder.h"
#include "delay.h"

#define DEBOUNCE_TIME 200
static volatile int64_t encoder_l = 0; // 左电机编码器的值
static volatile int64_t encoder_r = 0; // 右电机编码器的值
static volatile int8_t direction_l = 1; // 左电机旋转的方向，1 - 正转，-1 - 反转
static volatile int8_t direction_r = 1; // 右电机旋转的方向，1 - 正转，-1 - 反转
static volatile uint64_t t0_l = 0, t1_l = 0; // 左电机编码器发生变化的时间，单位us
static volatile uint64_t t0_r = 0, t1_r = 0; // 右电机编码器发生变化的时间，单位us

void Encoder_Init(void)
{
    NVIC_EnableIRQ(ENCODER_GPIOA_INT_IIDX);
    NVIC_EnableIRQ(ENCODER_GPIOB_INT_IIDX);
}
void GROUP1_IRQHandler(void)//Group1的中断服务函数
{
    //读取Group1的中断寄存器并清除中断标志位
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
                
                if(direction_r < 0) // 之前轮胎是反转
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
		
                if(direction_r > 0) // 之前轮胎是正转
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
                
                if(direction_l > 0) // 之前轮胎是正转
                {
                    direction_l = -2;
                }
                else
                {
                    direction_l = -1;
                }
            }
            else // 现在轮胎是正转
            {
                encoder_l++;
                
                if(direction_l < 0) // 之前轮胎是反转，现在轮胎是正转
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
// @简介：获取左编码器的角度
// @单位：度
float Encoder_Get_L(void)
{
    return encoder_l / 22.0f / (30613.0f / 1500.0f) * 360.0f;
}
//
// @简介：获取右轮胎的角度
// @单位：度
float Encoder_Get_R(void)
{
    return encoder_r / 22.0f / (30613.0f / 1500.0f) * 360.0f;
}





float Encoder_Get_R_Speed(void)
{
	__disable_irq(); // 关闭单片机的总中断
	
	int8_t direction_cpy = direction_r;
	uint64_t t0_cpy = t0_r;
	uint64_t t1_cpy = t1_r;
	
	__enable_irq(); // 开启单片机的总中断
	
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
    	__disable_irq(); // 关闭单片机的总中断
	
	int8_t direction_cpy = direction_l;
	uint64_t t0_cpy = t0_l;
	uint64_t t1_cpy = t1_l;
	
	__enable_irq(); // 开启单片机的总中断
	
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