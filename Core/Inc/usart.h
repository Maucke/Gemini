/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "sys.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

#define Uart_Max_Length 250
#define Uart_FFT_Length 192
extern uint8_t Uart_Recv1_Buf[];
extern uint8_t Uart_Overflow1_Flag;
extern uint16_t Uart_Recv1_Length;

extern uint8_t Uart_Recv2_Buf[];
extern uint8_t Uart_Overflow2_Flag;
extern uint16_t Uart_Recv2_Length;
extern uint8_t Uart_Overflow3_Flag;
#define End_Frame_ADDR 0x55AA
#define WEATHER_FLASH 9216*1024 //1MB
#define VOICE_FLASH 2048*1024 //7MB
#define FML_FLASH 10240*1024 //1MB
#define RAB_FLASH 1024*1024	//1MB
#define ROG_FLASH 4096	//400KB
#define SAVE_FLASH 0

extern u8 Uart_Recv3_Data;
typedef struct
{
	u8 hour;
	u8 minute;
	u8 cation;//֪ͨ
	u8 mute;//���� NO use
	u8 alarm;//����
	u8 tmsys;
	u8 wifi;//WiFi״̬
	u8 aiwork;
} CLOCK_MSG;

extern CLOCK_MSG Clock_Msg;

#define CPU_Temp 						0x1
#define CPU_Clock 					0x2
#define CPU_Load 						0x3
#define CPU_Fan 						0x4

#define GPU_Temp 						0x11
#define GPU_Clock 					0x12
#define GPU_Load 						0x13
#define GPU_Fan 						0x14

#define Main_Temp 					0x21
#define Main_Fan 						0x22
#define Main_Vol						0x23

#define RAM_Load 	  				0x31
#define RAM_Data 						0x32

#define HDD_Load 	  				0x41
#define HDD_Temp 						0x42

#define Uart_Year 	 			 	0x201
#define Uart_Month 					0x202
#define Uart_Day 	  				0x203
#define Uart_Week 					0x204
#define Uart_Hour 	  			0x205
#define Uart_Minute 				0x206
#define Uart_Second 	  		0x207
#define CPU_POWER 	  			0x208

#define CPU_NAME 	 		 		0x101
#define GPU_NAME 	 		 		0x102
#define Main_NAME 	 		 	0x103
#define Uart_END	 	  		0x55AA

#define Command_Mode      0x8001
#define Command_Motion    0x8002
#define Command_Style     0x8003
#define Command_Speed     0x8004
#define Command_Brightness 0x8005
#define Command_Set       0x8006
#define Command_RGBMode   0x8007
#define Command_RGBColor  0x8008
#define Command_RGBBrightness 0x8009

#define ESP_Weather 	 	0x401
#define ESP_Temp 	 			0x402
#define ESP_Humi 	 			0x403
#define ESP_Wind_Dir 	 	0x404
#define ESP_Wind_Pw 	 	0x405
#define ESP_Report_Tm	 	0x406

#define ESP_WeatherNum			0x400
#define ESP_Weather				0x401
#define ESP_Temp				0x402
#define ESP_Humi				0x403
#define ESP_Wind_Dir			0x404
#define ESP_Wind_Pw				0x405
#define ESP_Report_Tm			0x406

#define ESP_D1_WeatherNum		0x410
#define ESP_D1_Weeek			0x411
#define ESP_D1_Weather			0x412
#define ESP_D1_Temp				0x413
#define ESP_D1_NTemp			0x414

#define ESP_D2_WeatherNum		0x420
#define ESP_D2_Weeek			0x421
#define ESP_D2_Weather			0x422
#define ESP_D2_Temp				0x423
#define ESP_D2_NTemp			0x424

#define ESP_D3_WeatherNum		0x430
#define ESP_D3_Weeek			0x431
#define ESP_D3_Weather			0x432
#define ESP_D3_Temp				0x433
#define ESP_D3_NTemp			0x434

#define ESP_Year 	 			0x501
#define ESP_Month 				0x502
#define ESP_Day 	  			0x503
#define ESP_Week 				0x504
#define ESP_Hour 	  			0x505
#define ESP_Minute 				0x506
#define ESP_Second 	  			0x507

#define ESP_Bili_Msg 	  		0x511
#define ESP_Bili_Fow   			0x512


