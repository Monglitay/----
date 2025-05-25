#include "mpu6050.h"
#include "delay.h"
#include "task.h"
#include "math.h"

// ������ԭʼ���ݺ�ŷ����
static float ax,ay,az,gx,gy,gz,temperature; 
static float yaw = 0,pitch = 0,roll = 0; // ŷ����

static uint32_t nxt = 0;
static float pitch_ploy=0,roll_ploy=0,gz_ploy=0;

/**
 * @brief ��ʼ��IIC����
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
 * @brief ֹͣIIC����
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
 * @brief ����ACK��NACK
 * @param ack 0��ʾACK��1��ʾNACK
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
 * @brief �ȴ�ACK�ź�
 * @return 0��ʾ�յ�ACK��1��ʾ��ʱ
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
 * @brief ����һ���ֽ�
 * @param dat Ҫ���͵�����
 */
void Send_Byte(uint8_t dat)
{
    int i = 0;
    SDA_OUT();
    SCL(0); // ����ʱ�ӿ�ʼ���ݴ���

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
 * @brief ��ȡһ���ֽ�
 * @return ��ȡ��������
 */
unsigned char Read_Byte(void)
{
    unsigned char i,receive=0;
    SDA_IN(); // SDA����Ϊ����
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
 * @brief ��MPU6050д������
 * @param addr �豸��ַ
 * @param regaddr �Ĵ�����ַ
 * @param num д�����ݵĳ���
 * @param regdata Ҫд������ݵ�ָ��
 * @return 0��ʾ�ɹ�������ֵ��ʾʧ��
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
 * @brief ��MPU6050��ȡ����
 * @param addr �豸��ַ
 * @param regaddr �Ĵ�����ַ
 * @param num Ҫ��ȡ�����ݳ���
 * @param Read �洢��ȡ���ݵĻ�����
 * @return 0��ʾ�ɹ�������ֵ��ʾʧ��
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
 * @brief ����MPU6050���������̷�Χ
 * @param fsr ��Χ���ã�0=��250dps, 1=��500dps, 2=��1000dps, 3=��2000dps
 * @return 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU6050_WriteReg(0x68,MPU_GYRO_CFG_REG,1,(uint8_t*)(fsr<<3));
}

/**
 * @brief ����MPU6050���ٶȼ����̷�Χ
 * @param fsr ��Χ���ã�0=��2g, 1=��4g, 2=��8g, 3=��16g
 * @return 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU6050_WriteReg(0x68,MPU_ACCEL_CFG_REG,1,(uint8_t*)(fsr<<3));
}

/**
 * @brief ����MPU6050�����ֵ�ͨ�˲���
 * @param lpf ���ֵ�ͨ�˲�Ƶ��(Hz)
 * @return 0��ʾ�ɹ�������ֵ��ʾʧ��
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
 * @brief ����MPU6050�Ĳ�����
 * @param rate ������(4-1000Hz)
 * @return 0��ʾ�ɹ�������ֵ��ʾʧ��
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
 * @brief ��ȡMPU6050����������
 * @param gyroData �洢���������ݵ�����
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
 * @brief ��ȡMPU6050���ٶȼ�����
 * @param accData �洢���ٶȼ����ݵ�����
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
 * @brief ��ȡMPU6050�¶�
 * @return �¶�ֵ�����϶ȣ�
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
 * @brief ��ȡMPU6050��ID
 * @return 0��ʾ��⵽MPU6050��1��ʾδ��⵽
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
 * @brief ��ʼ��MPU6050
 * @return 0��ʾ�ɹ���1��ʾδ��⵽MPU6050
 */
char MPU6050_Init(void)
{
    SDA_OUT();
    delay_ms(10);
    // ��λMPU6050
    MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1, 1,(uint8_t*)(0x80));
    delay_ms(100);
    // ���õ�Դ����Ĵ���
    MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1,1, (uint8_t*)(0x00));

    MPU_Set_Gyro_Fsr(3);    // ���������̣���2000dps
    MPU_Set_Accel_Fsr(0);   // ���ٶȼ����̣���2g
    MPU_Set_Rate(50);       // �����ʣ�50Hz

    MPU6050_WriteReg(0x68,MPU_INT_EN_REG , 1,(uint8_t*)0x00);        // �ر������ж�
    MPU6050_WriteReg(0x68,MPU_USER_CTRL_REG,1,(uint8_t*)0x00);       // �ر�I2C��ģʽ
    MPU6050_WriteReg(0x68,MPU_FIFO_EN_REG,1,(uint8_t*)0x00);         // �ر�FIFO
    MPU6050_WriteReg(0x68,MPU_INTBP_CFG_REG,1,(uint8_t*)0X80);       // INT���ŵ͵�ƽ��Ч

    if( MPU6050ReadID() == 0 ) // ���MPU6050
    {
        MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1, 1,(uint8_t*)0x01); // ����CLKSEL��PLL X��Ϊ�ο�
        MPU6050_WriteReg(0x68,MPU_PWR_MGMT2_REG, 1,(uint8_t*)0x00);     // ���ٶ��������Ƕ�����
        MPU_Set_Rate(50);
        return 1;
    }
    
    // У׼����
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
 * @brief ����MPU6050����
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

// ��ȡ���ִ��������ݵĺ���
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
 * @brief MPU6050���ݴ�����
 */
void MPU6050_Proc(void)
{
    PERIODIC(5);
    MPU6050_Update();
    float yaw_g = yaw + (gz-gz_ploy)*0.005;
    //float pitch_g = pitch + (gx)*0.005;
    //float roll_g = roll - (gy)*0.005;
    //ʹ�ü��ٶȼƼ���ĽǶ�
    //float pitch_a = atan2(ay,az)/3.1415927*180;
    //float roll_a = atan2(ax,az)/3.1415927*180;
    
    //ʹ�û����˲����������Ǻͼ��ٶȼƵ����ݽ����ں�
    yaw = yaw_g;
    //pitch = 0.952381f*pitch_g + 0.047619f*pitch_a-pitch_ploy;
    //roll = 0.952381f*roll_g + 0.047619f*roll_a-roll_ploy;
}
