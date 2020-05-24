#ifndef _mpu6050_h_
#define _mpu6050_h_

#include "sys.h"
#include "stm32f1xx_hal.h"
#include "main.h"


#define MPU6050_SCL_H() HAL_GPIO_WritePin(GPIOB, XYZ_SCL_Pin, GPIO_PIN_SET)
#define MPU6050_SCL_L() HAL_GPIO_WritePin(GPIOB, XYZ_SCL_Pin, GPIO_PIN_RESET)

#define MPU6050_SDA_H() HAL_GPIO_WritePin(GPIOB, XYZ_SDA_Pin, GPIO_PIN_SET)
#define MPU6050_SDA_L() HAL_GPIO_WritePin(GPIOB, XYZ_SDA_Pin, GPIO_PIN_RESET)

#define MPU6050_IN_SDA() HAL_GPIO_ReadPin(GPIOB, XYZ_SDA_Pin)

//#define ds_a = 0 IN_SDA

void MPU6050_IIC_Init(void);
void MPU6050_IIC_SDA_OUT(void);
void MPU6050_IIC_SDA_IN(void);
void MPU6050_IIC_Start(void);
void MPU6050_IIC_top(void);
void MPU6050_IIC_Ack(u8 a);
u8 MPU6050_IIC_Write_Byte(u8 dat);
u8 MPU6050_IIC_Read_Byte(void);
//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ


void MPU6050_Init(void);
int MPU6050_GetData(uint8_t REG_Address);
void Single_WriteI2C(uint8_t REG_Address,uint8_t REG_data);
uint8_t Single_ReadI2C(uint8_t REG_Address);
#endif