#define FFT_Data 0x0301
#define Left_Vol 0x0302
#define Right_Vol 0x0303

//			SendValue(0x8001, Convert.ToUInt16(Ctl.Mode)); // ��ʾģʽ
//			Thread.Sleep(2);
//			SendValue(0x8002, Convert.ToUInt16(Ctl.Motion)); // ��ʾģʽ
//			Thread.Sleep(2);
//			SendValue(0x8003, Convert.ToUInt16(Ctl.Style)); // ��ʾģʽ
//			Thread.Sleep(2);
//			SendValue(0x8004, Convert.ToUInt16(Ctl.Speed)); // ��ʾģʽ
//			Thread.Sleep(2);
typedef struct
{
	u16 cputemp;
	u16 cpuclock;
	u16 cpuload;
	u16 cpufan;
	u16 cpupower;

	u16 gputemp;
	u16 gpuclock;
	u16 gpuload;
	u16 gpufan;

	u16 maintemp;
	u16 mainfan;
	u16 mainvoltage;

	u16 ramload;
	u16 ramusrdata;

	u16 hddload;
	u16 hddtemp;

	u16 uartyear;
	u16 uartmonth;
	u16 uartday;
	u16 uartweek;
	u16 uarthour;
	u16 uartminute;
	u16 uartsecond;

	char cpuname[40];
	char gpuname[40];
	char mainname[40];
	u8 fft[Uart_FFT_Length];
	u16 leftvol;
	u16 rightvol;
}DEVICE_MSG;

typedef struct
{
	u16 weathernum;
	char weather[10];
	char temp[10];
	char humidity[10];
	char winddir[10];
	char windpw[10];
	char reporttime[10];
	
	u16 weather1num;
	char week1[10];
	char weather1[10];
	char temp1[10];
	char ntemp1[10];
	
	u16 weather2num;
	char week2[10];
	char weather2[10];
	char temp2[10];
	char ntemp2[10];
	
	u16 weather3num;
	char week3[10];
	char weather3[10];
	char temp3[10];
	char ntemp3[10];
	
	u16 year;
	u16 month;
	u16 day;
	u16 week;
	u16 hour;
	u16 minute;
	u16 second;
	
	u16 bilifans;
	u16 bilimsg;
}WIFI_MSG;
typedef struct
{
	u16 commandmode;
	u16 commandmotion;
	u16 commandstyle;
	u16 commandspeed;
	u16 commandbrightness;
	u16 commandset;
	u16 commandrgbmode;
	u16 commandrgbbrightness;
	u16 commandrgbcolor;
	
	u16 commandvfdbrightness;
}DEVICE_CMD;


typedef struct
{
	char cputemp[10];	
	char cpuclock[10];	
	char cpuload[10];	
	char cpufan[10];	

	char gputemp[10];	
	char gpuclock[10];	
	char gpuload[10];	
	char gpufan[10];		

	char maintemp[10];		
	char mainfan[10];

	char ramload[10];	
	char ramusrdata[10];
	
	char ngputemp[10];	
	char ncpuclock[10];	
	char ngpufan[10];		
	char ncpufan[10];	
	
	char vcputemp[10];	
	char vgputemp[10];	
	
}DEVICE_STR;
extern u8 SaveFlag;
extern DEVICE_STR Device_Str;
extern DEVICE_MSG Device_Msg;
extern DEVICE_CMD Device_Cmd;
extern WIFI_MSG WiFi_Msg;
extern uint8_t OfflineCount;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern uint8_t Uart_Recv1_Data;
extern uint8_t Uart_Recv3_Data;

void ConvertData(void);
void InitData(void);
void AnalysisComputermsg(uint8_t *Buf);
void AnalysisComputername(uint8_t *Buf);
void AnalysisMiStaus(uint8_t Data);
void AnalysisWiFiString(uint8_t *Buf);
void AnalysisWiFiInter(uint8_t *Buf);
void AnalysisFFT(uint8_t *Buf);
void AnalysisCommand(uint8_t *Buf);
void ClearFFT(void);
void UsartCommand(u16 Addr,u16 Data);
void ChangeCmd(u16 Type);
void UsartPrint(char *Data);
void UsartClear(void);
void Tranfcmd(void);
void Recvcmd(void);

int cont_str(char *s);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
