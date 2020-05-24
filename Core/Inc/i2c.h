#ifndef _I2C_H
#define _I2C_H

#include "sys.h"

#define IIC_SCL    PBout(0) //SCL
#define IIC_SDA    PBout(1) //SDA	 
#define READ_SDA   PBin(1)  //输入SDA 


void delay_us(u16 us);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);	    
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);
u8 read_IIC(u8 slave,u8 ReadAddr);
void write_IIC(u8 slave,u8 WriteAddr,u8 DataToWrite);

void SDA_IN(void);  //浮空输入
void SDA_OUT(void);  //推挽输出

u8 IIC_Write_Byte(u8 dat);
u8 IIC_Read_Byte1(void);

#endif
