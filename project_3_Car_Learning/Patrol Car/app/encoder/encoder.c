#include "encoder.h"
#include "tb6612.h"

// 编码器相关全局变量
volatile uint32_t Encoder_L_Port, Encoder_R_Port;      // 左右编码器端口状态
volatile int32_t Encoder_L_CNT, Encoder_R_CNT;         // 左右编码器计数值
volatile int32_t Encoder_L_VEL, Encoder_R_VEL;         // 左右马达速度
volatile int32_t Encoder_L_INT, Encoder_R_INT;          // 左右编码器积分    

/**
 * @brief 初始化编码器
 * 
 * 该函数关闭所有电机输出，并使能编码器相关的中断
 */
void Encoder_Init(void)
{
    /* 关闭所有电机输出 */
    AIN1_OUT(0);
    AIN2_OUT(0);
    BIN1_OUT(0);
    BIN2_OUT(0);
    
    /* 使能编码器相关中断 */
    NVIC_EnableIRQ(GPIO_Encoder_R_INT_IRQN);
    NVIC_EnableIRQ(GPIO_Encoder_L_INT_IRQN);
    NVIC_EnableIRQ(TIMER_Encoder_Read_INST_INT_IRQN);
    
    /* 启动编码器读取定时器 */
    DL_Timer_startCounter(TIMER_Encoder_Read_INST);
    Encoder_L_INT = 0;
    Encoder_R_INT = 0; 
}

/**
 * @brief 编码器读取中断服务函数
 * 
 * 该函数在定时器中断触发时调用，用于读取和重置编码器计数值
 */
void TIMER_Encoder_Read_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_Encoder_Read_INST)){
        case DL_TIMER_IIDX_ZERO:
            // 更新速度值并重置计数器
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
 * @brief 编码器GPIO中断处理函数
 * 
 * 该函数处理编码器A和B的GPIO中断，更新编码器计数值
 */
void GROUP1_IRQHandler(void)
{
    // 获取左右编码器的中断状态
    Encoder_L_Port = DL_GPIO_getEnabledInterruptStatus(GPIO_Encoder_L_PORT, GPIO_Encoder_L_L_A_PIN | GPIO_Encoder_L_L_B_PIN);
    Encoder_R_Port = DL_GPIO_getEnabledInterruptStatus(GPIO_Encoder_R_PORT, GPIO_Encoder_R_R_A_PIN | GPIO_Encoder_R_R_B_PIN);
    
    /* 处理左编码器 */
    if((Encoder_L_Port & GPIO_Encoder_L_L_A_PIN) == GPIO_Encoder_L_L_A_PIN)
    {
        // 根据B相状态判断旋转方向
        if(!DL_GPIO_readPins(GPIO_Encoder_L_PORT,GPIO_Encoder_L_L_B_PIN))   {Encoder_L_CNT--;Encoder_L_INT--;}
        else                                                                {Encoder_L_CNT++;Encoder_L_INT++;}
    }
    else if((Encoder_L_Port & GPIO_Encoder_L_L_B_PIN) == GPIO_Encoder_L_L_B_PIN)
    {
        // 根据A相状态判断旋转方向
        if(!DL_GPIO_readPins(GPIO_Encoder_L_PORT,GPIO_Encoder_L_L_A_PIN))   {Encoder_L_CNT++;Encoder_L_INT++;}
        else                                                                {Encoder_L_CNT--;Encoder_L_INT--;}
    }
    // 清除左编码器中断标志
    DL_GPIO_clearInterruptStatus(GPIO_Encoder_L_PORT, GPIO_Encoder_L_L_A_PIN|GPIO_Encoder_L_L_B_PIN);

    /* 处理右编码器 */
    if((Encoder_R_Port & GPIO_Encoder_R_R_A_PIN) == GPIO_Encoder_R_R_A_PIN)
    {
        // 根据B相状态判断旋转方向
        if(!DL_GPIO_readPins(GPIO_Encoder_R_PORT,GPIO_Encoder_R_R_B_PIN))    {Encoder_R_CNT--;Encoder_R_INT--;}
        else                                                                {Encoder_R_CNT++;Encoder_R_INT++;}
    }
    else if((Encoder_R_Port & GPIO_Encoder_R_R_B_PIN) == GPIO_Encoder_R_R_B_PIN)
    {
        // 根据A相状态判断旋转方向
        if(!DL_GPIO_readPins(GPIO_Encoder_R_PORT,GPIO_Encoder_R_R_A_PIN))   {Encoder_R_CNT++;Encoder_R_INT++;}
        else                                                                {Encoder_R_CNT--;Encoder_R_INT--;}
    }
    // 清除右编码器中断标志
    DL_GPIO_clearInterruptStatus(GPIO_Encoder_R_PORT, GPIO_Encoder_R_R_A_PIN|GPIO_Encoder_R_R_B_PIN);
}

/**
 * @brief 获取左轮速度
 * @return 左轮速度值
 */
int32_t Encoder_Get_L_Speed()
{
    return -Encoder_L_VEL;
}

/**
 * @brief 获取右轮速度
 * @return 右轮速度值（取反）
 */
int32_t Encoder_Get_R_Speed()
{
    return -Encoder_R_VEL;
}

int32_t Encoder_Get_L_Integral()
{
    return Encoder_L_INT;
}
int32_t Encoder_Get_R_Integral()
{
    return -Encoder_R_INT;
}