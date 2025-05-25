#include "mpu6050.h"
#include "delay.h"
#include "task.h"
#include "math.h"

// 传感器原始数据和欧拉角
static float ax,ay,az,gx,gy,gz,temperature; 
static float yaw = 0,pitch = 0,roll = 0; // 欧拉角

static uint32_t nxt = 0;
static float pitch_ploy=0,roll_ploy=0,gz_ploy=0;

/**
 * @brief 初始化IIC总线
 */
void IIC_Start(void)
{
    SDA_OUT();
    SCL(1);
    SDA(0);

    SDA(1);
    delay_us(5);
    SDA(0);
    delay_us(5);

    SCL(0);
}

/**
 * @brief 停止IIC总线
 */
void IIC_Stop(void)
{
    SDA_OUT();
    SCL(0);
    SDA(0);

    SCL(1);
    delay_us(5);
    SDA(1);
    delay_us(5);
}

/**
 * @brief 发送ACK或NACK
 * @param ack 0表示ACK，1表示NACK
 */
void IIC_Send_Ack(unsigned char ack)
{
    SDA_OUT();
    SCL(0);
    SDA(0);
    delay_us(5);
    if(!ack) SDA(0);
    else     SDA(1);
    SCL(1);
    delay_us(5);
    SCL(0);
    SDA(1);
}

/**
 * @brief 等待ACK信号
 * @return 0表示收到ACK，1表示超时
 */
unsigned char I2C_WaitAck(void)
{
    char ack = 0;
    unsigned char ack_flag = 10;
    SCL(0);
    SDA(1);
    SDA_IN();

    SCL(1);
    while( (SDA_GET()==1) && ( ack_flag ) )
    {
        ack_flag--;
        delay_us(5);
    }

    if( ack_flag <= 0 )
    {
        IIC_Stop();
        return 1;
    }
    else
    {
        SCL(0);
        SDA_OUT();
    }
    return ack;
}

/**
 * @brief 发送一个字节
 * @param dat 要发送的数据
 */
void Send_Byte(uint8_t dat)
{
    int i = 0;
    SDA_OUT();
    SCL(0); // 拉低时钟开始数据传输

    for( i = 0; i < 8; i++ )
    {
        SDA( (dat & 0x80) >> 7 );
        delay_us(1);
        SCL(1);
        delay_us(5);
        SCL(0);
        delay_us(5);
        dat<<=1;
    }
}

/**
 * @brief 读取一个字节
 * @return 读取到的数据
 */
unsigned char Read_Byte(void)
{
    unsigned char i,receive=0;
    SDA_IN(); // SDA设置为输入
    for(i=0;i<8;i++ )
    {
        SCL(0);
        delay_us(5);
        SCL(1);
        delay_us(5);
        receive<<=1;
        if( SDA_GET() )
        {
            receive|=1;
        }
        delay_us(5);
    }

    SCL(0);

    return receive;
}

/**
 * @brief 向MPU6050写入数据
 * @param addr 设备地址
 * @param regaddr 寄存器地址
 * @param num 写入数据的长度
 * @param regdata 要写入的数据的指针
 * @return 0表示成功，其他值表示失败
 */
char MPU6050_WriteReg(uint8_t addr,uint8_t regaddr,uint8_t num,uint8_t *regdata)
{
    uint16_t i = 0;
    IIC_Start();
    Send_Byte((addr<<1)|0);
    if( I2C_WaitAck() == 1 ) {IIC_Stop();return 1;}
    Send_Byte(regaddr);
    if( I2C_WaitAck() == 1 ) {IIC_Stop();return 2;}

    for(i=0;i<num;i++)
    {
        Send_Byte(regdata[i]);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return (3+i);}
    }

    IIC_Stop();
    return 0;
}

/**
 * @brief 从MPU6050读取数据
 * @param addr 设备地址
 * @param regaddr 寄存器地址
 * @param num 要读取的数据长度
 * @param Read 存储读取数据的缓冲区
 * @return 0表示成功，其他值表示失败
 */
