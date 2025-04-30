#ifndef __SI2C_H_
#define __SI2C_H_

#include "ti_msp_dl_config.h"
typedef struct  
{
    int i2c_scl_iomux;
    int i2c_sda_iomux;
    unsigned int i2c_scl_pin;
    unsigned int i2c_sda_pin;
    GPIO_Regs *i2c_port;
}SI2C_InitTypedef;

typedef struct
{
    int i2c_scl_iomux;
    int i2c_sda_iomux;
    unsigned int i2c_scl_pin;
    unsigned int i2c_sda_pin;
    GPIO_Regs *i2c_port;
}SI2C_Typedef;
void SI2C_Init(SI2C_Typedef *SI2C,SI2C_InitTypedef *SI2C_InitStruct);
int SI2C_SendBytes(SI2C_Typedef *SI2C, uint8_t DevAddr, uint8_t RegAddr,uint8_t length,int8_t *data);
int SI2C_RecieveBytes(SI2C_Typedef *SI2C, uint8_t DevAddr, uint8_t RegAddr,uint8_t length,int8_t *buffer);

static void SDA_In(SI2C_Typedef *SI2C);
static int SDA_Get(SI2C_Typedef *SI2C);
static int SCL_Get(SI2C_Typedef *SI2C);
static void SDA_Set_Byte(SI2C_Typedef *SI2C,int byte);
static void SCL_Set_Byte(SI2C_Typedef *SI2C,int byte);
static void SI2C_Start(SI2C_Typedef *SI2C);
static void SI2C_Stop(SI2C_Typedef *SI2C);
static int SI2C_Wait_Ack(SI2C_Typedef *SI2C);
static void SI2C_Send_Ack(SI2C_Typedef *SI2C,uint8_t ack);
static void SI2C_Send_Byte(SI2C_Typedef *SI2C,uint8_t byte);
static uint8_t SI2C_Recieve_Byte(SI2C_Typedef *SI2C);

#endif

