#ifndef __REMOTE_H
#define __REMOTE_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//����ң�ؽ�����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/31
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define RDATA   PAin(1)		//�������������

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   

extern u8 RemoteFlag;	        //�������µĴ���
extern u8 RmtCnt;	        //�������µĴ���
extern u8 RmtSta;
void Remote_Init(void);     //���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);

#define IRF_PW 0x80

#define IRF_1 0xA8
#define IRF_2 0x68
#define IRF_3 0xE8
#define IRF_4 0x98
#define IRF_5 0x58
#define IRF_6 0xD8
#define IRF_7 0xB8
#define IRF_8 0x78
#define IRF_9 0xF8
#define IRF_0 0x82
#define IRF_NL 0xD2
#define IRF_CH 0x22

#define IRF_PC 0xA2
#define IRF_TV 0xA0

#define IRF_LF 0x08
#define IRF_RT 0x30
#define IRF_UP 0x18
#define IRF_DN 0x38
#define IRF_OK 0x50

#define IRF_MT 0x20
#define IRF_IF 0x02

#define IRF_PIP 0x48
#define IRF_SLP 0xC8
#define IRF_WID 0x28

#endif
