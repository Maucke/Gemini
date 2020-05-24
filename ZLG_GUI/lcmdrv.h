/****************************************************************************
* �ļ�����LCMDRV.H
* ���ܣ�12864ͼ��Һ��ģ����������
*       ����ZLG/GUI�û�ͼ�ν��档
*  
*  ���ߣ����ܱ�
*  ���ڣ�2005/3/7
****************************************************************************/
#ifndef  LCMDRV_H
#define  LCMDRV_H  


/* ������ɫ��������(���������ݽṹ) */
#define  TCOLOR	            uint8_t     // �����޸�

/* ����LCM�������� */
#define  GUI_LCM_XMAX		304     /* ����Һ��x��������� */
#define  GUI_LCM_YMAX		96      /* ����Һ��y��������� */

/* ����LCM�������� */
#define  GUI_VFD_LCM_XMAX		84     /* ����Һ��x��������� */
#define  GUI_VFD_LCM_YMAX		16      /* ����Һ��y��������� */


void  GUI_Initialize(void);
void  GUI_FillSCR(TCOLOR dat);
void  GUI_ClearSCR(void);
void  GUI_Point(uint16_t x, uint8_t y, TCOLOR color);
uint8_t  GUI_ReadPoint(uint16_t x, uint8_t y, TCOLOR *ret);
void  GUI_HLine(uint16_t x0, uint8_t y0, uint16_t x1, TCOLOR color);
void  GUI_RLine(uint16_t x0, uint8_t y0, uint8_t y1, TCOLOR color);
void  GUI_Exec(void);

void  GUI_VFD_Initialize(void);
void  GUI_VFD_FillSCR(TCOLOR dat);
void  GUI_VFD_ClearSCR(void);
uint8_t  GUI_VFD_ReadPoint(uint16_t x, uint8_t y, TCOLOR *ret);
void  GUI_VFD_HLine(uint16_t x0, uint8_t y0, uint16_t x1, TCOLOR color);
void  GUI_VFD_RLine(uint16_t x0, uint8_t y0, uint8_t y1, TCOLOR color);
void  GUI_VFD_Exec(void);
void  GUI_VFD_Point(uint16 x, uint8 y, TCOLOR color);
/****************************************************************************
* ���ƣ�GUI_CmpColor()
* ���ܣ��ж���ɫֵ�Ƿ�һ�¡�
* ��ڲ�����color1		��ɫֵ1
*		   color2		��ɫֵ2
* ���ڲ���������1��ʾ��ͬ������0��ʾ����ͬ��
* ˵����������ɫ����TCOLOR�����ǽṹ���ͣ�������Ҫ�û���д�ȽϺ�����
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )

/****************************************************************************
* ���ƣ�GUI_CopyColor()
* ���ܣ���ɫֵ���ơ�
* ��ڲ�����color1		Ŀ����ɫ����
*		   color2		Դ��ɫ����
* ���ڲ�������
* ˵����������ɫ����TCOLOR�����ǽṹ���ͣ�������Ҫ�û���д���ƺ�����
****************************************************************************/
#define  GUI_CopyColor(color1, color2) 	*color1 = color2



#endif