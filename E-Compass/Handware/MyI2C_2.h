#ifndef __MYI2C_2_H
#define __MYI2C_2_H

void MyI2C_Init_2(void);
void MyI2C_Start_2(void);
void MyI2C_Stop_2(void);
void MyI2C_SendByte_2(uint8_t Byte);
uint8_t MyI2C_ReceiveByte_2(void);
void MyI2C_SendAck_2(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck_2(void);

#endif 