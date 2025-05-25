#include "ti_msp_dl_config.h"
#include "encoder.h"
#include "hc_06.h"
#include "delay.h"
#include "tb6612.h"
#include  "motor.h"
#include "task.h"
#include "mpu6050.h"
#include "tracking.h"
#include "buzzer.h" 
static float speed = 12;
int state = 0;
int cnt = 0;
float last_direction = 0;
void Task_1();
void Task_2();
int main(void)
{
    SYSCFG_DL_init();
    HC06_Init();
    Motor_Init();
    Tracking_Init();
    Encoder_Init();
    Buzzer_Init();
    while (1) 
    {   
        Task_2();
    }
    
}
void Task_1()
{
    MPU6050_Proc();
    Motor_Proc();
    Buzzer_Proc();
    
    if(state == 0)
    {
        buzzer_flag = 1;
        tracking_flag = 0;
        Motor_SetSpeed_R(speed);
        Motor_SetSpeed_L(speed);
        Motor_direction(0);
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
    else if(state == 1)
    {
        Motor_direction(0);
        if(IS_INBLACK())
        {
            cnt++;
            if(cnt > 30)             //检测到黑线持续5次
            {
                buzzer_flag = 1;
                state = 2;               //进入状态二
                cnt = 0;
                return;
            }
            
        }else   cnt = 0;
    }
    
}

void Task_2()
{
        MPU6050_Proc();
        Motor_Proc();
        Buzzer_Proc();
        
        if(state == 0)
        {
            buzzer_flag = 1;
            tracking_flag = 0;
            Motor_SetSpeed_R(speed);
            Motor_SetSpeed_L(speed);
            Motor_direction(0);
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
        else if(state == 1)
        {
            Motor_direction(0);
            if(IS_INBLACK())
            {
                cnt++;
                if(cnt > 20)             //检测到黑线持续5次
                {
                    buzzer_flag = 1;
                    state = 2;               //进入状态二
                    cnt = 0;
                    return;
                }
                
            }else   cnt = 0;
        }
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
            } else   cnt = 0;

        }
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
            } else   cnt = 0;
        }
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
            } else   cnt = 0;
        }
        
        
}