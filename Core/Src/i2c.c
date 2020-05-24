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
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
    IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK 
    return receive;
}

u8 read_IIC(u8 slave,u8 ReadAddr)
{				  
	  u8 temp=0;		  	    																 
    IIC_Start();  
    IIC_Send_Byte(slave);   //����������ַ,д���� 	   
	  IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //���͵͵�ַ
	  IIC_Wait_Ack();	    
	  IIC_Start();  	 	   
	  IIC_Send_Byte(slave|0x01);           //�������ģʽ			   
	  IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	  return temp;
}

void write_IIC(u8 slave,u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
    IIC_Send_Byte(slave);   //����������ַ,д���� 	 
	  IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr);   //���͵͵�ַ
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	  IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
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

	IIC_SDA=1;;	//�ͷ�������

	SDA_IN();	//���ó�����

	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	
	IIC_Ack |= READ_SDA;	//����Ӧ��λ
	IIC_SCL=0;
	return IIC_Ack;	//����Ӧ���ź�
}

u8 IIC_Read_Byte1(void)
{
	u8 i;
	u8 x=0;

	IIC_SDA=1;	//������������Ϊ�ߵ�ƽ

	SDA_IN();	//���ó�����

	for(i = 0;i < 8;i++)
	{
		x <<= 1;	//�������ݣ���λ��ǰ

		delay_us(2);
		IIC_SCL=1;	//ͻ��
		delay_us(2);
		
		if(READ_SDA)	x |= 0x01;	//�յ��ߵ�ƽ

		IIC_SCL=0;
		delay_us(2);
	}	//���ݽ������

	IIC_SCL=0;

	return x;	//���ض�ȡ��������
}

