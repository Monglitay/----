#ifndef	__JY62_H__
#define __JY62_H__

#include "ti_msp_dl_config.h"
#include "usart.h"
#include <stdint.h>
USART_Typedef JY62;

// 角度数据结构
typedef struct {
    float roll;           // 横滚角
    float pitch;          // 俯仰角
    float yaw;            // 偏航角
    uint8_t isValid;      // 数据是否有效
} JY62AnglesData;

// 校准命令类型
typedef enum {
    JY62_CALIB_ACCEL,     // 加速度校准
    JY62_CALIB_GYRO,      // 陀螺仪校准
    JY62_RESET_YAW,       // 偏航角置零
    JY62_SET_BAUD_9600,   // 设置波特率9600
    JY62_SET_BAUD_115200  // 设置波特率115200
} JY62_CalibCmd;

/**
 * @brief 初始化JY62陀螺仪
 */
void JY62_Init(void);

/**
 * @brief 检查是否有新的JY62数据
 * @return 1表示有新数据，0表示无新数据
 */
uint8_t JY62_HasNewData(void);

/**
 * @brief 获取原始JY62数据
 * @param buffer 指向接收数据的缓冲区，大小至少为11字节
 * @return 1表示成功获取，0表示没有新数据
 */
uint8_t JY62_GetRawData(uint8_t *buffer);

/**
 * @brief 计算JY62角度数据
 * @param rawData 指向原始数据的指针，应为11字节：头(0x55,0x53) + 数据(9字节)
 * @param angles 指向存储计算结果的结构体
 * @return 1表示计算成功，0表示失败(校验错误)
 */
uint8_t JY62_CalculateAngles(const uint8_t *rawData, JY62AnglesData *angles);

/**
 * @brief 更新JY62角度数据
 * @return 1表示有新数据更新，0表示无新数据
 */
uint8_t JY62_UpdateAngles(void);

/**
 * @brief 获取Roll角度
 * @return Roll角度值，范围-180到180度
 */
float JY62_GetRoll(void);

/**
 * @brief 获取Pitch角度
 * @return Pitch角度值，范围-180到180度
 */
float JY62_GetPitch(void);

/**
 * @brief 获取Yaw角度
 * @return Yaw角度值，范围-180到180度
 */
float JY62_Get_Yaw(void);

/**
 * @brief 打印JY62调试信息
 */
void JY62_PrintDebugInfo(void);

/**
 * @brief 发送命令到JY62
 * @param cmd1 命令字节1
 * @param cmd2 命令字节2
 * @param cmd3 命令字节3
 */
void JY62_SendCommand(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3);

/**
 * @brief 发送校准命令到JY62
 * @param cmd 校准命令类型
 */
void JY62_SendCalibrationCmd(JY62_CalibCmd cmd);

#endif /* JY62_H */

