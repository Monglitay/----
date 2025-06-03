/**
 * @file jy62.c
 * @brief JY62陀螺仪模块驱动 - 数据接收与解算分离
 */

#include "jy62.h"
#include "delay.h"
#include "hc_06.h"
#include "task.h"
#include "usart.h"
#include <string.h>

// 原始数据缓冲区
typedef struct {
    uint8_t data[11];     // 完整数据包: 0x55 0x53 + 9字节数据
    uint8_t isNewData;    // 是否为新数据
} JY62RawData;


// 全局变量
static volatile JY62RawData rawBuffer;         // 原始数据缓冲区
static JY62AnglesData anglesData = {0, 0, 0, 0}; // 计算后的角度数据

// 状态机状态
#define WAIT_HEADER1 0
#define WAIT_HEADER2 1
#define RECEIVE_DATA 2

// 状态机变量
static volatile uint8_t RxState = WAIT_HEADER1;
static volatile uint8_t dataIndex = 0;

// 调试信息结构体
typedef struct {
    uint32_t bytesReceived;
    uint32_t packetsReceived;
    uint32_t checksumErrors;
    uint8_t currentState;
} JY62DebugInfo;

static volatile JY62DebugInfo jy62Debug = {0};

/**
 * @brief 初始化JY62陀螺仪，发送置偏航角置零命令
 */
void JY62_Init(void)
{
    USART_InitTypedef USART_InitStructure;
    USART_InitStructure.usart_inst = JY62_INST;
    USART_InitStructure.usart_inst_int_irqn = JY62_INST_INT_IRQN;
    USART_Init(&JY62, &USART_InitStructure);

    // 初始化变量
    memset((void*)&rawBuffer, 0, sizeof(JY62RawData));
    memset((void*)&jy62Debug, 0, sizeof(JY62DebugInfo));

    // 发送偏航角置零命令
    DL_UART_Main_transmitDataBlocking(JY62_INST, 0xFF);
    DL_UART_Main_transmitDataBlocking(JY62_INST, 0xAA);
    DL_UART_Main_transmitDataBlocking(JY62_INST, 0x52);
    delay_ms(500);
    
    // 发送连续输出模式命令
    DL_UART_Main_transmitDataBlocking(JY62_INST, 0xFF);
    DL_UART_Main_transmitDataBlocking(JY62_INST, 0xAA);
    DL_UART_Main_transmitDataBlocking(JY62_INST, 0x60);
}

/**
 * @brief JY62串口中断处理函数
 * 将数据读取到缓冲区，不进行解算
 */
void UART0_IRQHandler(void) 
{
    uint32_t intStatus = DL_UART_getPendingInterrupt(JY62_INST);
    
    if (intStatus == DL_UART_IIDX_RX)
    {
        uint8_t uartdata = DL_UART_Main_receiveData(JY62_INST);
        jy62Debug.bytesReceived++;
        
        uint8_t oldState = RxState;
        
        switch (RxState) {
        case WAIT_HEADER1:
            if (uartdata == 0x55) {
                RxState = WAIT_HEADER2;
                rawBuffer.data[0] = 0x55;  // 保存第一个头字节
            }
            break;
            
        case WAIT_HEADER2:
            if (uartdata == 0x53) {
                RxState = RECEIVE_DATA;
                rawBuffer.data[1] = 0x53;  // 保存第二个头字节
                dataIndex = 2;  // 从第三个位置开始存储数据
            } else {
                RxState = WAIT_HEADER1;  // 头部2不匹配，重置状态
            }
            break;
            
        case RECEIVE_DATA:
            if (dataIndex < 11) {  // 头部(2字节) + 数据(9字节) = 11字节
                rawBuffer.data[dataIndex++] = uartdata;
                
                if (dataIndex == 11) {
                    // 数据包接收完成，设置标志位
                    rawBuffer.isNewData = 1;
                    jy62Debug.packetsReceived++;
                    RxState = WAIT_HEADER1;  // 重置状态以等待下一个数据包
                }
            } else {
                // 索引超出范围，重置状态
                RxState = WAIT_HEADER1;
            }
            break;
        }
        
        // 记录状态变化
        if (oldState != RxState) {
            jy62Debug.currentState = RxState;
        }
    }
    
    DL_UART_clearInterruptStatus(JY62_INST, intStatus);
}

/**
 * @brief 检查是否有新的JY62数据
 * @return 1表示有新数据，0表示无新数据
 */
uint8_t JY62_HasNewData(void)
{
    return rawBuffer.isNewData;
}

/**
 * @brief 获取原始JY62数据
 * @param buffer 指向接收数据的缓冲区，大小至少为11字节
 * @return 1表示成功获取，0表示没有新数据
 */
uint8_t JY62_GetRawData(uint8_t *buffer)
{
    if (!rawBuffer.isNewData) {
        return 0;  // 没有新数据
    }
    
    // 创建临界区，禁用中断
    __disable_irq();
    memcpy(buffer, (void*)rawBuffer.data, 11);  // 拷贝完整数据包
    rawBuffer.isNewData = 0;  // 清除新数据标志
    __enable_irq();
    
    return 1;  // 成功获取数据
}

/**
 * @brief 计算JY62角度数据
 * @param rawData 指向原始数据的指针，应为11字节：头(0x55,0x53) + 数据(9字节)
 * @param angles 指向存储计算结果的结构体
 * @return 1表示计算成功，0表示失败(校验错误)
 */
