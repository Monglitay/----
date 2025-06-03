#include "control.h"

float speed = 12;
int state = 0;
float tracking_speed = 10;
int cnt=0;
int turning_flag = 0;
int last_turned_ticks = 0;
int cycles=0;

#define AB_WHITE_CNT  1000
#define AB_BLACK_CNT  1000

#define ABCD_WHITE_CNT 5000
#define ABCD_BLACK_CNT 1500

#define ACBD_WHITE_CNT 6000
#define ACBD_BLACK_CNT 1000

void My_Key_Init();

void Modules_Init(void)
{
    HC06_Init();     // 蓝牙模块初始化
    Motor_Init();    // 电机控制初始化
    Tracking_Init(); // 循迹模块初始化
    Encoder_Init();  // 编码器初始化
    Buzzer_Init();   // 蜂鸣器初始化
    JY62_Init();     // 陀螺仪初始化
    My_Key_Init();   // 按键初始化
    NVIC_EnableIRQ(TIMER_Proc_INST_INT_IRQN);
    
    /* 启动编码器读取定时器 */
    DL_Timer_startCounter(TIMER_Proc_INST);
    mode = 0;
    delay_ms(1000);
}
/**
 * @brief 巡线小车直线路径运动控制函数（AB)
 * @details 通过状态机实现小车在A点处直行到B点
 * 状态机工作流程：
 * - state 0: 从A运动到B,走0度直线
 * - state 1: 检测到达B点
 */
void Control_AB(void)
{
    if(state == 0)
    {
                // 蜂鸣器提示
        tracking_flag = 0;         // 关闭循迹
        Motor_SetSpeed_R(speed);   // 设置右电机速度
        Motor_SetSpeed_L(speed);   // 设置左电机速度
        Motor_direction(0);        // 设置电机方向
        // 检测是否脱离黑线
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > AB_WHITE_CNT)             
            {
                state = 1;           
                cnt = 0;
                tracking_flag = 0;
                BUZZER_LONG_BEEP; 
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
            if(cnt > AB_BLACK_CNT)             // 检测到黑线持续
            {
                BUZZER_LONG_BEEP;
                state = 2;            // 进入状态二
                cnt = 0;
                Motor_SetSpeed_R(0);   // 设置右电机速度
                Motor_SetSpeed_L(0);   // 设置左电机速度
                return;
            }
        }
        else cnt = 0;
    }
}

/**
 * @brief 巡线小车椭圆路径运动控制函数（ABCDA)
 * @details 通过状态机实现小车在两个半圆上连续运动1圈
 * 状态机工作流程：
 * - state 0: 从A运动到B,走0度直线
 * - state 1: 检测到达B点，并回正
 * - state 2：从B巡线到C
 * - state 3: 从C运动到D，走225度直线
 * - state 4: 从D运动到A，在结尾处回正
 */
void Control_ABCD(void)
{
       // 状态0：初始行驶状态
    if(state == 0)
    {
        tracking_flag = 0;         // 关闭循迹
        Motor_SetSpeed_R(speed);   // 设置右电机速度
        Motor_SetSpeed_L(speed);   // 设置左电机速度
        Motor_direction(0);        // 设置电机方向直行
        
        // 检测是否脱离黑线
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > ABCD_WHITE_CNT)             
            {
                state = 1;           
                cnt = 0;
                tracking_flag = 0;
                return;
            }
        }
        else cnt = 0;
    }
    // 状态1：检测到B点并回正
    else if(state == 1)
    {
        Motor_direction(0);
        if(IS_INBLACK())
        {
            cnt++;
            if(cnt > ABCD_BLACK_CNT)             
            {
                    
                    BUZZER_LONG_BEEP;   // 进入状态2
                    state = 2;  
                    tracking_flag = 1;
                    Motor_SetSpeed_L(tracking_speed);
                    Motor_SetSpeed_R(tracking_speed);            
                    cnt = 0;
                    return;
            }
        }else cnt = 0;
    }
        
    // 状态2：进入循迹模式（从B到C）
    else if (state == 2)
    {
        tracking_flag = 1;
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > ABCD_WHITE_CNT)             
            {   
                BUZZER_LONG_BEEP;                
                state = 3;           
                tracking_flag = 0;
                Motor_SetSpeed_R(speed);
                Motor_SetSpeed_L(speed);
                cnt = 0;
                return;
            }
        } 
        else cnt = 0;
    }
    // 状态3：从C运动到D
    else if (state == 3)
    {
        Motor_direction(177);
        if(IS_INBLACK())
        {
            cnt++;
            if(cnt > ABCD_BLACK_CNT)             
            {
                BUZZER_LONG_BEEP;                
                state = 4;
                tracking_flag = 1;
                Motor_SetSpeed_L(tracking_speed);
                Motor_SetSpeed_R(tracking_speed);           
                cnt = 0;
                return;
            }
        } 
        else cnt = 0;
    }
    // 状态4：从D运动到A
    else if (state == 4)
    {
        
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > ABCD_WHITE_CNT)             
            {
                BUZZER_LONG_BEEP;                
                state = 0;           
                cnt = 0;
                tracking_flag = 0;
                return;
            }
        } 
        else cnt = 0;
    }
}

