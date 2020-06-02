/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "math.h"
#include "config.h"
#include "oled.h"
#include "ds3231.h"
//#include "mpu6050.h"
#include "flash.h"
#include "stdio.h"
#include "vfd.h"	 
#include "apds9960.h"
#include "sk6812.h"	 
#include "remote.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SLEEPTIME    20*60

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t Flag_Refrash = False;
uint8_t Flag_Blink = False;
uint8_t Flag_Sleep = False;
uint8_t Flag_Continue = False;
uint8_t Flag_Reception = True;
//uint8_t Flag_Uptime = True;
uint8_t OfflineCount = 7;
uint8_t SystemActive = False;
uint8_t SetTimeFlag = True;
uint8_t RespFlag = True;

char Display_Time[6]="--:--";
char Display_State[5] = "----";
char Display_Notification[6]="V1.00";

uint8_t Remote_Value;
uint8_t RandomP = 6;
u16 HumanCount = 40;
uint8_t RandomPosX = 30,RandomPosY = 16;
u8 BTVPostionX = 1;//124
u8 BTVPostionY = 20;//30
u16 VFD_Bright = 10;
u16 VFD_Bright_Con = 10;
u8 VFDDisFlag = False;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
u8 ConvertWeather(u16 Num)
{
	if (Num == 0) {
		return 6;  //晴
	}
	else if (Num >= 1 && Num <= 3) {
		return 0;  //多云
	}
	else if (Num == 4) {
		return 4;   //阴
	}
	else if (Num >= 5 && Num <= 17) {
		return 4;    //风
	}
	else if (Num >= 18 && Num <= 21) {
		return 3;   //小雨
	}
	else if (Num >= 22 && Num <= 29) {
		return 1;   //大雨
	}
	else if (Num >= 30 && Num <= 32) {
		return 3;   //小雨
	}
	else if (Num >= 33 && Num <= 40) {
		return 1;   //大雨
	}
	else if (Num >= 41 && Num <= 49) {
		return 5;   //雪
	}
	else if (Num >= 50 && Num <= 54) {
		return 4;  //沙尘
	}
	else if (Num >= 55 && Num <= 60) {
		return 4;   //雾
	}
	else if (Num >= 61 && Num <= 64) {
		return 4;   //霾
	}
	else {
		return 0;   //未知
	}
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

#define TrumHeight   96
#define TrumWidth    1
#define TrumInterval 1
#define TrumNum	   	 152

uint8_t fall_pot[250]={0};	//记录下落点的坐标
uint8_t flow_pot[250]={0};

u16 Display_Mode = MODE_OFFLINE;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void OLED_Boot(void);
void TIME_Mode(void);

u8 BootGRAM[304*48];

int cont_str(char *s)
{
	int i = 0;      
	while ( s[++i] != '\0')   ;
	return i-1;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM4_Init();
  MX_SPI2_Init();
  MX_TIM8_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
//	printf("Init START\r\n");
	OLED_Init();
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart1,Uart_Recv1_Buf,Uart_Max_Length);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart2,Uart_Recv2_Buf,Uart_Max_Length);
	SK6812_Init();
//	DS3231_Time_Init(DS3231_Init_Buf);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim8);
	SPI_Flash_Init();
	VFD_Init();
	Device_Cmd.commandmode = 1;
//	HAL_GPIO_WritePin(GPIOA,ESP_PW_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,MT3608_EN_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,DS_PW_Pin,GPIO_PIN_RESET);
	Remote_Init();				    	//初始化	红外接收	
	OLED_AllMotion_Init();
	HAL_Delay(50);
	InitData();
	Recvcmd();
	UsartCommand(0xA002,3);//获取命令
//	HAL_Delay(200);
#if GEMINI == 1
	SparkFun_APDS9960();
	HAL_Delay(10);
	printf("APDS-9960 complete\r\n");
	if(SparkFun_APDS9960_init()) 
	{
		printf("APDS-9960 initialization complete\r\n");
	} 
	else  
	{
		printf("Something went wrong during APDS-9960 init!\r\n");
	}   
	HAL_Delay(10);
	if (enableGestureSensor(true) ) 
	{
		printf("Gesture sensor is now running\r\n");
	} 
	else 
	{
		printf("Something went wrong during gesture sensor init!\r\n");
	}
#endif
//	Device_Cmd.commandmotion = 0xF;
//	Device_Cmd.commandspeed = 500;
//	Device_Cmd.commandset = 0x8;
//	Device_Cmd.commandbrightness = 0xff;
	sprintf(Device_Str.vcputemp,"---#");	
	sprintf(Device_Str.vgputemp,"---#");	
//	printf("Flash ID is 0x%X\r\n",SPI_Flash_ReadID());
	HAL_UART_Receive_IT(&huart3,&Uart_Recv3_Data,1);
	
	Update_Pos();
	OLEDBright = Device_Cmd.commandbrightness;
	RGBBright = Device_Cmd.commandrgbbrightness;
	VFD_Bright = Device_Cmd.commandbrightness;
	OLEDBk = Device_Cmd.commandset&0x8;
//	printf("Init OK\r\n");
	if(Device_Cmd.commandset&0x2)
	{
		OLED_Boot();
	}
	SystemActive = True;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
	{
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		switch(Display_Mode)
		{
//#if GEMINI == 1
			case MODE_DEFALUT:DATE_Mode();break;
			case MODE_DATE:DATE_Mode();break;
//#else
//			case MODE_DEFALUT:TIME_Mode();break;
//			case MODE_DATE:TIME_Mode();break;
//#endif
			case MODE_NORMAL:NORMAL_Mode();break;
			case MODE_GAME:GAME_Mode();break;
			case MODE_OFFLINE:DATE_Mode();break;
			case MODE_SLEEP:SLEEP_Mode();break;
			case MODE_MUSIC:MUSIC_Mode();break;
			case MODE_AIWORK:AIWORK_Mode();break;
			default:Display_Mode = MODE_DEFALUT;break;
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void OLED_Boot(void)
{
#if GEMINI == 1
	
	int i;
	for(i=0;i<56;i++)
	{
		SPI_Flash_Read(BootGRAM,i*304*48+RAB_FLASH,304*48);   
		OLED_Clear();
		OLED_Full(BootGRAM);
		OLED_Refresh_Gram();
		HAL_Delay(20);
	}
	HAL_Delay(100);
	for(i=0;i<96;i+=8)
	{
		OLED_Clear();
		OLED_Full(BootGRAM);
		OLED_Refresh_Gram();
	}
#elif MARKC == 1
//	int i;
//	for(i=0;i<90;i++)
//	{
//		SPI_Flash_Read(BootGRAM,i*304*12+ROG_FLASH,304*12);   
//		OLED_Clear();
//		OLED_Convert(0,4,152,BootGRAM);
//		OLED_Refresh_Gram();
//	}
//	HAL_Delay(100);
//	for(i=4;i<96;i+=8)
//	{
//		OLED_Clear();
//		OLED_Convert(0,i,152,BootGRAM);
//		OLED_Refresh_Gram();
//	}
	int i;
	for(i=137;i>=1;i--)
	{
		SPI_Flash_Read(BootGRAM,i*256*32+FML_FLASH,256*32);   
		OLED_Clear();
		OLED_25664(BootGRAM);
		OLED_Refresh_Gram();
//		HAL_Delay(20);
	}
	OLED_Clear();
	OLED_Refresh_Gram();
	HAL_Delay(200);
#endif
}

u8 flow_vfdpot[82];
float flow_vfdfall[82];

void FFT_VFDDisplay(void)
{
	
#if GEMINI == 1
	uint16_t i = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < 82; i++)	
	{
		Index = (float)i*180/82;
		if(Device_Msg.fft[Index]/15>=15)
			Temp = 0;
		else
			Temp = 15 - Device_Msg.fft[Index]/15;
//		else if(Temp<4) Temp = 4;
		
		if(flow_vfdpot[i]+2 < Temp)
			flow_vfdpot[i] += 2;
		else if(flow_vfdpot[i]+1 < Temp)
			flow_vfdpot[i] += 1;
		
		if(flow_vfdpot[i] > (Temp+2))
			flow_vfdpot[i] -= 2;
		else if(flow_vfdpot[i] > Temp+1)
			flow_vfdpot[i] -= 1;
		
		if(flow_vfdfall[i]+1 > flow_vfdpot[i]) 
		{
			flow_vfdfall[i] = flow_vfdpot[i]-1;
		}
		else if(flow_vfdfall[i]+1 < flow_vfdpot[i]) 
		{
			flow_vfdfall[i] +=0.2;
		}
		GUI_VFD_RLine(i,flow_vfdpot[i],15,1);
		GUI_VFD_Point(i,flow_vfdfall[i],1);
	}
#elif MARKC == 1
	uint16_t i = 0;
	uint16_t j = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < 7; i++)	
	{
		Index = (float)i*180/7;
		if(Device_Msg.fft[Index]/20>7)
			Temp = 0;
		else
			Temp = 7 - Device_Msg.fft[Index]/20;
//		else if(Temp<4) Temp = 4;
		
		if(flow_vfdpot[i]+1 < Temp)
			flow_vfdpot[i] += 1;
		
		if(flow_vfdpot[i] > Temp+1)
			flow_vfdpot[i] -= 1;
//		
		for(j=0;j<5;j++)
			GUI_VFD_RLine(i*7,flow_vfdpot[i],7,1);
//		GUI_VFD_Point(i,flow_vfdpot[i],1);
	}
#endif
}


