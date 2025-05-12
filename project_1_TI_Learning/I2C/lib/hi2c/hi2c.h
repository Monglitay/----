#ifndef __HI2C_H_
#define __HI2C_H_

#include "ti_msp_dl_config.h"
#include "delay.h"
typedef struct
{
    I2C_Regs *i2c_inst;
}I2C_InitTypedef;

typedef struct
{
    I2C_Regs *i2c_inst;
}I2C_Tpyedef;

void I2C_Init(I2C_Tpyedef *I2Cx,I2C_InitTypedef *I2C_InitStruct);
int I2C_Write(I2C_Tpyedef *I2Cx,unsigned char slave_addr,
                    unsigned char reg_addr,
                    unsigned char length,
                     unsigned char const *data);

int I2C_Read(I2C_Tpyedef *I2Cx,unsigned char slave_addr,
                    unsigned char reg_addr,
                    unsigned char length,
                    unsigned char *data);

#endif