char MPU6050_ReadData(uint8_t addr, uint8_t regaddr,uint8_t num,uint8_t* Read)
{
    uint8_t i;
    IIC_Start();
    Send_Byte((addr<<1)|0);
    if( I2C_WaitAck() == 1 ) {IIC_Stop();return 1;}
    Send_Byte(regaddr);
    if( I2C_WaitAck() == 1 ) {IIC_Stop();return 2;}

    IIC_Start();
    Send_Byte((addr<<1)|1);
    if( I2C_WaitAck() == 1 ) {IIC_Stop();return 3;}

    for(i=0;i<(num-1);i++){
        Read[i]=Read_Byte();
        IIC_Send_Ack(0);
    }
    Read[i]=Read_Byte();
    IIC_Send_Ack(1);
    IIC_Stop();
    return 0;
}

/**
 * @brief 设置MPU6050陀螺仪量程范围
 * @param fsr 范围设置：0=±250dps, 1=±500dps, 2=±1000dps, 3=±2000dps
 * @return 0表示成功，其他值表示失败
 */
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU6050_WriteReg(0x68,MPU_GYRO_CFG_REG,1,(uint8_t*)(fsr<<3));
}

/**
 * @brief 设置MPU6050加速度计量程范围
 * @param fsr 范围设置：0=±2g, 1=±4g, 2=±8g, 3=±16g
 * @return 0表示成功，其他值表示失败
 */
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU6050_WriteReg(0x68,MPU_ACCEL_CFG_REG,1,(uint8_t*)(fsr<<3));
}

/**
 * @brief 设置MPU6050的数字低通滤波器
 * @param lpf 数字低通滤波频率(Hz)
 * @return 0表示成功，其他值表示失败
 */
uint8_t MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data=0;

    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return data=MPU6050_WriteReg(0x68,MPU_CFG_REG,1,&data);
}

/**
 * @brief 设置MPU6050的采样率
 * @param rate 采样率(4-1000Hz)
 * @return 0表示成功，其他值表示失败
 */
uint8_t MPU_Set_Rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU6050_WriteReg(0x68,MPU_SAMPLE_RATE_REG,1,&data);
    return MPU_Set_LPF(rate/2);
}

/**
 * @brief 读取MPU6050陀螺仪数据
 * @param gyroData 存储陀螺仪数据的数组
 */
void MPU6050ReadGyro(short *gyroData)
{
    uint8_t buf[6];
    uint8_t reg = 0;
    reg = MPU6050_ReadData(0x68,MPU6050_GYRO_OUT,6,buf);
    if( reg == 0 )
    {
        gyroData[0] = (buf[0] << 8) | buf[1];
        gyroData[1] = (buf[2] << 8) | buf[3];
        gyroData[2] = (buf[4] << 8) | buf[5];
    }
}

/**
 * @brief 读取MPU6050加速度计数据
 * @param accData 存储加速度计数据的数组
 */
void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    uint8_t reg = 0;
    reg = MPU6050_ReadData(0x68, MPU6050_ACC_OUT, 6, buf);
    if( reg == 0)
    {
        accData[0] = (buf[0] << 8) | buf[1];
        accData[1] = (buf[2] << 8) | buf[3];
        accData[2] = (buf[4] << 8) | buf[5];
    }
}

/**
 * @brief 读取MPU6050温度
 * @return 温度值（摄氏度）
 */
float MPU6050_GetTemp(void)
{
    short temp3;
    uint8_t buf[2];
    float Temperature = 0;
    MPU6050_ReadData(0x68,MPU6050_RA_TEMP_OUT_H,2,buf);
    temp3= (buf[0] << 8) | buf[1];
    Temperature=((double) temp3/340.0)+36.53;
    return Temperature;
}

/**
 * @brief 读取MPU6050的ID
 * @return 0表示检测到MPU6050，1表示未检测到
 */
uint8_t MPU6050ReadID(void)
{
    unsigned char Re[2] = {0};

    if (Re[0] != 0x68)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}

/**
 * @brief 初始化MPU6050
 * @return 0表示成功，1表示未检测到MPU6050
 */