void FFTS_VFDDisplay(void)
{
#if GEMINI == 1
	uint16_t i = 0;
	uint16_t j = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < 12; i++)	
	{
		Index = (float)i*180/12;
		if(Device_Msg.fft[Index]/15>=15)
			Temp = 0;
		else
			Temp = 15 - Device_Msg.fft[Index]/15;
//		else if(Temp<4) Temp = 4;
		
		if(flow_vfdpot[i]+2 < Temp)
			flow_vfdpot[i] += 2;
		else if(flow_vfdpot[i]+1 < Temp)
			flow_vfdpot[i] += 1;
		
		if(flow_vfdpot[i] > (Temp+2))
			flow_vfdpot[i] -= 2;
		else if(flow_vfdpot[i] > Temp+1)
			flow_vfdpot[i] -= 1;
		
		if(flow_vfdfall[i]+1 > flow_vfdpot[i]) 
		{
			flow_vfdfall[i] = flow_vfdpot[i]+1;
		}
		else if(flow_vfdfall[i]+1 < flow_vfdpot[i]) 
		{
			flow_vfdfall[i] +=0.2;
		}
		for(j=0;j<5;j++)
			GUI_VFD_RLine(i*7+j,flow_vfdpot[i],15,1);
		for(j=0;j<5;j++)
			GUI_VFD_Point(i*7+j,flow_vfdfall[i],1);
	}
	
#elif MARKC == 1
	uint16_t i = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < 47; i++)	
	{
		Index = (float)i*180/47;
		if(Device_Msg.fft[Index]/20>8)
			Temp = 0;
		else
			Temp = 7 - Device_Msg.fft[Index]/20;
//		else if(Temp<4) Temp = 4;
		
		if(flow_vfdpot[i]+1 < Temp)
			flow_vfdpot[i] += 1;
		
		if(flow_vfdpot[i] > Temp+1)
			flow_vfdpot[i] -= 1;
//		
		GUI_VFD_RLine(i,flow_vfdpot[i],7,1);
//		GUI_VFD_Point(i,flow_vfdpot[i],1);
	}
#endif
}

//void FFTD_VFDDisplay(void)
//{
//	
//#if GEMINI == 1
//	uint16_t i = 0;
//	int Temp = 0;
//	uint16_t Index;
//	/*******************显示*******************/
//	for(i = 0; i < 82; i++)	
//	{
//		Index = (float)i*180/82;
//		if(Device_Msg.fft[Index]/15>=15)
//			Temp = 0;
//		else
//			Temp = 15 - Device_Msg.fft[Index]/15;
////		else if(Temp<4) Temp = 4;
//		
//		if(flow_vfdpot[i]+2 < Temp)
//			flow_vfdpot[i] += 2;
//		else if(flow_vfdpot[i]+1 < Temp)
//			flow_vfdpot[i] += 1;
//		if(flow_vfdpot[i] > (Temp+2))
//			flow_vfdpot[i] -= 2;
//		else if(flow_vfdpot[i] > Temp+1)
//			flow_vfdpot[i] -= 1;
//		
//		GUI_VFD_RLine(i,flow_vfdpot[i],15,1);
//		GUI_VFD_RLine(81-i,14-flow_vfdpot[i],0,1);
//		
//		GUI_VFD_RLine(0,0,15,1);
//		GUI_VFD_RLine(81,0,15,1);
//		
//	}
//#else
//	uint16_t i = 0;
//	uint16_t j = 0;
//	int Temp = 0;
//	uint16_t Index;
//	/*******************显示*******************/
//	for(i = 0; i < 7; i++)	
//	{
//		Index = (float)i*180/7;
//		if(Device_Msg.fft[Index]/20>7)
//			Temp = 0;
//		else
//			Temp = 7 - Device_Msg.fft[Index]/20;
////		else if(Temp<4) Temp = 4;
//		
//		if(flow_vfdpot[i]+1 < Temp)
//			flow_vfdpot[i] += 1;
//		
//		if(flow_vfdpot[i] > Temp+1)
//			flow_vfdpot[i] -= 1;
////		
//		for(j=0;j<5;j++)
//			GUI_VFD_RLine(i*7,flow_vfdpot[i],7,1);
////		GUI_VFD_Point(i,flow_vfdpot[i],1);
//	}
//#endif
//}



void AIWORK_MainDisplay(void)
{
	int i;
	static u8 DirectionFalg=False;
	if(DirectionFalg)
	{
		for(i=10;i>=0;i--)
		{
			SystemActive = False;
			SPI_Flash_Read(BootGRAM,960000+i*80*40,80*40);   
			OLED_Clear();
			OLED_Clar(5,0,40,80,BootGRAM);
//			printf("Index:%d",i);
			OLED_SF12x24(45,40,"AT YOU SEVICE");
			OLED_Refresh_Gram();
			SystemActive = True;
		}
		DirectionFalg = False;
	}
	else
	{
		for(i=0;i<11;i++)
		{
			SystemActive = False;
			SPI_Flash_Read(BootGRAM,960000+i*80*40,80*40);   
			OLED_Clear();
			OLED_Clar(5,0,40,80,BootGRAM);
//			printf("Index:%d",i);
			OLED_SF12x24(45,40,"AT YOU SEVICE");
			OLED_Refresh_Gram();
			SystemActive = True;
		}
		DirectionFalg = True;
		
	}

}
void AIWORKUI_Init(void)
{
	int i;
	for(i=119;i>=0;i--)
	{
		SystemActive = False;
		if(i==28)
			i=26;
		SPI_Flash_Read(BootGRAM,960000+i*80*40,80*40);   
		OLED_Clear();
		OLED_Clar(5,0,40,80,BootGRAM);
//		printf("Index:%d",i);
		OLED_SF12x24(45,40,"AT YOU SEVICE");
		OLED_Refresh_Gram();
		SystemActive = True;
	}

}
void AIWORKUI_Out(void)
{

}
	
void AIWORK_Mode(void)
{
	AIWORKUI_Init();
	while(Display_Mode != MODE_MUSIC)
	{
		AIWORK_MainDisplay();
	}
}


u8 MovRight[] = {0xFF,0xFF,0x40,0x00};
u8 MovLeft[] = {0x00,0x40,0xFF,0xFF};

void VFD_Load()
{
	static u8 Flowleft;
	if(Display_Mode == MODE_MUSIC)
	{
		if(Flowleft < Device_Msg.leftvol*90/65535)
			Flowleft += 1;
		else if(Flowleft > Device_Msg.leftvol*90/65535)
			Flowleft -= 1;
		if(Flowleft>47) Flowleft = 47;
		
		GUI_VFD_RectangleFill(0,1,Flowleft+1,5,1);	
	}
}

char VFDTip[12];
u16 VFDTipCount = 0;