uint8_t JY62_CalculateAngles(const uint8_t *rawData, JY62AnglesData *angles)
{
    // 确保数据有效
    if (rawData[0] != 0x55 || rawData[1] != 0x53) {
        return 0;  // 头部不正确
    }
    
    // 提取各字节
    uint8_t RollL = rawData[2];
    uint8_t RollH = rawData[3];
    uint8_t PitchL = rawData[4];
    uint8_t PitchH = rawData[5];
    uint8_t YawL = rawData[6];
    uint8_t YawH = rawData[7];
    uint8_t VL = rawData[8];
    uint8_t VH = rawData[9];
    uint8_t SUM = rawData[10];
    
    // 校验SUM是否正确
    uint8_t calculatedSum = 0x55 + 0x53 + RollH + RollL + PitchH + PitchL + YawH + YawL + VH + VL;
    
    if (calculatedSum != SUM) {
        jy62Debug.checksumErrors++;
        angles->isValid = 0;
        return 0;  // 校验失败
    }
    
    // 处理Roll角度
    int16_t rollRaw = ((uint16_t)RollH << 8) | RollL;
    float rollTemp = (float)rollRaw / 32768.0f * 180.0f;
    angles->roll = (rollTemp > 180.0f) ? (rollTemp - 360.0f) : rollTemp;
    
    // 处理Pitch角度
    int16_t pitchRaw = ((uint16_t)PitchH << 8) | PitchL;
    float pitchTemp = (float)pitchRaw / 32768.0f * 180.0f;
    angles->pitch = (pitchTemp > 180.0f) ? (pitchTemp - 360.0f) : pitchTemp;
    
    // 处理Yaw角度
    int16_t yawRaw = ((uint16_t)YawH << 8) | YawL;
    float yawTemp = (float)yawRaw / 32768.0f * 180.0f;
    angles->yaw = (yawTemp > 180.0f) ? (yawTemp - 360.0f) : yawTemp;
    
    angles->isValid = 1;  // 标记数据有效
    return 1;  // 计算成功
}

/**
 * @brief 更新JY62角度数据
 * @return 1表示有新数据更新，0表示无新数据
 */
uint8_t JY62_UpdateAngles(void)
{
    uint8_t rawData[11];
    
    if (JY62_GetRawData(rawData)) {
        // 获取到新数据，进行计算
        JY62AnglesData newAngles;
        if (JY62_CalculateAngles(rawData, &newAngles)) {
            // 计算成功，更新角度数据
            anglesData = newAngles;
            return 1;  // 更新成功
        }
    }
    
    return 0;  // 没有新数据或计算失败
}

/**
 * @brief 获取Roll角度
 * @return Roll角度值，范围-180到180度
 */
float JY62_GetRoll(void)
{
    return anglesData.roll;
}

/**
 * @brief 获取Pitch角度
 * @return Pitch角度值，范围-180到180度
 */
float JY62_GetPitch(void)
{
    return anglesData.pitch;
}

/**
 * @brief 获取Yaw角度
 * @return Yaw角度值，范围-180到180度
 */
float JY62_Get_Yaw(void)
{
    return anglesData.yaw;
}

/**
 * @brief 打印JY62调试信息
 */
void JY62_PrintDebugInfo(void)
{
    USART_Printf(&HC_06, "JY62状态: %d\n", jy62Debug.currentState);
    USART_Printf(&HC_06, "接收字节: %lu, 数据包: %lu, 校验错误: %lu\n", 
                jy62Debug.bytesReceived, jy62Debug.packetsReceived, 
                jy62Debug.checksumErrors);
    
    if (anglesData.isValid) {
        USART_Printf(&HC_06, "当前角度 - Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n",
                    anglesData.roll, anglesData.pitch, anglesData.yaw);
    } else {
        USART_Printf(&HC_06, "角度数据无效\n");
    }
}

/**
 * @brief 发送命令到JY62
 * @param cmd1 命令字节1
 * @param cmd2 命令字节2
 * @param cmd3 命令字节3
 */
void JY62_SendCommand(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3)
{
    DL_UART_Main_transmitDataBlocking(JY62_INST, cmd1);
    DL_UART_Main_transmitDataBlocking(JY62_INST, cmd2);
    DL_UART_Main_transmitDataBlocking(JY62_INST, cmd3);
}

/**
 * @brief 发送校准命令到JY62
 * @param cmd 校准命令类型
 */
void JY62_SendCalibrationCmd(JY62_CalibCmd cmd)
{
    switch (cmd) {
    case JY62_CALIB_ACCEL:  // 加速度校准
        JY62_SendCommand(0xFF, 0xAA, 0x67);
        break;
        
    case JY62_CALIB_GYRO:   // 陀螺仪校准
        JY62_SendCommand(0xFF, 0xAA, 0x65);
        break;
        
    case JY62_RESET_YAW:    // 偏航角置零
        JY62_SendCommand(0xFF, 0xAA, 0x52);
        break;
        
    case JY62_SET_BAUD_9600:  // 设置波特率9600
        JY62_SendCommand(0xFF, 0xAA, 0x63);
        break;
        
    case JY62_SET_BAUD_115200:  // 设置波特率115200
        JY62_SendCommand(0xFF, 0xAA, 0x64);
        break;
    }
}
