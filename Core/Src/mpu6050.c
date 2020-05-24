#include "mpu6050.h"

void MPU6050_IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = XYZ_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void MPU6050_IIC_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = XYZ_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void MPU6050_delay_us(u16 us)
{
	u8 i,j;
	for(i=0;i<us;i++)
		for(j=0;j<50;j++)
	;
}

/*I2C��ʼ*/
void MPU6050_IIC_Start(void)
{
	MPU6050_IIC_SDA_OUT();	

	MPU6050_SDA_H();
	MPU6050_delay_us(2);	
	MPU6050_SCL_H();
	MPU6050_delay_us(2);		
	MPU6050_SDA_L();
	MPU6050_delay_us(2);	
	MPU6050_SCL_L();
	MPU6050_delay_us(2);
}
/*I2Cֹͣ*/
void MPU6050_IIC_Stop(void)
{
	MPU6050_IIC_SDA_OUT();	

	MPU6050_SDA_L();	
	MPU6050_delay_us(2);
	MPU6050_SCL_L();	
	MPU6050_delay_us(2);
	MPU6050_SDA_H();
	MPU6050_delay_us(2);
}
/*I2C����Ӧ��*/
void MPU6050_IIC_Ack(u8 a)
{
	MPU6050_IIC_SDA_OUT();	

	if(a)	
		MPU6050_SDA_H();
	else	
		MPU6050_SDA_L();

	MPU6050_delay_us(2);
	MPU6050_SCL_H();	
	MPU6050_delay_us(2);
	MPU6050_SCL_L();
	MPU6050_delay_us(2);

}
/*I2Cд��һ���ֽ�*/
u8 MPU6050_IIC_Write_Byte(u8 dat)
{
	u8 i;
	u8 MPU6050_IIC_Ack=0;	

	MPU6050_IIC_SDA_OUT();	

	for(i = 0;i < 8;i++)
	{
		if(dat & 0x80)	
			MPU6050_SDA_H();
		else
			MPU6050_SDA_L();
			
		MPU6050_delay_us(2);
		MPU6050_SCL_H();
	    MPU6050_delay_us(2);
		MPU6050_SCL_L();
		dat<<=1;
	}

	MPU6050_SDA_H();	//�ͷ�������

	MPU6050_IIC_SDA_IN();	//���ó�����

	MPU6050_delay_us(2);
	MPU6050_SCL_H();
	MPU6050_delay_us(2);
	
	MPU6050_IIC_Ack |= MPU6050_IN_SDA();	//����Ӧ��λ
	MPU6050_SCL_L();
	MPU6050_delay_us(4);
	return MPU6050_IIC_Ack;	//����Ӧ���ź�
}
/*I2C��ȡһ���ֽ�*/
u8 MPU6050_IIC_Read_Byte(void)
{
	u8 i;
	u8 x=0;

	MPU6050_SDA_H();	//������������Ϊ�ߵ�ƽ

	MPU6050_IIC_SDA_IN();	//���ó�����

	for(i = 0;i < 8;i++)
	{
		x <<= 1;	//�������ݣ���λ��ǰ

		MPU6050_delay_us(2);
		MPU6050_SCL_H();	//ͻ��
		MPU6050_delay_us(2);
		
		if(MPU6050_IN_SDA())	x |= 0x01;	//�յ��ߵ�ƽ

		MPU6050_SCL_L();
		MPU6050_delay_us(2);
	}	//���ݽ������

	MPU6050_SCL_L();

	return x;	//���ض�ȡ��������
}

//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteI2C(uint8_t REG_Address,uint8_t REG_data)
{
    MPU6050_IIC_Start();                  //��ʼ�ź�
    MPU6050_IIC_Write_Byte(SlaveAddress);   //�����豸��ַ+д�ź�
    MPU6050_IIC_Write_Byte(REG_Address);    //�ڲ��Ĵ�����ַ��
    MPU6050_IIC_Write_Byte(REG_data);       //�ڲ��Ĵ������ݣ�
    MPU6050_IIC_Stop();                   //����ֹͣ�ź�
}
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
uint8_t Single_ReadI2C(uint8_t REG_Address)
{
	uint8_t REG_data;
	MPU6050_IIC_Start();                   //��ʼ�ź�
	MPU6050_IIC_Write_Byte(SlaveAddress);    //�����豸��ַ+д�ź�
	MPU6050_IIC_Write_Byte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
	MPU6050_IIC_Start();                   //��ʼ�ź�
	MPU6050_IIC_Write_Byte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=MPU6050_IIC_Read_Byte();       //�����Ĵ�������
	MPU6050_IIC_Ack(1);                //����Ӧ���ź�
	MPU6050_IIC_Stop();                    //ֹͣ�ź�
	return REG_data;
}
//**************************************
//��ʼ��MPU6050
//**************************************
void MPU6050_Init(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//**************************************
//�ϳ�����
//**************************************
int MPU6050_GetData(uint8_t REG_Address)
{
	uint8_t H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //�ϳ�����
}