void VFD_Display(void)
{
#if GEMINI == 1
	static u8 Flowleft,FlowRight;
	static u16 Step = 0;
	static u16 MotionStep[3] = {0};
	Step++;
	if(Display_Mode == MODE_MUSIC)
	{
		if(Flowleft < Device_Msg.leftvol*70/65535)
			Flowleft += 1;
		else if(Flowleft > Device_Msg.leftvol*70/65535)
			Flowleft -= 1;
		
		if(FlowRight < Device_Msg.rightvol*70/65535)
			FlowRight += 1;
		else if(FlowRight > Device_Msg.rightvol*70/65535)
			FlowRight -= 1;
			
//		GUI_VFD_HLine(0,0,2,1);
//		GUI_VFD_HLine(79,0,81,1);
		MotionStep[1] = Step%60;
		if(MotionStep[1]<=14)
		{
			GUI_VFD_RLine(1,0,MotionStep[1],1);
			GUI_VFD_RLine(0,0,MotionStep[1],1);
			
			GUI_VFD_RLine(80,0,MotionStep[1],1);
			GUI_VFD_RLine(81,0,MotionStep[1],1);
		}
		else if(MotionStep[1]<=60-7)
		{
			GUI_VFD_RLine(1,0,14,1);
			GUI_VFD_RLine(0,0,14,1);
			
			GUI_VFD_RLine(80,0,14,1);
			GUI_VFD_RLine(81,0,14,1);
			GUI_VFD_HLine(0,14,2,1);
			GUI_VFD_HLine(79,14,81,1);
		}
		else
		{
			
			GUI_VFD_RLine(1,14-(60-MotionStep[1])*2,14,1);
			GUI_VFD_RLine(0,14-(60-MotionStep[1])*2,14,1);
			
			GUI_VFD_RLine(81,14-(60-MotionStep[1])*2,14,1);
			GUI_VFD_RLine(80,14-(60-MotionStep[1])*2,14,1);
			GUI_VFD_HLine(0,14,2,1);
			GUI_VFD_HLine(79,14,81,1);
		}
		
		
		GUI_VFD_RectangleFill(3,1,Flowleft+3,5,1);	
		GUI_VFD_RectangleFill(78,1,81-FlowRight-3,5,1);	
	}
	MotionStep[2] = Step%60;
	if(MotionStep[2]<10)
	{
		VFD_Part(MotionStep[2],1,4,1,MovRight);
		VFD_Part(81-MotionStep[2]-3,1,4,1,MovLeft);
	}
	else if(MotionStep[2]<30)
	{
//		if(MotionStep[2]/6%2)
		{
			VFD_Part(9,1,4,1,MovRight);
			VFD_Part(81-9-3,1,4,1,MovLeft);
		}
	}
	else if(MotionStep[2]<35)
	{
		VFD_Part(9-(MotionStep[2]-30)*2,1,4,1,MovRight);
		VFD_Part(81-(9-(MotionStep[2]-30)*2)-3,1,4,1,MovLeft);
	}
		
//	HAL_Delay(100);
	Motion_Blin();
	if(Display_Mode != MODE_MUSIC)
	{
		VFD_Bow(0,5,7,2,1);
		VFD_Bow(77,5,7,2,2);
	}
	if(VFDTipCount>0&&Display_Mode != MODE_MUSIC)
	{
		VFDTipCount --;
		
		if(VFDTipCount>42)
			VFD_SF5x7((11-cont_str(VFDTip)%12)/2*7,0,VFDTip,True);
		else
			VFD_SF5x7(0-(85-VFDTipCount*2)+(11-cont_str(VFDTip)%12)/2*7,0,VFDTip,False);
	}
	else if(Display_Mode != MODE_MUSIC)
	{
		sprintf(VFDTip,"%s-%s %s",ds3231.Monm,ds3231.Day,ds3231.Weekm);
		VFD_SF5x7((12-cont_str(VFDTip)%12)/2*7,0,VFDTip,True);
	}
	VFD_SF5x7(2*7,1,ds3231.Time,True);
#elif MARKC == 1
	switch(Display_Mode)
	{
		case MODE_DEFALUT:
		VFD_SF5x7(7,0,ds3231.Hour,True);
		VFD_SF5x7(21,0,ds3231.Min,True);
		VFD_SF5x7(35,0,ds3231.Sec,True);
		VFD_PNTTIME();
		VFD_Bow(0,5,7,2,0);break;
		case MODE_DATE:
		VFD_SF5x7(7,0,ds3231.Hour,True);
		VFD_SF5x7(21,0,ds3231.Min,True);
		VFD_SF5x7(35,0,ds3231.Sec,True);
		VFD_PNTTIME();
		VFD_Bow(0,5,7,2,0);break;
		case MODE_NORMAL:VFD_SF5x7(0,0,"CPU",True); VFD_SF5x7(7*3,0,Device_Str.vcputemp,True);VFD_PNTMSG();break;
		case MODE_GAME:VFD_SF5x7(0,0,"GPU",True); VFD_SF5x7(7*3,0,Device_Str.vgputemp,True);VFD_PNTMSG();break;
		case MODE_OFFLINE:
		VFD_SF5x7(7,0,ds3231.Hour,True);
		VFD_SF5x7(21,0,ds3231.Min,True);
		VFD_SF5x7(35,0,ds3231.Sec,True);
		VFD_PNTTIME();
		VFD_Bow(0,5,7,2,0);break;
		case MODE_SLEEP:VFD_Bow(0,47,5,5,1);break;
		case MODE_MUSIC:VFD_Load();VFD_PNTCls();VFD_Bow(0,47,5,5,1);break;
		default:Display_Mode = MODE_DEFALUT;break;
	}
#endif
}

void VFD_MainDis(void)
{
	static u8 Step;
	VFD_Clear();
	VFD_Display();
	if(!VFDDisFlag)
	{
		if(Step<=38)
		{
			GUI_VFD_RectangleFill(81+Delt*7,0,Step+Delt*7,15,0);	
			GUI_VFD_RectangleFill(0+Step+Delt*7,0,1+Step+Delt*7,15,1);	
			
			GUI_VFD_RectangleFill(0,0,38-Step,15,0);	
			GUI_VFD_RectangleFill(38-Step,0,38-Step+1,15,1);	
			VFD_PNTCls();
			Step++;
		}
	}
	else if(Step>0)
	{
		GUI_VFD_RectangleFill(81+Delt*7,0,Step+Delt*7,15,0);	
		GUI_VFD_RectangleFill(0+Step+Delt*7,0,1+Step+Delt*7,15,1);	
		
		GUI_VFD_RectangleFill(0,0,38-Step,15,0);	
		GUI_VFD_RectangleFill(38-Step,0,38-Step+1,15,1);	
		VFD_PNTCls();
		Step--;
	}
	if((VFD_Bright_Con>VFD_Bright+5))
	{
		VFD_Bright_Con -=5 ;
		VFD_Brightness(VFD_Bright_Con);
	}
	else if(VFD_Bright_Con+5<VFD_Bright)
	{
		VFD_Bright_Con +=5;
		VFD_Brightness(VFD_Bright_Con);
	}
	VFD_Refresh_Vram();
}


void Display_Style3(void)
{
	uint16_t i = 0;
	uint16_t j = 0;
	int Temp = 0;
	uint16_t Index;
	static u8 BackFlag[100]={0};
	/*******************显示*******************/
	for(i = 0; i < 76; i++)	
	{
		Index = (float)i*180/TrumNum;
		Temp = TrumHeight - (u16)(Device_Msg.fft[Index]/2);
		if(Temp < 4)
			Temp = 4;
		if(flow_pot[i]+6 < Temp)
			flow_pot[i] += 4;
		else if(flow_pot[i]+2 < Temp)
			flow_pot[i] += 2;
		else if(flow_pot[i] < Temp)
			flow_pot[i] += 1;
		
		if(flow_pot[i] > (Temp+6))
			flow_pot[i] -= 4;
		else if(flow_pot[i] > (Temp+2))
			flow_pot[i] -= 2;
		else if(flow_pot[i] > Temp)
			flow_pot[i] -= 1;
		
		if(fall_pot[i]+3 > flow_pot[i]) 
		{
			fall_pot[i] = flow_pot[i]-3;
			BackFlag[i] = 0;
		}
		else if(fall_pot[i]+3 < flow_pot[i]) 
		{
			if(!BackFlag[i])
			{
				if(fall_pot[i]>=4)
					fall_pot[i] -= 3;
				else if(fall_pot[i])
					BackFlag[i] = 1;
			}
			else
				fall_pot[i] += 2;
		}
		
		GUI_RLine(4*i+0,fall_pot[i],fall_pot[i]+1,4);
		GUI_RLine(4*i+1,fall_pot[i],fall_pot[i]+1,8);
		GUI_RLine(4*i+2,fall_pot[i],fall_pot[i]+1,4);
		
		GUI_RLine(4*i+0,flow_pot[i],TrumHeight-1,10);
		GUI_RLine(4*i+1,flow_pot[i],TrumHeight-1,15);
		GUI_RLine(4*i+2,flow_pot[i],TrumHeight-1,10);	
		for(j=TrumHeight-1;j>=flow_pot[i];j-=2)
			GUI_HLine(4*i+0,j,4*i+2,0);
	}
}


