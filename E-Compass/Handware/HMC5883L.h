#ifndef __HMC5883L_
#define __HMC5883L_


void HMC5883L_WriteReg(uint8_t RegAddress,uint8_t Data);//地址和数据
//指定地址读
uint8_t HMC5883L_ReadReg(uint8_t RegAddress);
void HMC5883L_Init();
void HMC5883L_GetData(float *MagX,float *MagY,float *MagZ);

	
#endif