char MPU6050_Init(void)
{
    SDA_OUT();
    delay_ms(10);
    // 复位MPU6050
    MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1, 1,(uint8_t*)(0x80));
    delay_ms(100);
    // 配置电源管理寄存器
    MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1,1, (uint8_t*)(0x00));

    MPU_Set_Gyro_Fsr(3);    // 陀螺仪量程：±2000dps
    MPU_Set_Accel_Fsr(0);   // 加速度计量程：±2g
    MPU_Set_Rate(50);       // 采样率：50Hz

    MPU6050_WriteReg(0x68,MPU_INT_EN_REG , 1,(uint8_t*)0x00);        // 关闭所有中断
    MPU6050_WriteReg(0x68,MPU_USER_CTRL_REG,1,(uint8_t*)0x00);       // 关闭I2C主模式
    MPU6050_WriteReg(0x68,MPU_FIFO_EN_REG,1,(uint8_t*)0x00);         // 关闭FIFO
    MPU6050_WriteReg(0x68,MPU_INTBP_CFG_REG,1,(uint8_t*)0X80);       // INT引脚低电平有效

    if( MPU6050ReadID() == 0 ) // 检查MPU6050
    {
        MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1, 1,(uint8_t*)0x01); // 设置CLKSEL，PLL X轴为参考
        MPU6050_WriteReg(0x68,MPU_PWR_MGMT2_REG, 1,(uint8_t*)0x00);     // 加速度与陀螺仪都工作
        MPU_Set_Rate(50);
        return 1;
    }
    
    // 校准处理
    for(int i=0;i<100;i++)
    {
        MPU6050_Update();
        gz_ploy+=Get_gz();
        //float pitch_g = pitch + (gx)*0.005;
        //float roll_g = roll - (gy)*0.005;
        //float pitch_a = atan2(ay,az)/3.1415927*180;
        //float roll_a = atan2(ax,az)/3.1415927*180;
        //pitch_ploy += 0.952381f*pitch_g + 0.047619f*pitch_a;
        //roll_ploy += 0.952381f*roll_g + 0.047619f*roll_a;
    }
    gz_ploy/=100;
    //pitch_ploy/=100;
    //roll_ploy/=100;
    return 0;
}

/**
 * @brief 更新MPU6050数据
 */
void MPU6050_Update(void)
{
    short acc_raw[3],gyro_raw[3];
    MPU6050ReadAcc(acc_raw);
    MPU6050ReadGyro(gyro_raw);
    ax = acc_raw[0] * 6.1035e-5f;
    //ay = acc_raw[1] * 6.1035e-5f;
    //az = acc_raw[2] * 6.1035e-5f;

    gx = gyro_raw[0] * 6.1035e-2f;
    //gy = gyro_raw[1] * 6.1035e-2f;
    //gz = gyro_raw[2] * 6.1035e-2f;

    float temperature_raw = MPU6050_GetTemp();
    temperature = temperature_raw / 333.87f + 21.0f; 
}

// 获取各种传感器数据的函数
float Get_ax(void) { return ax; }
float Get_ay(void) { return ay; }
float Get_az(void) { return az; }
float Get_gx(void) { return gx; }
float Get_gy(void) { return gy; }
float Get_gz(void) { return gz; }
float Get_temperature(void) { return temperature; }
float Get_yaw(void) { return yaw*0.72; }
float Get_pitch(void) { return pitch; }
float Get_roll(void) { return roll; }

static uint32_t start_tick = 0;

/**
 * @brief MPU6050数据处理函数
 */
void MPU6050_Proc(void)
{
    PERIODIC(5);
    MPU6050_Update();
    float yaw_g = yaw + (gz-gz_ploy)*0.005;
    //float pitch_g = pitch + (gx)*0.005;
    //float roll_g = roll - (gy)*0.005;
    //使用加速度计计算的角度
    //float pitch_a = atan2(ay,az)/3.1415927*180;
    //float roll_a = atan2(ax,az)/3.1415927*180;
    
    //使用互补滤波器对陀螺仪和加速度计的数据进行融合
    yaw = yaw_g;
    //pitch = 0.952381f*pitch_g + 0.047619f*pitch_a-pitch_ploy;
    //roll = 0.952381f*roll_g + 0.047619f*roll_a-roll_ploy;
}