void Display_Style2(void)
{
	uint16_t i = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < 102; i++)	
	{
		Index = (float)i*180/100;
		Temp = 72 - (u16)(Device_Msg.fft[Index]/3);
		if(Temp < 2)
			Temp = 2;
//		else if(Temp<4) Temp = 4;
		
		if(flow_pot[i]+6 < Temp)
			flow_pot[i] += 4;
		else if(flow_pot[i]+2 < Temp)
			flow_pot[i] += 2;
		else if(flow_pot[i]+1 < Temp)
			flow_pot[i] += 1;
		
		if(flow_pot[i] > (Temp+6))
			flow_pot[i] -= 4;
		else if(flow_pot[i] > (Temp+2))
			flow_pot[i] -= 2;
		else if(flow_pot[i] > Temp+1)
			flow_pot[i] -= 1;
		
		if(fall_pot[i]+3 > flow_pot[i]) 
		{
			fall_pot[i] = flow_pot[i]-3;
		}
		else if(fall_pot[i]+3 < flow_pot[i]) 
		{
			fall_pot[i] ++;
		}
		GUI_RLine(3*i,flow_pot[i],71,15);
		GUI_RLine(3*i+1,flow_pot[i],71,15);
		GUI_RLine(3*i,(72-flow_pot[i])/3+71,71,5);
		GUI_RLine(3*i+1,(72-flow_pot[i])/3+71,71,5);
		
		GUI_RLine(3*i,fall_pot[i]+1,fall_pot[i]+1,6);
		GUI_RLine(3*i+1,fall_pot[i]+1,fall_pot[i]+1,6);
	}
}

void Display_Style1(void)
{
	uint16_t i = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < TrumNum; i++)	
	{
		Index = (float)i*180/TrumNum;
		Temp = TrumHeight - (u16)(Device_Msg.fft[Index]/2);
		if(Temp < 2)
			Temp = 2;
//		else if(Temp<4) Temp = 4;
		
		if(flow_pot[i]+6 < Temp)
			flow_pot[i] += 4;
		else if(flow_pot[i]+2 < Temp)
			flow_pot[i] += 2;
		else if(flow_pot[i]+1 < Temp)
			flow_pot[i] += 1;
		
		if(flow_pot[i] > (Temp+6))
			flow_pot[i] -= 4;
		else if(flow_pot[i] > (Temp+2))
			flow_pot[i] -= 2;
		else if(flow_pot[i] > Temp+1)
			flow_pot[i] -= 1;
		
		if(fall_pot[i]+3 > flow_pot[i]) 
		{
			fall_pot[i] = flow_pot[i]-3;
		}
		else if(fall_pot[i]+3 < flow_pot[i]) 
		{
			fall_pot[i] ++;
		}
		GUI_RLine((TrumWidth+TrumInterval)*i,flow_pot[i],TrumHeight-1,15);
		GUI_RLine((TrumWidth+TrumInterval)*i,fall_pot[i]+1,fall_pot[i]+1,6);
	}
}

u16 SampPoint[4][192*2];

void Display_Style4()
{

	uint16_t i = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < 102; i++)	
	{
		Index = (float)i*180/100;
		Temp = TrumHeight - (u16)(Device_Msg.fft[Index]/2);
		if(Temp < 2)
			Temp = 2;
		if(flow_pot[i]+10 < Temp)
			flow_pot[i] += 4;
		else if(flow_pot[i]+3 < Temp)
			flow_pot[i] += 2;
		else if(flow_pot[i] < Temp)
			flow_pot[i] += 1;
		if(flow_pot[i] > (Temp+10))
			flow_pot[i] -= 4;
		else if(flow_pot[i] > (Temp+3))
			flow_pot[i] -= 2;
		else if(flow_pot[i] > Temp)
			flow_pot[i] -= 1;
		
//		GUI_Line(3*i,flow_pot[i],3*i+3,flow_pot[i+1],13);
		SampPoint[0][i*2] = 3*i;
		SampPoint[0][i*2+1] = flow_pot[i];
		GUI_RLine(3*i,flow_pot[i],TrumHeight-1,3);
		SampPoint[1][i*2] = 3*i;
		SampPoint[1][i*2+1] = flow_pot[i]-1;
	}
	GUI_LineS(SampPoint[0],102,13);
	GUI_LineS(SampPoint[1],102,9);
}

void Display_Style6()
{
	uint16_t i = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < 152; i++)	
	{
		Index = (float)i*180/152;
		Temp = 48 - (u16)(Device_Msg.fft[Index]/4);
		if(Temp < 2)
			Temp = 2;
		if(flow_pot[i]+10 < Temp)
			flow_pot[i] += 4;
		else if(flow_pot[i]+3 < Temp)
			flow_pot[i] += 2;
		else if(flow_pot[i] < Temp)
			flow_pot[i] += 1;
		if(flow_pot[i] > (Temp+10))
			flow_pot[i] -= 4;
		else if(flow_pot[i] > (Temp+3))
			flow_pot[i] -= 2;
		else if(flow_pot[i] > Temp)
			flow_pot[i] -= 1;
		
//		GUI_Line(3*i,flow_pot[i],3*i+3,flow_pot[i+1],13);
		if(i%2)
		{
			SampPoint[0][i*2] = 2*i;
			SampPoint[0][i*2+1] = flow_pot[i];
			SampPoint[1][i*2] = 2*i;
			SampPoint[1][i*2+1] = flow_pot[i]-1;
			
			SampPoint[2][i/2*2] = 2*i;
			SampPoint[2][i/2*2+1] = flow_pot[i];
			
		}
		else
		{
			SampPoint[0][i*2] = 2*i;
			SampPoint[0][i*2+1] = - flow_pot[i] + 96;
			SampPoint[1][i*2] = 2*i;
			SampPoint[1][i*2+1] = - flow_pot[i] + 96-1;
			
			SampPoint[3][i/2*2] = 2*i;
			SampPoint[3][i/2*2+1] = - flow_pot[i] + 96;
		}
			
		
//		GUI_RLine(3*i,SampPoint[0][i*2+1],SampPoint[1][i*2+1],5);
		
	}
	GUI_LineS(SampPoint[2],76,5);
	GUI_LineS(SampPoint[3],76,5);
	
	GUI_LineS(SampPoint[0],152,13);
	GUI_LineS(SampPoint[1],152,13);
}

void Display_Style5()
{

	uint16_t i = 0;
	int Temp = 0;
	uint16_t Index;
	/*******************显示*******************/
	for(i = 0; i < 102; i++)	
	{
		Index = (float)i*180/100;
		Temp = 48 - (u16)(Device_Msg.fft[Index]/4);
		if(Temp < 2)
			Temp = 2;
		if(flow_pot[i]+10 < Temp)
			flow_pot[i] += 4;
		else if(flow_pot[i]+3 < Temp)
			flow_pot[i] += 2;
		else if(flow_pot[i] < Temp)
			flow_pot[i] += 1;
		if(flow_pot[i] > (Temp+10))
			flow_pot[i] -= 4;
		else if(flow_pot[i] > (Temp+3))
			flow_pot[i] -= 2;
		else if(flow_pot[i] > Temp)
			flow_pot[i] -= 1;
		
//		GUI_Line(3*i,flow_pot[i],3*i+3,flow_pot[i+1],13);
		SampPoint[0][i*2] = 3*i;
		SampPoint[0][i*2+1] = flow_pot[i];
		SampPoint[1][i*2] = 3*i;
		SampPoint[1][i*2+1] = - flow_pot[i] + 96;
		
		GUI_RLine(3*i,SampPoint[0][i*2+1],SampPoint[1][i*2+1],5);
		
	}
	GUI_LineS(SampPoint[0],102,13);
	GUI_LineS(SampPoint[1],102,13);
}

void MUSICUI_Init()
{
	u8 i;
	for(i=0;i<TrumNum;i++)
	{
		fall_pot[i] = 95;
		flow_pot[i] = 95;
	}
	
	Flag_Reception = False;
	ClearFFT();
	for(i=0;i<15;)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			i++;
			OLED_Clear();
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			switch(Device_Cmd.commandstyle)
			{
				case 1:Display_Style1();break;
				case 2:Display_Style2();break;
				case 3:Display_Style3();break;
				case 4:Display_Style4();break;
				case 5:Display_Style5();break;
				case 6:Display_Style6();break;
				default:Display_Style1();break;
			}
			
			OLED_Refresh_Gram();
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
	}
	Flag_Reception = True;
	OLED_Clear();
}

