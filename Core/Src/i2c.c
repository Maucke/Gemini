#include "i2c.h"
#include "main.h"

void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);		
}

void delay_us(u16 us)
{
	u16 i,j;
	for(i=0;i<us;i++)
		for(j=0;j<30;j++)
	;
}
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
    IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK 
    return receive;
}

u8 read_IIC(u8 slave,u8 ReadAddr)
{				  
	  u8 temp=0;		  	    																 
    IIC_Start();  
    IIC_Send_Byte(slave);   //发送器件地址,写数据 	   
	  IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //发送低地址
	  IIC_Wait_Ack();	    
	  IIC_Start();  	 	   
	  IIC_Send_Byte(slave|0x01);           //进入接收模式			   
	  IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	  return temp;
}

void write_IIC(u8 slave,u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
    IIC_Send_Byte(slave);   //发送器件地址,写数据 	 
	  IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr);   //发送低地址
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(DataToWrite);     //发送字节							   
	  IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	  HAL_Delay(10);	 
}
u8 IIC_Write_Byte(u8 dat)
{
	u8 i;
	u8 IIC_Ack=0;	

	SDA_OUT(); 	    

	for(i = 0;i < 8;i++)
	{
		if(dat & 0x80)	
			IIC_SDA=1;
		else
			IIC_SDA=0;
			
		delay_us(2);
		IIC_SCL=1;
	  delay_us(2);
		IIC_SCL=0;
		dat<<=1;
	}

	IIC_SDA=1;;	//释放数据线

	SDA_IN();	//设置成输入

	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	
	IIC_Ack |= READ_SDA;	//读入应答位
	IIC_SCL=0;
	return IIC_Ack;	//返回应答信号
}

u8 IIC_Read_Byte1(void)
{
	u8 i;
	u8 x=0;

	IIC_SDA=1;	//首先置数据线为高电平

	SDA_IN();	//设置成输入

	for(i = 0;i < 8;i++)
	{
		x <<= 1;	//读入数据，高位在前

		delay_us(2);
		IIC_SCL=1;	//突变
		delay_us(2);
		
		if(READ_SDA)	x |= 0x01;	//收到高电平

		IIC_SCL=0;
		delay_us(2);
	}	//数据接收完成

	IIC_SCL=0;

	return x;	//返回读取到的数据
}

