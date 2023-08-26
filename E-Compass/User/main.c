#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "math.h"

int16_t Angle;
int16_t  AX, AY, AZ, GX, GY, GZ;
uint8_t i=0 ;
float GaX,GaY,GaZ,GaXmax=0,GaXmin=0,GaYmax=0,GaYmin=0,Xoffest,Yoffest,Kx,Ky;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	HMC5883L_Init();	
	//进行校准，20秒内将传感器缓慢旋转几周获取各个方向磁场强度最大值和最小值
	while(i != 200)
	{
		HMC5883L_GetData(&GaX, &GaY, &GaZ);
		GaXmax = GaXmax < GaX? GaX:GaXmax;
		GaXmin = GaXmin > GaX? GaX:GaXmin;
		GaYmax = GaYmax < GaY? GaY:GaYmax;
		GaYmin = GaYmin > GaY? GaY:GaYmin;
		Delay_ms(100);
		i++;
		OLED_ShowNum(1, 1,i, 3);
	}
	Xoffest = (GaXmax+GaXmin)/2;
	Yoffest = (GaYmax+GaYmin)/2;
	Kx = 2/(GaXmax-GaXmin);
	Ky = 2/(GaXmax-GaXmin);Xoffest = (GaXmax+GaXmin)/2;
	Yoffest = (GaYmax+GaYmin)/2;
	Kx = 2/(GaXmax-GaXmin);
	Ky = 2/(GaXmax-GaXmin);
	
	while (1)
	{
		//读取角度，输出与北极的夹角
		HMC5883L_GetData(&GaX, &GaY, &GaZ);
		//获得校准后的磁场强度
		GaX = (GaX - Xoffest) * Kx;
		GaY = (GaY - Yoffest) * Ky;
		//计算角度，芯片是国产的，数据很不稳定，角度会发生大范围波动，也有可能是我焊的时候焊出问题了
		if((GaX > 0)&&(GaY < 0)) Angle = -atan(GaY/GaX)*57;
		else if((GaX > 0)&&(GaY > 0)) Angle = 360-atan(GaY/GaX)*57;
		else if((GaX == 0)&&(GaY < 0)) Angle = 90;
		else if((GaX == 0)&&(GaY > 0)) Angle = 270;
		else if(GaX < 0) Angle = 180-atan(GaY/GaX)*57;
		OLED_ShowNum(1, 7, Angle, 3);
		OLED_ShowString(1,1,"Angle:");
		//检测是否水平，误差设定为100至-100，这个范围比较合适，不容易波动，灵敏度也还行
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
	    if(AX<100&&AX>-100&&AY<100&&AY>-100)
			OLED_ShowString(2,1,"YesHorrizontal");
		else OLED_ShowString(2,1,"No_Horrizontal");
		Delay_ms(500);
	}
}