void MUSICUI_Out()
{
	u8 i;
	
	Flag_Reception = False;
	ClearFFT();
	for(i=0;i<30;)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			i++;
			OLED_Clear();
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			switch(Device_Cmd.commandstyle)
			{
				case 1:Display_Style1();break;
				case 2:Display_Style2();break;
				case 3:Display_Style3();break;
				case 4:Display_Style4();break;
				case 5:Display_Style5();break;
				case 6:Display_Style6();break;
				default:Display_Style1();break;
			}
			
			OLED_Refresh_Gram();
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
	}
	Flag_Reception = True;
	OLED_Clear();
}



void MUSIC_Mode(void)
{
	MUSICUI_Init();
	while(Display_Mode == MODE_MUSIC)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			if(Flag_Sleep)
			{
				Flag_Sleep = False;
				Display_Mode = MODE_SLEEP;
				break;
			}
			OLED_Clear();
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			switch(Device_Cmd.commandstyle)
			{
				case 1:Display_Style1();break;
				case 2:Display_Style2();break;
				case 3:Display_Style3();break;
				case 4:Display_Style4();break;
				case 5:Display_Style5();break;
				case 6:Display_Style6();break;
				default:Display_Style1();break;
			}
			
			OLED_Refresh_Gram();
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
	}
	MUSICUI_Out();
}

void Judge_Notification(void)
{
//	if(!Clock_Msg.aiwork)
//	{
		sprintf(Display_Notification,"%5s",ds3231.Time);
		if(Flag_Blink)
			Display_Notification[2] = ' '; 
//	}
//	else if(Clock_Msg.aiwork)
//		sprintf(Display_Notification,"XIOAI");
}



typedef struct
{
	u16 MonLen ;
	u16 WeekLen ;
	u16 LineLen;
	u8 MonBtWeek;
}TIMED;

TIMED timed;

void CalcTIMED(char *Mon,char *Week)
{
	timed.MonLen = cont_str(Mon)*5;
	timed.WeekLen = cont_str(Week)*6;
	if(timed.WeekLen>timed.MonLen)
	{
		timed.MonLen = timed.WeekLen;
		timed.MonBtWeek = True;
	}
	else
		timed.MonBtWeek = False;
	timed.LineLen = (timed.MonLen+38)*2;
}

void handleGesture() 
{
    switch ( readGesture() ) 
		{
      case DIR_UP:
        printf("UP\n");
        break;
      case DIR_DOWN:
        printf("DOWN\n");
        break;
      case DIR_LEFT:
        printf("LEFT\n");
        break;
      case DIR_RIGHT:
        printf("RIGHT\n");
        break;
      case DIR_NEAR:
        printf("NEAR\n");
        break;
      case DIR_FAR:
        printf("FAR\n");
        break;
      default:
        printf("NONE\n");
    }
}

u8 VoiceFlag = True;
u8 Disco;
void DATE_MainDisplay()
{
//	static u8 RunCount = 0;
//	if(RunCount++>23)
//	{
//		RunCount = 0;
//		Time_Handle();
//	}
//	if(Flag_Uptime)
//	{
//		Flag_Uptime = False;
//		Time_Handle();
//	}
//		
//	if(APDSFlag)
//	{
//		Disco++;
//		HumanCount = 40;
//		APDSFlag = False;
//		handleGesture();
////		
////		SparkFun_APDS9960();
////		enableGestureSensor(true);	
//		SparkFun_APDS9960();
////		HAL_Delay(10);
////		printf("APDS-9960 complete\r\n");
////		if(SparkFun_APDS9960_init()) 
////		{
////			printf("APDS-9960 initialization complete\r\n");
////		} 
////		else  
////		{
////			printf("Something went wrong during APDS-9960 init!\r\n");
////		}   
////		HAL_Delay(10);
////	if (enableGestureSensor(true) ) 
////	{
////		printf("Gesture sensor is now running\r\n");
////	} 
////	else 
////	{
////		printf("Something went wrong during gesture sensor init!\r\n");
////	}
//	}
////		
	CalcTIMED(ds3231.Mon,ds3231.Week);
	OLED_SF22x40(RandomPosX,RandomPosY,ds3231.Day);
	if(timed.MonBtWeek)
	{
		OLED_SF12x24(RandomPosX+33,RandomPosY+15,ds3231.Week);
	}
	else
		OLED_SF12x24(RandomPosX+33+timed.MonLen - timed.WeekLen,RandomPosY+15,ds3231.Week); 
	
	OLED_SF10x16(RandomPosX,RandomPosY+47,ds3231.Mon);
	OLED_SF10x16(RandomPosX+timed.MonLen+18,RandomPosY+47,ds3231.Year);
	GUI_HLine(RandomPosX*2,RandomPosY+44,RandomPosX*2+timed.LineLen,0xF);
	
#if GEMINI == 1
	if(VoiceFlag)
	{
		SHOW_CORN.BTV = False;
		if(Flow_BTV() == OLED_IDLE)
		{
			if(BTVPostionX>50)
				Motion_VOICE(BTVPostionX-6,15);
			else
				Motion_VOICE(BTVPostionX,15);
		}
	}
	else
		Motion_WEATHER(BTVPostionX,RandomPosY+6,ConvertWeather(WiFi_Msg.weather1num));
#else
		Flow_BTV();
#endif
//		Motion_WEATHER(BTVPostionX,RandomPosY+6,Disco%7);
}

void DATEUI_Init()
{
	int i;
	CalcTIMED(ds3231.Mon,ds3231.Week);
	SHOW_CORN.BTV = True;
	for(i = 0;i<20;i++)
	{
			OLED_Clear();
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			
#if GEMINI == 1
			Motion_WEATHER(BTVPostionX,RandomPosY+6+38-i*2,ConvertWeather(WiFi_Msg.weathernum));
#else
			Flow_BTV();
			
#endif
			OLED_SF22x40(RandomPosX ,RandomPosY-40+i*2,ds3231.Day);
			if(timed.MonBtWeek)
				OLED_SF12x24(RandomPosX+33,RandomPosY+15-40+i*2,ds3231.Week);
			else
				OLED_SF12x24(RandomPosX+33+timed.MonLen - timed.WeekLen,RandomPosY+15-40+i*2,ds3231.Week); 
			OLED_SF10x16(RandomPosX,RandomPosY+47+40-i*2,ds3231.Mon);
			OLED_SF10x16(RandomPosX+timed.MonLen+18,RandomPosY+47+40-i*2,ds3231.Year);
			
			GUI_HLine(RandomPosX*2,RandomPosY+44,RandomPosX*2+(float)timed.LineLen/(20-i),0xF); //注意
		
		OLED_Refresh_Gram();
	}
}

void DATEUI_Out()
{
	int i;
	float line;
	UsartCommand(0xA001,3);//获取命令
	SHOW_CORN.BTV = False;
	Motion_MovmeteorInit();
	line = timed.LineLen;
	for(i = 19;i>=0;i--)
	{
		OLED_Clear();
		
		OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
		
#if GEMINI == 1
			Motion_WEATHER(BTVPostionX,RandomPosY+6+38-i*2,ConvertWeather(WiFi_Msg.weathernum));
#else
			Flow_BTV();
			
#endif
		
		OLED_SF22x40(RandomPosX ,RandomPosY-40+i*2,ds3231.Day);
		if(timed.MonBtWeek)
			OLED_SF12x24(RandomPosX+33,RandomPosY+15-40+i*2,ds3231.Week);
		else
			OLED_SF12x24(RandomPosX+33+timed.MonLen - timed.WeekLen,RandomPosY+15-40+i*2,ds3231.Week); 
		OLED_SF10x16(RandomPosX,RandomPosY+47+40-i*2,ds3231.Mon);
		OLED_SF10x16(RandomPosX+timed.MonLen+18,RandomPosY+47+40-i*2,ds3231.Year);
		
		GUI_HLine(RandomPosX*2,RandomPosY+44,RandomPosX*2+line/(20-i),0xF); //注意
		
		OLED_Refresh_Gram();
	}
}


