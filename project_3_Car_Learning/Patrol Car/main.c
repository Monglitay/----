#include "control.h"

int main(void)
{
    // 系统级配置初始化
    SYSCFG_DL_init();
    
    Modules_Init();
    // 进入主循环，执行任务
    while (1) 
    {   
        Key_Proc(&Key1);
        JY62_UpdateAngles();
        switch (mode)
        {
            case 0:
                // 调试模式
                break;
            case 1:
                Control_AB();
                break;
            case 2:
                Control_ABCD();
                break;
            case 3:
                Control_ACBD();
                break;
        }
    }  
}