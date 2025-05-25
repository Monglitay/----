#include "tracking.h"

// 定义宏：读取各个循迹传感器的状态
#define P1 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_1_PIN)
#define P2 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_2_PIN)
#define P3 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_3_PIN)
#define P4 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_4_PIN)
#define P5 DL_GPIO_readPins(Tracking_PORT, Tracking_HW_5_PIN)

// 循迹传感器的读数，表示偏离中心线的程度
static volatile int32_t Tracking_value;  

/**
 * @brief 初始化循迹模块
 * 
 * 启用定时器中断，用于周期性读取循迹传感器
 */
void Tracking_Init()
{
    NVIC_EnableIRQ(TIMER_Tracking_Read_INST_INT_IRQN);
    Tracking_value = 0;
}

/**
 * @brief 检测循迹传感器状态并计算偏移值
 * 
 * 根据各个传感器的状态计算车辆相对于线的位置
 */
void HW_Detect(void)
{
    Tracking_value = 0;
    if(P1) // 检测到最右端
        Tracking_value -= 6;
    if(P2)
        Tracking_value -= 4;
    if(P4)
        Tracking_value += 4;
    if(P5) // 检测到最左端
        Tracking_value += 6;
}

/**
 * @brief 获取循迹角度（实际上是偏移值）
 * @return 返回计算得到的偏移值
 */
int Tracking_Angle()
{
    return Tracking_value;
}

/**
 * @brief 定时器中断处理函数
 * 
 * 周期性调用HW_Detect函数来更新循迹状态
 */
void TIMA0_IRQHandler(void)
{
    HW_Detect();
}

/**
 * @brief 检查是否检测到黑线
 * @return 如果任何一个传感器检测到黑线，返回1；否则返回0
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