void Update_Pos(void)
{
	if(BTVPostionX > 100)
	{
		BTVPostionX = rand()%4+1;
		BTVPostionY = rand()%10+20;
		RandomPosX = rand()%24 + 34;
		RandomPosY = rand()%18 + 10;
		RandomP = rand()%13;
	}
	else
	{
		BTVPostionX = rand()%4+120;
		BTVPostionY = rand()%10+20;
		RandomPosX = rand()%24 + 15;
		RandomPosY = rand()%22 + 10;
		RandomP = rand()%13;
	}
}


void DATE_Mode(void)
{
//	Update_Pos();
	DATEUI_Init();
	while(Display_Mode == MODE_DATE||Display_Mode == MODE_DEFALUT||Display_Mode == MODE_OFFLINE)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			if(Flag_Sleep)
			{
				Flag_Sleep = False;
				Display_Mode = MODE_SLEEP;
				break;
			}
			OLED_Clear();
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			DATE_MainDisplay();
			OLED_Refresh_Gram();


			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
	}
	DATEUI_Out();
}

void TIMEUI_Init()
{
	static char str[20];
	int i;sprintf(str,"%s.%4s.%s",ds3231.Year,ds3231.Mon,ds3231.Day);
	for(i = 0;i<20;i++)
	{
		OLED_Clear();
		
		OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
		
		OLED_SF32x46(RandomP+4-10+i/2,31,ds3231.Hour,'B');
		OLED_SF32x46(RandomP+48,32-40+i*2,ds3231.Min,'T');
		GUI_CircleFill(RandomP*2+84,43-40+i*2,2,15);
		GUI_CircleFill(RandomP*2+84,68+20-i,2,15);
		OLED_SF10x16(RandomP+90,39-40+i*2,ds3231.Week);
		
		OLED_SF8x16(RandomP+90+60-i*3,56,str);
		
		GUI_HLine(RandomP*2+180,56,RandomP*2+180+i*5,15);
		
		OLED_Refresh_Gram();
	}
	
}

void TIMEUI_Out()
{
	static char str[20];
	int i;sprintf(str,"%s.%4s.%s",ds3231.Year,ds3231.Mon,ds3231.Day);
	for(i = 20;i>=0;i--)
	{
		OLED_Clear();
		
		OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
		
		OLED_SF32x46(RandomP+4-10+i/2,31,ds3231.Hour,'B');
		OLED_SF32x46(RandomP+48,32-40+i*2,ds3231.Min,'T');
		GUI_CircleFill(RandomP*2+84,43-40+i*2,2,15);
		GUI_CircleFill(RandomP*2+84,68+20-i,2,15);
		OLED_SF10x16(RandomP+90,39-40+i*2,ds3231.Week);
		
		OLED_SF8x16(RandomP+90+60-i*3,56,str);
		GUI_HLine(RandomP*2+180,56,RandomP*2+180+i*5,15);
		
		OLED_Refresh_Gram();
	}
	
}
void TIME_MainDisplay(void)
{
	static char str[20];
	OLED_SF32x46(RandomP+4,31,ds3231.Hour,'B');
	OLED_SF32x46(RandomP+48,32,ds3231.Min,'T');
	GUI_CircleFill(RandomP*2+84,43,2,15);
	GUI_CircleFill(RandomP*2+84,68,2,15);
	OLED_SF10x16(RandomP+90,39,ds3231.Week);
	sprintf(str,"%s.%4s.%s",ds3231.Year,ds3231.Mon,ds3231.Day);
	OLED_SF8x16(RandomP+90,56,str);
	GUI_HLine(RandomP*2+180,56,RandomP*2+278,15);
}

void TIME_Mode(void)
{
	TIMEUI_Init();
	while(Display_Mode == MODE_DATE)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			if(Flag_Sleep)
			{
				Flag_Sleep = False;
				Display_Mode = MODE_SLEEP;
				break;
			}
			OLED_Clear();
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			TIME_MainDisplay();
			OLED_Refresh_Gram();

			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
	}
//	TIMEUI_Out();
}

void SLEEP_Mode(void)
{
	u16 i;
	for(i=0;i<1000;)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;i++;
			OLED_Clear();
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			OLED_OlRefresh_Gram();
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
	}
	OLED_Clear();
	UsartCommand(0xA002,3);
}

void OFFLINE_Mode(void)
{
	while(Display_Mode == MODE_OFFLINE)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			OLED_Clear();
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			OLED_Refresh_Gram();
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
	}
	OLED_Clear();
}

void NORMAL_MainDisplay(void)
{
//	if(Flag_Uptime)
//	{
//		Flag_Uptime = False;
//		Time_Handle();
//	}
	OLED_SF8x16(30,0,Device_Str.cputemp);
	OLED_SF8x16(30,16,Device_Str.cpuclock);
	OLED_BAR64x10(60+2,36,Device_Msg.cpuload/10,0);
	OLED_SF8x16(30,50+2,Device_Str.gputemp);
	OLED_SF8x16(30,66+2,Device_Str.gpuclock);
	OLED_BAR64x10(60+2,86+2,Device_Msg.gpuload/10,1);
	
	Flow_Coordinate(2);
	Showbar(Display_Notification);
	
	Flow_Corn();
	Flow_Line();
	Flow_USB();
	
	OLED_Part(30,33,32,10,Show_Load);
	OLED_Part(30,83+2,32,10,Show_Load);
	OLED_Part(24,0,5,16,Corn_Temp);
	OLED_Part(24,16,5,16,Corn_Freq);
	OLED_Part(24,32,5,10,Corn_Load);
	OLED_Part(24,50+2,5,16,Corn_Temp);
	OLED_Part(24,66+2,5,16,Corn_Freq);
	OLED_Part(24,82+2,5,10,Corn_Load);
#if GEMINI == 1
	if(VoiceFlag)
	{
		SHOW_CORN.ASUS = False;
		if(Flow_ASUS() == OLED_IDLE)
		{
			Motion_VOICE(119,34);
		}
	}
	else
		Flow_ASUS();
		
#else
		Flow_ASUS();
			
#endif	
}

void NORMALUI_Init(void)
{
	Notbarautoflag = True;
	Notbarrefsflag = True;
	OLED_Clear();
	SHOW_CORN.ASUS = True;
	SHOW_CORN.CPUGPU = True;
	SHOW_CORN.LINE = True;
	SHOW_CORN.USBD = True;
	Clear_FRAM();
	OLED_RefrashCoordinate(Device_Msg.cpuload/10);
	OLED_Part(30,33,32,10,Show_Load);
	OLED_Part(30,83+2,32,10,Show_Load);
	OLED_Part(24,0,5,16,Corn_Temp);
	OLED_Part(24,16,5,16,Corn_Freq);
	OLED_Part(24,32,5,10,Corn_Load);
	OLED_Part(24,50+2,5,16,Corn_Temp);
	OLED_Part(24,66+2,5,16,Corn_Freq);
	OLED_Part(24,82+2,5,10,Corn_Load);
	HAL_TIM_Base_Start_IT(&htim6);
	Flag_Reception = False;
	InitData();
	Device_Msg.cpuload = 1000;
	Device_Msg.gpuload = 1000;
	while(!Flag_Continue)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;Judge_Notification();
			OLED_Fill(0);
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			NORMAL_MainDisplay();
			OLED_Refresh_Gram();
		}
	}
	Flag_Continue = False;
	HAL_TIM_Base_Start_IT(&htim6);
	Flag_Reception = True;
	while(!Flag_Continue)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;Judge_Notification();
			OLED_Fill(0);
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			Device_Msg.cpuload = 0;
			Device_Msg.gpuload = 0;
			NORMAL_MainDisplay();
			OLED_Refresh_Gram();
		}
	}
	Flag_Continue = False;
}

void NORMALUI_Out(void)
{
	u8 i;
	SHOW_CORN.ASUS = False;
	SHOW_CORN.CPUGPU = False;
	SHOW_CORN.LINE = False;
	SHOW_CORN.USBD = False;
	sprintf(Display_Notification,"-----");
	Notbarautoflag = False;
	for(i=0;i<36;i++)
	{
//		while(!Flag_Refrash) __ASM("NOP");
		Flag_Refrash = False;
		Device_Msg.cpuload=0;
		Device_Msg.cpuload=0;
		OLED_Fill(0);
		NORMAL_MainDisplay();if(i>2)
		GUI_RectangleFill(44,0,128,95,0);
		OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
		Flow_Coordinate(0);
		OLED_Refresh_Gram();
	}
	OLED_Fill(0);
	DampValueClear();
}

