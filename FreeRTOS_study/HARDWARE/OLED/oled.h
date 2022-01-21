//              ����˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PB12��SCL��
//              SDA   ��PB13��SDA��            
//              ----------------------------------------------------------------
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"
#include "datatype.h"
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    					

//#define OLED_BMPINIT			OLED_DrawBMP(32, 0, 96, 8, BMP_INIT)		//��ʾ��ʼ��ͼ��
//#define OLED_BMPBEEP			OLED_DrawBMP(0, 2, 16, 4, BMP_BEEP)			//��ʾ����������ͼ��
//#define	OLED_BMPBEEPCLOSED		OLED_DrawBMP(0, 2, 16, 4, BMP_BEEPCLOSE)	//��ʾ�������ر�ͼ��
//#define OLED_BMPDOOROPEN		OLED_DrawBMP(112, 2, 128, 4, BMP_DOOROPEN)	//��ʾ����ͼ��
//#define OLED_BMPDOORCLOSED		OLED_DrawBMP(112, 2, 128, 4, BMP_DOORCLOSE)	//��ʾ����ͼ��
//#define	OLED_BMPMANUAL			OLED_DrawBMP(96, 2, 112, 4, BMP_MANUAL)		//��ʾ�ֶ�ģʽͼ��
//#define OLED_BMPAUTO			OLED_DrawBMP(96, 2, 112, 4, BMP_AUTO)		//��ʾ�Զ�ģʽͼ��
//#define OLED_BMPFAN				OLED_DrawBMP(16, 2, 32, 4, BMP_FAN)			//��ʾ���ȿ���ͼ��
//#define OLED_BMPNOFAN			OLED_DrawBMP(16, 2, 32, 4, BMP_NO)			//��ʾ�޷���ͼ��

//#define OLED_BMPHELP			OLED_DrawBMP(32, 2, 48, 4, BMP_HELP);		//��ʾ����ͼ��
//#define OLED_BMPNOHELP			OLED_DrawBMP(32, 2, 48, 4, BMP_NO);		//��ʾ�ޱ���ͼ��

//#define OLED_BMPTEM				OLED_DrawBMP(0, 4, 16, 6, BMP_TEM) 			//��ʾ�¶�ͼ��
//#define OLED_BMPHUMI			OLED_DrawBMP(64, 4, 80, 6, BMP_HUMI)		//��ʾʪ��ͼ��
//#define OLED_BMPMQ2				OLED_DrawBMP(32, 6, 48, 8, BMP_MQ2)			//��ʾ����Ũ��ͼ��

//#define OLED_BMPHEARTRATE		OLED_DrawBMP(0, 3, 16, 5, BMP_HEARTRATE)
//#define OLED_BMPSPO2			OLED_DrawBMP(0, 6, 16, 8, BMP_SPO2)
//#define OLED_BMPTEMPGUN			OLED_DrawBMP(96, 0, 128, 4, BMP_TEMPGUN)


//-----------------OLED IIC�˿ڶ���------------------------------------------------------ 					   
//��һ��OLED
#define OLED_GPIO 					GPIOB
#define OLED_SCK					GPIO_Pin_13
#define OLED_SDA					GPIO_Pin_12

#define OLED_SCLK_Clr() GPIO_ResetBits(OLED_GPIO,OLED_SCK)//SCL IIC�ӿڵ�ʱ���ź�
#define OLED_SCLK_Set() GPIO_SetBits(OLED_GPIO,OLED_SCK)

#define OLED_SDIN_Clr() GPIO_ResetBits(OLED_GPIO,OLED_SDA)//SDA IIC�ӿڵ������ź�
#define OLED_SDIN_Set() GPIO_SetBits(OLED_GPIO,OLED_SDA)

/*
//�ڶ���OLED
#define OLED_SCLK_Clr1() GPIO_ResetBits(GPIOE,GPIO_Pin_7)//SCL IIC�ӿڵ�ʱ���ź�
#define OLED_SCLK_Set1() GPIO_SetBits(GPIOE,GPIO_Pin_7)

#define OLED_SDIN_Clr1() GPIO_ResetBits(GPIOE,GPIO_Pin_8)//SDA IIC�ӿڵ������ź�
#define OLED_SDIN_Set1() GPIO_SetBits(GPIOE,GPIO_Pin_8)
*/

/*-------------------------------------------------------------------------------------*/
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�����ú���
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
//void Picture();
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack(void);

//void OLED_MAIN(void);
//void OLED_SHOWBMP(Byte8 data);
//void OLED_SHOWDATA(void);







//OLED2
//OLED2�����ú���
//void OLED_WR_Byte1(unsigned dat,unsigned cmd);  
//void OLED_Display_On1(void);
//void OLED_Display_Off1(void);	   							   		    
//void OLED_Init1(void);
//void OLED_Clear1(void);
//void OLED_DrawPoint1(u8 x,u8 y,u8 t);
//void OLED_Fill1(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
//void OLED_ShowChar1(u8 x,u8 y,u8 chr,u8 Char_Size);
//void OLED_ShowNum1(u8 x,u8 y,u32 num,u8 len,u8 size);
//void OLED_ShowString1(u8 x,u8 y, u8 *p,u8 Char_Size);	 
//void OLED_Set_Pos1(unsigned char x, unsigned char y);
//void OLED_ShowCHinese1(u8 x,u8 y,u8 no);
//void OLED_DrawBMP1(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

//void fill_picture1(unsigned char fill_Data);
////void Picture1();
//void IIC_Start1(void);
//void IIC_Stop1(void);
//void Write_IIC_Command1(unsigned char IIC_Command);
//void Write_IIC_Data1(unsigned char IIC_Data);
//void Write_IIC_Byte1(unsigned char IIC_Byte);

//void IIC_Wait_Ack1(void);
#endif  
	 



