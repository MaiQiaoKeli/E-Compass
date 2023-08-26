#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MyI2C_W_SCL_2(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA_2(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,(BitAction)BitValue);
	Delay_us(10);//加不加延时mpu6050作为从机都能跟上
}

uint8_t MyI2C_R_SDA_2(void)
{
	uint8_t Bit;
	Bit=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
	Delay_us(10);//加不加延时mpu6050作为从机都能跟上
	return Bit;
}


void MyI2C_Init_2(void){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
}

void MyI2C_Start_2()
{
	MyI2C_W_SDA_2(1);
	MyI2C_W_SCL_2(1);
	MyI2C_W_SDA_2(0);	
	MyI2C_W_SCL_2(0);

}

void MyI2C_Stop_2()
{
	MyI2C_W_SDA_2(0);
	MyI2C_W_SCL_2(1);
	MyI2C_W_SDA_2(1);
}


//主机发送数据
void MyI2C_SendByte_2(uint8_t Byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		MyI2C_W_SDA_2(Byte&0x80>>i);
		MyI2C_W_SCL_2(1);
		MyI2C_W_SCL_2(0);
	}//连续发送8个比特以发送一个字节
}

//主机接受数据
uint8_t MyI2C_ReceiveByte_2(void)
{
	uint8_t i,Byte=0x00;
	MyI2C_W_SDA_2(1);
	for(i=0;i<8;i++)
	{
		MyI2C_W_SCL_2(1);
		if(MyI2C_R_SDA_2()==1){Byte|=0x80>>i;}
		MyI2C_W_SCL_2(0);
	}
	return Byte;	
}

//发送应答
void MyI2C_SendAck_2(uint8_t Byte)
{
	MyI2C_W_SDA_2(Byte);
	MyI2C_W_SCL_2(1);
	MyI2C_W_SCL_2(0);
}

//接收应答
uint8_t MyI2C_ReceiveAck_2(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA_2(1);//主机释放SDA交给从机操作
	MyI2C_W_SCL_2(1);
	AckBit=MyI2C_R_SDA_2();
	MyI2C_W_SCL_2(0);
	return AckBit;
}