void NORMAL_Mode(void)
{
	static u16 SleepTypo = 0;
	static u8 FlashFlag = 0;
	NORMALUI_Init();
	while(Display_Mode == MODE_NORMAL)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			if(!FlashFlag++%64)
				OLED_RefrashCoordinate(Device_Msg.cpuload/10);
			if(Flag_Sleep)
			{
				sprintf(Display_Notification,"Sleep");
				SleepTypo++;
				if(SleepTypo >= 250)
				{
					SleepTypo = 0;
					Flag_Sleep = False;
					Display_Mode = MODE_SLEEP;
				}
			}
			else
			{
				Judge_Notification();
			}
			OLED_Fill(0);
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			NORMAL_MainDisplay();
			OLED_Refresh_Gram();
		}
	}
	NORMALUI_Out();
}

void GAME_MainDisplay(void)
{
	SHOW_CORN.GAMEBK=True;
	OLED_SF8x16(10,40,Device_Str.ncpufan);
	OLED_SF8x16(10+76,40,Device_Str.ngpufan);
	OLED_SF10x16(2,72,Device_Str.ncpuclock);
	OLED_SF10x16(27,72,Device_Str.ngputemp);
	OLED_SF10x16(52,72,Display_State);

	OLED_BAR(74,46+2,75,8,Device_Msg.cpufan/30,1,2);
	OLED_BAR(74+152,46+2,75,8,Device_Msg.gpufan/30,1,3);

	OLED_BAR(152+2,73,146,21,Device_Msg.ramload/10,2,4);

	OLED_BAR(0+2,12,146,15,Device_Msg.cpuload/10,2,5);
	OLED_BAR(152+2,12,146,15,Device_Msg.gpuload/10,2,6);
}

void GAME_Out(void)
{
	int i;
	Flag_Reception = False;
	InitData();
	sprintf(Display_State,"%4s",Device_Str.ramusrdata);
	Device_Msg.cpuload = 0;
	Device_Msg.gpuload = 0;
	Device_Msg.cpufan = 0;
	Device_Msg.gpufan = 0;
	Device_Msg.ramload = 0;
	for(i = 300 ;i>=0;i-=6)
	{
		OLED_Fill(0);
		SportUIMt(i);
		
		if(i>100)
		{
			GAME_MainDisplay();
		}
		
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			SportUI2Mt(i);
			SportUIMt(i);
			if(i>100)
			{
				GAME_MainDisplay();
			}

			OLED_Refresh_Gram();
		}
		if(i>420&&!DampValueBusy())
			break;
	}
	DampValueClear();
	Flag_Reception = True;
}	

void GAME_Init(void)
{
	int i;
	Flag_Reception = False;
	InitData();
	Device_Msg.cpuload = 1000;
	Device_Msg.gpuload = 1000;
	Device_Msg.cpufan = 3000;
	Device_Msg.gpufan = 3000;
	Device_Msg.ramload = 1000;
	sprintf(Display_State,"%4s",Device_Str.ramusrdata);
	for(i= 0 ;i<300;i+=6)
	{
		OLED_Fill(0);
		SportUIMt(i);
		
		if(i>100)
		{
			GAME_MainDisplay();
		}
		
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			SportUIMt(i);
			SportUI2Mt(i);
			if(i>100)
			{
				GAME_MainDisplay();
			}

			OLED_Refresh_Gram();
		}
	}
	Flag_Reception = True;
}	

void GAME_Mode(void)
{
	static u16 SleepTypo = 0;
	GAME_Init();
	while(Display_Mode == MODE_GAME)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			if(Flag_Sleep)
			{
				sprintf(Display_State,"REST");
				SleepTypo++;
				if(SleepTypo >= 250)
				{
					SleepTypo = 0;
					Flag_Sleep = False;
					Display_Mode = MODE_SLEEP;
				}
			}
			else if(Device_Msg.ramusrdata<=9999)
				sprintf(Display_State,"%4s",Device_Str.ramusrdata);
			else
				sprintf(Display_State,"%1d.%1dG",Device_Msg.ramusrdata/10000,Device_Msg.ramusrdata/1000%10);
				
			OLED_Fill(0);
			OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			OLED_Full(UI_Sport);
			GAME_MainDisplay();
			OLED_Refresh_Gram();
		}
	}
	GAME_Out();
}

u8 TimeSystem_Convert(u8 Hour,u8 Sys)
{
	if(!Sys)
	{
		if(Hour == 0x12)
			return 0;
		else
			return Hour;
	}
	else
	{
		if(Hour == 0x12)
			return 0x12;
		else
			return HEX2BCD(BCD2HEX(Hour) + 12);
	}	
}

void Remote_Control(u8 Value)
{
	static u8 STAValue;
	if(Value!=STAValue)
	{
		STAValue = Value;
		HumanCount = 40;
		if(Value != IRF_PW)
			HAL_GPIO_WritePin(GPIOA,DS_PW_Pin,GPIO_PIN_RESET);
		switch(Value)
		{
			case IRF_1:Device_Cmd.commandrgbcolor = 25.5*1;ChangeCmd(Command_RGBColor);break;
			case IRF_2:Device_Cmd.commandrgbcolor = 25.5*2;ChangeCmd(Command_RGBColor);break;
			case IRF_3:Device_Cmd.commandrgbcolor = 25.5*3;ChangeCmd(Command_RGBColor);break;
			case IRF_4:Device_Cmd.commandrgbcolor = 25.5*4;ChangeCmd(Command_RGBColor);break;
			case IRF_5:Device_Cmd.commandrgbcolor = 25.5*5;ChangeCmd(Command_RGBColor);break;
			case IRF_6:Device_Cmd.commandrgbcolor = 25.5*6;ChangeCmd(Command_RGBColor);break;
			case IRF_7:Device_Cmd.commandrgbcolor = 25.5*7;ChangeCmd(Command_RGBColor);break;
			case IRF_8:Device_Cmd.commandrgbcolor = 25.5*8;ChangeCmd(Command_RGBColor);break;
			case IRF_9:Device_Cmd.commandrgbcolor = 25.5*9;ChangeCmd(Command_RGBColor);break;
			case IRF_0:Device_Cmd.commandrgbcolor = 25.5*10;ChangeCmd(Command_RGBColor);break;
			
			case IRF_NL: break;
			case IRF_CH:Device_Cmd.commandstyle++;if(Device_Cmd.commandstyle>5)Device_Cmd.commandstyle=1; ChangeCmd(Command_Style); break;
			case IRF_PC: break;
			case IRF_TV:Device_Cmd.commandmode++;if(Device_Cmd.commandmode>6)Device_Cmd.commandmode=1; ChangeCmd(Command_Mode);  break;
			
			case IRF_LF: break;
			case IRF_RT: break;
			case IRF_UP:if(Device_Cmd.commandbrightness<200) Device_Cmd.commandbrightness+=2;
			VFDTipCount = 300;sprintf(VFDTip,"BRT - %d",Device_Cmd.commandbrightness/2);SaveFlag = True; ChangeCmd(Command_Brightness);break;
			case IRF_DN:if(Device_Cmd.commandbrightness>20) Device_Cmd.commandbrightness-=2;
			VFDTipCount = 300;sprintf(VFDTip,"BRT - %d",Device_Cmd.commandbrightness/2);SaveFlag = True; ChangeCmd(Command_Brightness);break;
			case IRF_OK: break;
			
			case IRF_MT: break;
			case IRF_IF: break;
			case IRF_PIP: break;
			case IRF_SLP: break;
			case IRF_WID: break;
			
			case IRF_PW:if(HumanCount!=0) HumanCount = 0; HAL_GPIO_TogglePin(GPIOA,DS_PW_Pin);break;
		}
	}
//	printf("RemoteValue:%X\r\n",Value);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static u16 Temp=0;
	static u16 SleepCount = 0;
//	static u8 U3WatchDogCount = 0;
	static u16 ContinueCount = 0;
//	static u16 Runcount = 0;
	static u16 FixTimeCount = 0;
	static u8 RunC = 0;
	static u8 RunB = 0;
	static CLOCK_MSG Mi_Msg;
	if (htim->Instance == htim4.Instance)
	{
		Flag_Refrash = True;
//		Runcount++;
//		if(Runcount%5)
		{
//			if(Display_Mode!=MODE_OFFLINE)
			{
				switch(Device_Cmd.commandrgbmode)
				{
					case 1:
					switch(Display_Mode)
					{
						case MODE_DEFALUT:SK6812_WheelAll(Device_Cmd.commandrgbcolor);break;
						case MODE_DATE:SK6812_WheelAll(Device_Cmd.commandrgbcolor);break;
						case MODE_NORMAL:SK6812_Wheel(1,Temp+=2);break;
						case MODE_GAME:SK6812_WheelS(1,Temp+=3);break;
						case MODE_MUSIC:SK6812_WheelS(1,Temp+=6);break;
						default:SK6812_Wheel(1,Temp+=2);break;
					}break;
					case 2:SK6812_WheelS(1,Temp+=6);break;
					case 3:SK6812_WheelAll(Device_Cmd.commandrgbcolor);break;
					default:SK6812_WheelAll(Device_Cmd.commandrgbcolor);break;
				}
			}
//			else
//				SK6812_WheelAll(Device_Cmd.commandrgbcolor);
		}
		SK6812_Flow();
		
	}
	if(htim->Instance==TIM5)
	{
#if GEMINI == 1
 		if(RmtSta&0x80)//上次有数据被接收到了
		{	
//			RemoteFlag = True;
			RmtSta&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);//清空引导标识
				RmtSta&=0XF0;	//清空计数器	
//				RemoteFlag = False;
			}						 	   	
		}	
