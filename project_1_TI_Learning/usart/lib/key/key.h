#ifndef __KEY_H_  // 如果未定义宏__KEY_H_，则编译以下代码
#define __KEY_H_  // 定义宏__KEY_H_，防止重复包含该头文件

#include "ti_msp_dl_config.h"  // 包含TI MSP系列微控制器的驱动配置头文件


// 定义按键初始化结构体
typedef struct 
{
    GPIO_Regs *key_port;  // 指向按键所连接的GPIO端口的指针
    unsigned int key_pin; // 按键所连接的GPIO引脚
    void (*key_pressed_cb)(void); // 按键按下时的回调函数指针
    void (*key_released_cb)(void); // 按键松开时的回调函数指针
    void (*key_clicked_cb)(uint8_t clicks); // 按键点击（单击、双击等）时的回调函数指针
	void (*key_long_pressed_cb)(uint8_t ticks); // 按键长按时的回调函数指针
    
}Key_InitTypedef;


// 定义按键结构体
typedef struct 
{
	/* 初始化参数 */
	GPIO_Regs *key_port; // 指向按键所连接的GPIO端口的指针
    unsigned int key_pin; // 按键所连接的GPIO引脚
    uint32_t  key_iidx; // 按键的中断索引
    int key_irqn; // 按键的中断号
    uint32_t LongPressTime; // 按钮长按时间阈值，单位毫秒
	uint32_t LongPressTickInterval; // 长按后持续触发的间隔
	uint32_t ClickInterval; // 连击时间间隔，单位毫秒
    void (*key_pressed_cb)(void); // 按键按下时的回调函数指针
    void (*key_released_cb)(void); // 按键松开时的回调函数指针
    void (*key_clicked_cb)(uint8_t clicks); // 按键点击（单击、双击等）时的回调函数指针
	void (*key_long_pressed_cb)(uint8_t ticks); // 按键长按时的回调函数指针
	
	uint8_t  LastState;     // 按钮上次的状态，0 - 松开，1 - 按下
	uint8_t  ChangePending; // 按钮的状态是否正在发生改变
	uint32_t PendingTime;   // 按钮状态开始变化的时间
	uint32_t LongPressThreshold; // 长按时间阈值

	uint32_t LastPressedTime;  // 按钮上次按下的时间
	uint32_t LastReleasedTime; // 按钮上次松开的时间
	
	uint8_t LongPressTicks; // 长按计数
	uint32_t LastLongPressTickTime;  // 上次长按触发的时间
	
	uint8_t ClickCnt; // 点击计数
	
}Key_TypeDef;




// 初始化按键
void Key_Init(Key_TypeDef *Key,Key_InitTypedef *Key_Initstruct);
// 处理按键状态
void Key_Proc(Key_TypeDef *Key);
// 获取按键状态
uint8_t Key_GetState(Key_TypeDef *Key);
#endif 