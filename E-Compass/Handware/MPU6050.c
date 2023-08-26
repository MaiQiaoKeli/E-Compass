#include "stm32f10x.h"                  // Device header
#include "MyI2C_2.h"                  // Device header

#define MPU6050_ADDRESS  0xD0
#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75

//指定寄存器位置写入一个比特
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)//地址和数据
{
	MyI2C_Start_2();
	MyI2C_SendByte_2(MPU6050_ADDRESS);
	MyI2C_ReceiveAck_2();//不对返回值进行判断，因为太麻烦了
	MyI2C_SendByte_2(RegAddress);//用于指定具体的读写寄存器
	MyI2C_ReceiveAck_2();
	MyI2C_SendByte_2(Data);
	MyI2C_ReceiveAck_2();
	MyI2C_Stop_2();
}

//指定地址读
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	MyI2C_Start_2();
	MyI2C_SendByte_2(MPU6050_ADDRESS);
	MyI2C_ReceiveAck_2();//不对返回值进行判断，因为太麻烦了
	MyI2C_SendByte_2(RegAddress);//用于指定具体的读写寄存器
	MyI2C_ReceiveAck_2();
	
	MyI2C_Start_2();
	MyI2C_SendByte_2(MPU6050_ADDRESS|0x01);//读写位设为1，需要读写从机数据
	MyI2C_ReceiveAck_2();
	Data=MyI2C_ReceiveByte_2();
	MyI2C_SendAck_2(1);//读取一个数据，然后不予应答，主机收回控制权
	MyI2C_Stop_2();
	
	return Data;
}

void MPU6050_Init()
{
	MyI2C_Init_2();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);//解除睡眠模式 
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00); 
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);
}

void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ)
{
	uint8_t DataH,DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//加速度寄存器x轴高八位
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);//加速度寄存器x轴低八位
	*AccX=(DataH<<8)|DataL;//左移，将两个8位数据拼接成16位的数据
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY=(DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ=(DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);//陀螺仪
	DataL=MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX=(DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY=(DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ=(DataH<<8)|DataL;
	
}