#ifndef __KEY_H_
#define __KEY_H_

#include "ti_msp_dl_config.h"


typedef struct 
{
    GPIO_Regs *key_port;
    unsigned int key_pin;
    void (*key_pressed_cb)(void);
    void (*key_released_cb)(void);
    void (*key_clicked_cb)(uint8_t clicks);
	void (*key_long_pressed_cb)(uint8_t ticks);
    
}Key_InitTypedef;


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
	
	uint8_t LongPressTicks;
	uint32_t LastLongPressTickTime; 
	
	uint8_t ClickCnt;
	
}Key_TypeDef;




void Key_Init(Key_TypeDef *Key,Key_InitTypedef *Key_Initstruct);
void Key_Proc(Key_TypeDef *Key);
uint8_t Key_GetState(Key_TypeDef *Key);
#endif 