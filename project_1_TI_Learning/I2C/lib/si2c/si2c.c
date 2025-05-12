#include "si2c.h"

#include "delay.h"



void SI2C_Init(SI2C_Typedef *SI2C,SI2C_InitTypedef *SI2C_InitStruct)
{
    SI2C->i2c_port = SI2C_InitStruct->i2c_port;
    SI2C->i2c_scl_iomux = SI2C_InitStruct->i2c_scl_iomux;
    SI2C->i2c_sda_iomux = SI2C_InitStruct->i2c_sda_iomux;
    SI2C->i2c_scl_pin = SI2C_InitStruct->i2c_scl_pin;
    SI2C->i2c_sda_pin = SI2C_InitStruct->i2c_sda_pin;
}

int SI2C_SendBytes(SI2C_Typedef *SI2C, uint8_t DevAddr, uint8_t RegAddr,uint8_t length,int8_t *data)
{
    uint16_t i = 0;
    SI2C_Start(SI2C);
    SI2C_Send_Byte(SI2C,(DevAddr<<1)|0);
    if( SI2C_Wait_Ack(SI2C) == 1 ) {SI2C_Stop(SI2C);return 1;}
    SI2C_Send_Byte(SI2C,RegAddr);
    if( SI2C_Wait_Ack(SI2C) == 1 ) {SI2C_Stop(SI2C);return 2;}

    for(i=0;i<length;i++)
    {
        SI2C_Send_Byte(SI2C,data[i]);
        if( SI2C_Wait_Ack(SI2C) == 1 ) {SI2C_Stop(SI2C);return (3+i);}
    }

    SI2C_Stop(SI2C);
    return 0;

}
int SI2C_RecieveBytes(SI2C_Typedef *SI2C, uint8_t DevAddr, uint8_t RegAddr,uint8_t length,int8_t *buffer)
{
    uint8_t i;
    SI2C_Start(SI2C);
    SI2C_Send_Byte(SI2C,(DevAddr<<1)|0);
    if( SI2C_Wait_Ack(SI2C) == 1 ) {SI2C_Stop(SI2C);return 1;}
    SI2C_Send_Byte(SI2C,RegAddr);
    if( SI2C_Wait_Ack(SI2C) == 1 ) {SI2C_Stop(SI2C);return 2;}

    SI2C_Start(SI2C);
    SI2C_Send_Byte(SI2C,(DevAddr<<1)|1);
    if( SI2C_Wait_Ack(SI2C) == 1 ) {SI2C_Stop(SI2C);return 3;}

    for(i=0;i<(length-1);i++){
            buffer[i]=SI2C_Recieve_Byte(SI2C);
            SI2C_Send_Ack(SI2C,0);
    }
    buffer[i]=SI2C_Recieve_Byte(SI2C);
    SI2C_Send_Ack(SI2C,1);
    SI2C_Stop(SI2C);
    return 0;
}
void SDA_Out(SI2C_Typedef *SI2C)
{
    DL_GPIO_initDigitalOutput(SI2C->i2c_sda_iomux);
    DL_GPIO_setPins(SI2C->i2c_port, SI2C->i2c_sda_pin);
    DL_GPIO_enableOutput(SI2C->i2c_port, SI2C->i2c_sda_pin);
}
void SDA_In(SI2C_Typedef *SI2C)
{
    DL_GPIO_initDigitalInput(SI2C->i2c_sda_iomux); 
}
static int SDA_Get(SI2C_Typedef *SI2C)
{
    return ((DL_GPIO_readPins(SI2C->i2c_port, SI2C->i2c_sda_pin) & SI2C->i2c_sda_pin ) > 0 ) ? 1 : 0;
}
static int SCL_Get(SI2C_Typedef *SI2C)
{
    return ((DL_GPIO_readPins(SI2C->i2c_port, SI2C->i2c_scl_pin) & SI2C->i2c_scl_pin ) > 0 ) ? 1 : 0;
}
static void SDA_Set_Byte(SI2C_Typedef *SI2C,int byte)
{
    byte ? (DL_GPIO_setPins(SI2C->i2c_port, SI2C->i2c_sda_pin)) : (DL_GPIO_clearPins(SI2C->i2c_port, SI2C->i2c_sda_pin));
}

static void SCL_Set_Byte(SI2C_Typedef *SI2C,int byte)
{
    byte ? (DL_GPIO_setPins(SI2C->i2c_port, SI2C->i2c_scl_pin)) : (DL_GPIO_clearPins(SI2C->i2c_port, SI2C->i2c_scl_pin));
}

static void SI2C_Start(SI2C_Typedef *SI2C)
{
    SDA_Out(SI2C);
    SCL_Set_Byte(SI2C, 1);
    SDA_Set_Byte(SI2C,0);
    SDA_Set_Byte(SI2C,1);
    delay_us(5);
    SDA_Set_Byte(SI2C,0);
    delay_us(5);    
    SCL_Set_Byte(SI2C,0);
}

static void SI2C_Stop(SI2C_Typedef *SI2C)
{
    SDA_Out(SI2C);
    SCL_Set_Byte(SI2C, 0);
    SDA_Set_Byte(SI2C,0);
    SCL_Set_Byte(SI2C,1);
    delay_us(5);
    SDA_Set_Byte(SI2C,1);
    delay_us(5);
}

static int SI2C_Wait_Ack(SI2C_Typedef *SI2C)
{
    char ack = 0;
    unsigned char ack_flag = 10;
    SCL_Set_Byte(SI2C,0);
    SDA_Set_Byte(SI2C, 1);
    SDA_In(SI2C);
    SCL_Set_Byte(SI2C, 1);
    while((SDA_Get(SI2C)==1)&&(ack_flag)) 
    {
        ack_flag--;
        delay_us(5);
    }
    if( ack_flag <= 0 )
    {
        SI2C_Stop(SI2C);
        return 1;
    }
    else 
    {
        SCL_Set_Byte(SI2C, 0);
        SDA_Out(SI2C);
    }
    return ack;
}

static void SI2C_Send_Ack(SI2C_Typedef *SI2C,uint8_t ack)
{
    SDA_Out(SI2C);
    SCL_Set_Byte(SI2C, 0);
    SDA_Set_Byte(SI2C,0);
    delay_us(5);
    if(!ack) SDA_Set_Byte(SI2C,0);
    else SDA_Set_Byte(SI2C,1);
    SCL_Set_Byte(SI2C,1);
    delay_us(5);
    SCL_Set_Byte(SI2C,0);
    SDA_Set_Byte(SI2C,1);
}

static void SI2C_Send_Byte(SI2C_Typedef *SI2C,uint8_t byte)
{
    uint8_t i;
    SDA_Out(SI2C);
    SCL_Set_Byte(SI2C, 0);
    for(i=0;i<8;i++)
    {
        SDA_Set_Byte(SI2C,(byte & 0x80)>>7);
        delay_us(1);
        SCL_Set_Byte(SI2C,1);
        delay_us(5);
        SCL_Set_Byte(SI2C,0);
        delay_us(5);
        byte <<= 1;
    }
}

static uint8_t SI2C_Recieve_Byte(SI2C_Typedef *SI2C)
{
    char i,byte = 0;
    SDA_In(SI2C);
    for(i=0;i<8;i++)
    {
        SCL_Set_Byte(SI2C,0);
        delay_us(5);
        SCL_Set_Byte(SI2C,1);
        delay_us(5);
        byte<<=1;
        if(SDA_Get(SI2C))
        {
            byte |= 1;
        }
        delay_us(5);
    }
    SCL_Set_Byte(SI2C,0);
    return byte;
}

