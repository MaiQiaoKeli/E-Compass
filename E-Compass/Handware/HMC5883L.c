#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"                  // Device header

#define HMC5883L_ADDRESS 0x1a

//指定寄存器位置写入数据
void HMC5883L_WriteReg(uint8_t RegAddress,uint8_t Data)//地址和数据
{
	MyI2C_Start();
	MyI2C_SendByte(HMC5883L_ADDRESS);
	MyI2C_ReceiveAck();//不对返回值进行判断，因为太麻烦了
	MyI2C_SendByte(RegAddress);//用于指定具体的读写寄存器
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

//指定地址读
uint8_t HMC5883L_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	MyI2C_Start();
	MyI2C_SendByte(HMC5883L_ADDRESS);
	MyI2C_ReceiveAck();//不对返回值进行判断，因为太麻烦了
	MyI2C_SendByte(RegAddress);//用于指定具体的读写寄存器
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(HMC5883L_ADDRESS|0x01);//读写位设为1，需要读写从机数据
	MyI2C_ReceiveAck();
	Data=MyI2C_ReceiveByte();
	MyI2C_SendAck(1);//读取一个数据，然后不予应答，主机收回控制权
	MyI2C_Stop();
	
	return Data;
}

void HMC5883L_Init()
{
    MyI2C_Init();
    HMC5883L_WriteReg(0x09,0x1d);
	
	HMC5883L_WriteReg(0x0A,0x41);
	
   HMC5883L_WriteReg(0x0b,0x01);
   // HMC5883L_WriteReg(0x02,0x00);
}

void HMC5883L_GetData(float *MagX,float *MagY,float *MagZ)
{
	uint8_t DataH,DataL;
	
	DataH=HMC5883L_ReadReg(0x00);//加速度寄存器x轴高八位
	DataL=HMC5883L_ReadReg(0x01);//加速度寄存器x轴低八位
	*MagX=(float)((DataH<<8)|DataL)/1090;//左移，将两个8位数据拼接成16位的数据
	
	DataH=HMC5883L_ReadReg(0x02);
	DataL=HMC5883L_ReadReg(0x03);
	*MagY=(float)((DataH<<8)|DataL)/1090;
	
	DataH=HMC5883L_ReadReg(0x04);
	DataL=HMC5883L_ReadReg(0x05);
	*MagZ=(float)((DataH<<8)|DataL)/1090;
}