#endif 
		if(Display_Mode!=MODE_MUSIC)
			SK6812_Refrash();
		else
		{
			RunB++;
			if(RunB%8 == 0)
				SK6812_Refrash();
		}
	}
	if (htim->Instance == htim6.Instance)
	{
		if(ContinueCount++>20)
		{
			Flag_Continue = True;
			ContinueCount = 0;
			HAL_TIM_Base_Stop_IT(&htim6);
		}
	}
	if (htim->Instance == htim8.Instance)
	{
		VFD_MainDis();
//		if(RemoteFlag)
#if GEMINI == 1
		{
			Remote_Control(Remote_Scan());
		}
#endif 
	}
	if (htim->Instance == htim7.Instance)
	{
		Time_Handle();
//	HAL_ADC_Start(&hadc1);
//	HAL_ADC_PollForConversion(&hadc1, 50);
//	printf("Light:%X\r\n",HAL_ADC_GetValue(&hadc1));
//		U3WatchDogCount++;
		if(SystemActive)
		{
			if(OfflineCount<1)
			{ 
				OLEDBright = Device_Cmd.commandbrightness;
				RGBBright = Device_Cmd.commandrgbbrightness;
				VFD_Bright = Device_Cmd.commandbrightness;
				OLEDBk = Device_Cmd.commandset&0x8;
				OfflineCount++;
			}
			else if(OfflineCount<2)
			{
				ClearFFT();
				OfflineCount++;
			}
			else if(OfflineCount <10)
			{
				InitData();
				OfflineCount++;
			}
			else if(OfflineCount == 10)
			{
#if GEMINI == 1
				if(HumanCount==0)
				{
					RGBBright = 0;
					OLEDBright = 0;
					VFD_Bright = 2;
					OLEDBk = False;
				}
#else
					RGBBright = 25;
					OLEDBright = 2;
					VFD_Bright = 0;
					OLEDBk = False;
				
#endif 
				Display_Mode = MODE_OFFLINE;
			}
#if GEMINI == 1
			if(Clock_Msg.aiwork)
			{
				HumanCount = 40;
			}
			if(HumanCount>0)
			{
				HumanCount--;
				HAL_GPIO_WritePin(GPIOA,ESP_PW_Pin,GPIO_PIN_RESET);
				OLEDBright = Device_Cmd.commandbrightness;
				RGBBright = Device_Cmd.commandrgbbrightness;
				VFD_Bright = Device_Cmd.commandbrightness;
				OLEDBk = Device_Cmd.commandset&0x8;
			}
			else
			{
				HAL_GPIO_WritePin(GPIOA,ESP_PW_Pin,GPIO_PIN_SET);
			}
#endif 
		}
		Flag_Blink = (~Flag_Blink)&1;
		if(RespFlag)
		{
#if GEMINI == 1
			UsartPrint("Gemini is online\n");
#elif MARKC == 1
			UsartPrint("Mark_C is online\n");
#endif
			RespFlag = False;
		}
//		Flag_Uptime = True;
		if(Uart_Overflow1_Flag)
		{
//			printf("FixTimeCount:%d\r\n",FixTimeCount);
			if(SetTimeFlag)
			if((FixTimeCount++%3600==10)||(DS3231_US_Buf[6]<0x18))
			{
				DS3231_SetUart();
				SetTimeFlag = False;
			}
			Uart_Overflow1_Flag = False;
		}
		
#if GEMINI == 1
		if(Uart_Overflow2_Flag)
		{
			RunC = 1-RunC;
			VFD_Corn(MESSG,RunC);
//			printf("FixTimeCount:%d\r\n",FixTimeCount);
			if(SetTimeFlag)
			if((FixTimeCount++%3600==10)||(DS3231_US_Buf[6]<0x18))
			{
				DS3231_SetWiFi();
				SetTimeFlag = False;
//			printf("SetOK\r\n");
			}
//			printf("今日天气:%s\r\n",WiFi_Msg.weather);
//			printf("未来一天:%s\r\n",WiFi_Msg.week1);
//			printf("天气:%s\r\n",WiFi_Msg.weather1);
//			printf("白天温度:%s\r\n",WiFi_Msg.temp1);
//			printf("夜晚温度:%s\r\n",WiFi_Msg.ntemp1);
//			printf("日期 %02d-%02d-%02d-%02d\r\n",WiFi_Msg.year,WiFi_Msg.month,WiFi_Msg.day,WiFi_Msg.week);
//			printf("时间 %02d:%02d:%02d\r\n",WiFi_Msg.hour,WiFi_Msg.minute,WiFi_Msg.second);
//			printf("B数:%d\r\n",WiFi_Msg.bilifans);
//			Uart_Overflow2_Flag = False;
		}
		if(Uart_Overflow3_Flag)
		{
			VFD_Corn(CIRCLE,Clock_Msg.cation);
			VFD_Corn(TIME,Clock_Msg.alarm);
			VFD_Corn(HDCORN,(~Clock_Msg.mute)&1);
//			VFD_Corn(MESSG,Clock_Msg.wifi);
			VFD_Corn(DOLBY,Clock_Msg.aiwork);
			if(Clock_Msg.aiwork)
				VoiceFlag = True;
			if(VFDTipCount == 0)
			{
				if(Mi_Msg.wifi!=Clock_Msg.wifi)
				{
					Mi_Msg.wifi = Clock_Msg.wifi;
					VFDTipCount = 300;
					sprintf(VFDTip,"CONNECTING");
				}
				else if(Mi_Msg.cation!=Clock_Msg.cation)
				{
					Mi_Msg.cation = Clock_Msg.cation;
					if(Mi_Msg.cation)
					{
						VFDTipCount = 300;
						sprintf(VFDTip,"NEW REMINDER");
					}
				}
				else if(Mi_Msg.alarm!=Clock_Msg.alarm)
				{
					Mi_Msg.alarm = Clock_Msg.alarm;
					if(Mi_Msg.alarm)
					{
						VFDTipCount = 300;
						sprintf(VFDTip,"NEW ALARM");
					}
				}
				else if(Mi_Msg.mute!=Clock_Msg.mute)
				{
					Mi_Msg.mute = Clock_Msg.mute;
					VFDTipCount = 300;
					if(Clock_Msg.mute)
						sprintf(VFDTip,"MIC OFF");
					else sprintf(VFDTip,"MIC ON");
				}
			}
			else
			{
				Mi_Msg.cation = Clock_Msg.cation;
				Mi_Msg.alarm = Clock_Msg.alarm;
				Mi_Msg.mute = Clock_Msg.mute;
				Mi_Msg.wifi = Clock_Msg.wifi;
			}
//			U3WatchDogCount = 0;
			Uart_Overflow3_Flag = False;
		}
//		if(U3WatchDogCount>10)
//		{
//			U3WatchDogCount = 0;
//			MX_USART3_UART_Init();
//			HAL_UART_Receive_IT(&huart3,&Uart_Recv3_Data,1);      // 重新使能串口1接收中断
//		}
#endif
		
		if(SleepCount++>SLEEPTIME*2)
		{
			if(Device_Cmd.commandset&0x4)
			{
					Flag_Sleep = True;
			}
			SleepCount = 0;
			Update_Pos();
		}
		
		if(SaveFlag && SystemActive)
		{
			SaveFlag = False;
			Tranfcmd();
		}
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