/**
 * @brief 巡线小车8字路径运动控制函数(ACBDA)
 * @details 通过状态机实现小车在两个半圆上连续运动1圈
 * 状态机工作流程：
 * - state 0: 从A运动到C,走-45度直线
 * - state 1: 检测到达C点，并回正
 * - state 2：从C巡线到B，并在B点回正
 * - state 3: 从B运动到D，走225度直线
 * - state 4: 从D运动到A，在结尾处回正
 */
void Control_ACBD(void)
{// 状态0：初始行驶状态
    if(state == 0)
    {
        if(state == 0)
        {
        tracking_flag = 0;         // 关闭循迹
        Motor_SetSpeed_R(speed);   // 设置右电机速度
        Motor_SetSpeed_L(speed);   // 设置左电机速度
        Motor_direction(0);        // 设置电机方向直行
        }
        // 检测是否脱离黑线
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt > ACBD_WHITE_CNT)             
            {
                state = 1;           
                cnt = 0;
                tracking_flag = 0;
                
                return;
            }
        }
        else cnt = 0;
    }
    // 状态1：检测到B点并回正
    else if(state == 1)
    {
        if(turning_flag == 0)
            last_turned_ticks = get_ticks();
        turning_flag = 1;
        if(get_ticks()-last_turned_ticks < 1000)
        {
            Motor_SetSpeed_R(0);
            Motor_SetSpeed_L(0);   
            Motor_direction(-40);
            
            return;
        }
        Motor_SetSpeed_R(speed);
        Motor_SetSpeed_L(speed);
        if(IS_INBLACK_L())
        {
            cnt++;
            if(cnt > ACBD_BLACK_CNT)             
            {
                turning_flag = 0;
                BUZZER_LONG_BEEP;
                state = 2;              
                cnt = 0;
                return;
            }
        }else cnt = 0;
    }
        
    // 状态2：进入循迹模式（从B到C）
    else if (state == 2)
    {
        if(turning_flag == 0)
            last_turned_ticks = get_ticks();
        turning_flag = 1;
        if(get_ticks()-last_turned_ticks < 1000)
        {
            Motor_SetSpeed_R(0);
            Motor_SetSpeed_L(0);   
            Motor_direction(0);
            
            return;
        }
        tracking_flag = 1;
        Motor_SetSpeed_L(tracking_speed);
        Motor_SetSpeed_R(tracking_speed);
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt >ACBD_WHITE_CNT)             
            {   
                BUZZER_LONG_BEEP;                
                state = 3;           
                tracking_flag = 0;
                cnt = 0;
                turning_flag = 0;
                return;
            }
        } 
        else cnt = 0;
    }
    // 状态3：从C运动到D
    else if (state == 3)
    {
        
        if(turning_flag == 0)
            last_turned_ticks = get_ticks();
        turning_flag = 1;
        if(get_ticks()-last_turned_ticks < 2000)
        {
            Motor_SetSpeed_R(0);   // 设置右电机速度
            Motor_SetSpeed_L(0);   // 设置左电机速度
            Motor_direction(-143);
            return;
        }
        Motor_SetSpeed_R(speed);   // 设置右电机速度
        Motor_SetSpeed_L(speed);   // 设置左电机速度
        
        if(IS_INBLACK_R())
        {
            cnt++;
            if(cnt > ACBD_BLACK_CNT)             
            {
                turning_flag = 0;
                BUZZER_LONG_BEEP;                
                state = 4;           
                cnt = 0;
                return;
            }
        } 
        else cnt = 0;
    }
    // 状态4：从D运动到A
    else if (state == 4)
    {
        if(turning_flag == 0)
            last_turned_ticks = get_ticks();
        turning_flag = 1;
        if(get_ticks()-last_turned_ticks < 1000)
        {
            Motor_SetSpeed_R(0);
            Motor_SetSpeed_L(0);   
            Motor_direction(-178);
            
            return;
        }
        
        tracking_flag = 1;
        Motor_SetSpeed_L(tracking_speed);
        Motor_SetSpeed_R(tracking_speed);
        if(!(IS_INBLACK()))
        {
            cnt++;
            if(cnt >ACBD_WHITE_CNT)             
            {
                BUZZER_LONG_BEEP;                
                state = 1;           
                cnt = 0;
                tracking_flag = 0;
                turning_flag = 0;
                Motor_SetSpeed_L(0);
                Motor_SetSpeed_R(0);
                cycles++;
                return;
            }
        } 
        else cnt = 0;
    }
}
void key_pressed_cb(void);
void key_long_pressed_cb(uint8_t ticks);

void My_Key_Init()
{
    Key_InitTypedef Key_Initstruct;
    Key_Initstruct.key_pin = Key_k1_PIN;
    Key_Initstruct.key_port = Key_PORT;
    Key_Initstruct.key_pressed_cb = key_pressed_cb;
    Key_Initstruct.key_long_pressed_cb = key_long_pressed_cb;

    Key_Init(&Key1, &Key_Initstruct);
}

void key_pressed_cb(void)
{
    mode++;
}
void key_long_pressed_cb(uint8_t ticks)
{
    mode = 0;
}

void TIMG7_IRQHandler(void)
{
    Motor_Proc();
    Buzzer_Proc();
}