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

// 小车运行速度
static float speed = 12;
// 状态机状态变量
int state = 0;
// 计数器
int cnt = 0;
// 上一次方向记录（预留）
float last_direction = 0;

// 任务声明
void Task_1();
void Task_2();

/**
 * @brief 主函数入口
 * @details 
 * 1. 初始化系统配置
 * 2. 初始化各模块：蓝牙、电机、循迹、编码器、蜂鸣器
 * 3. 进入主循环，执行循迹任务
 */
int main(void)
{
    // 系统级配置初始化
    SYSCFG_DL_init();
    
    // 外设模块初始化
    HC06_Init();     // 蓝牙模块初始化
    Motor_Init();    // 电机控制初始化
    Tracking_Init(); // 循迹模块初始化
    Encoder_Init();  // 编码器初始化
    Buzzer_Init();   // 蜂鸣器初始化
    
    // 进入主循环，执行任务
    while (1) 
    {   
        Task_2(); // 执行主循环任务
    }
}

/**
 * @brief 任务1（预留/备用）
 * @details 
 * 状态机实现，包含：
 * 1. 设备初始状态检测
 * 2. 检测黑线并切换状态
 * 3. 蜂鸣器和电机联动
 */
void Task_1()
{
    // 周期性处理MPU6050、电机控制和蜂鸣器
    MPU6050_Proc();
    Motor_Proc();
    Buzzer_Proc();
    
    // 状态机状态0：初始状态
    if(state == 0)
    {
        buzzer_flag = 1;          // 蜂鸣器提示
        tracking_flag = 0;         // 关闭循迹
        Motor_SetSpeed_R(speed);   // 设置右电机速度
        Motor_SetSpeed_L(speed);   // 设置左电机速度
        Motor_direction(0);        // 设置电机方向
        
        // 检测是否脱离黑线
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
    // 状态1：检测黑线
    else if(state == 1)
    {
        Motor_direction(0);
        if(IS_INBLACK())
        {
            cnt++;
            if(cnt > 30)             // 检测到黑线持续
            {
                buzzer_flag = 1;
                state = 2;            // 进入状态二
                cnt = 0;
                return;
            }
        }
        else cnt = 0;
    }
}

/**
 * @brief 主任务执行函数
 * @details 
 * 复杂的状态机实现，包含多个状态：
 * 0. 初始状态：普通行驶
 * 1. 黑线检测状态
 * 2. 进入循迹模式
 * 3. 退出循迹模式
 * 4. 再次循迹
 * 5. 停止
 */
void Task_2()
{
    // 周期性处理传感器和执行器
    MPU6050_Proc();
    Motor_Proc();
    Buzzer_Proc();
    
    // 状态0：初始行驶状态
    if(state == 0)
    {
        buzzer_flag = 1;          // 蜂鸣器提示
        tracking_flag = 0;         // 关闭循迹
        Motor_SetSpeed_R(speed);   // 设置右电机速度
        Motor_SetSpeed_L(speed);   // 设置左电机速度
        Motor_direction(0);        // 设置电机方向
        
        // 检测是否脱离黑线
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
    // 状态1：检测黑线
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
    // 状态2：进入循迹模式
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
    // 状态3：退出循迹
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
    // 状态4：再次循迹
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
