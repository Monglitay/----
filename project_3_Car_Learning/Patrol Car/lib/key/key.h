#ifndef __KEY_H_
#define __KEY_H_

#include "ti_msp_dl_config.h"

// 定义按键初始化结构体
typedef struct 
{
    GPIO_Regs *key_port; // 按键端口
    unsigned int key_pin; // 按键引脚
    void (*key_pressed_cb)(void); // 按键按下回调函数
    void (*key_released_cb)(void); // 按键松开回调函数
    void (*key_clicked_cb)(uint8_t clicks); // 按键连击回调函数
    void (*key_long_pressed_cb)(uint8_t ticks); // 按键长按回调函数
    
    uint32_t LongPressThreshold;     // 长按阈值
    uint32_t LongPressTickInterval;  // 长按触发间隔
    uint32_t ClickInterval;          // 连击间隔
}Key_InitTypedef;

// 原有的 Key_TypeDef 结构体保持不变
typedef struct 
{
    /* 初始化参数 */
    GPIO_Regs *key_port;
    unsigned int key_pin;
    uint32_t  key_iidx;
    int key_irqn;
    uint32_t LongPressTime; // 按钮长按时间阈值，单位毫秒
    uint32_t LongPressTickInterval; // 长按后持续触发的间隔
    uint32_t ClickInterval; // 连击时间间隔，单位毫秒
    void (*key_pressed_cb)(void);
    void (*key_released_cb)(void);
    void (*key_clicked_cb)(uint8_t clicks);
    void (*key_long_pressed_cb)(uint8_t ticks);
    
    uint8_t  LastState;     // 按钮上次的状态，0 - 松开，1 - 按下
    uint8_t  ChangePending; // 按钮的状态是否正在发生改变
    uint32_t PendingTime;   // 按钮状态开始变化的时间
    uint32_t LongPressThreshold;

    uint32_t LastPressedTime;  // 按钮上次按下的时间
    uint32_t LastReleasedTime; // 按钮上次松开的时间
    
    // 长按次数
    uint8_t LongPressTicks;
    // 上次长按时间
    uint32_t LastLongPressTickTime; 
    
    // 点击次数
    uint8_t ClickCnt;
    
}Key_TypeDef;

// 初始化按键
void Key_Init(Key_TypeDef *Key,Key_InitTypedef *Key_Initstruct);
// 处理按键
void Key_Proc(Key_TypeDef *Key);
// 获取按键状态
uint8_t Key_GetState(Key_TypeDef *Key);
#endif 
