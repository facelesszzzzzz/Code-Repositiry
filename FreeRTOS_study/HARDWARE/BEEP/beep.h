#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//�������˿ڶ���
#define BEEP_GPIO				GPIOC
#define BEEP_GPIO_IO			GPIO_Pin_6 

//#define BEEP_ON                 GPIO_ResetBits(BEEP_GPIO,BEEP_GPIO_IO)
//#define BEEP_OFF                GPIO_SetBits(BEEP_GPIO,BEEP_GPIO_IO)

#define SOUND_GPIO              GPIOB
#define SOUND_GPIO_IO           GPIO_Pin_15

#define SOUND_READ              GPIO_ReadInputDataBit(SOUND_GPIO, SOUND_GPIO_IO)

typedef enum
{
    Beep_Off = 0,
    Beep_On,
}Beep_State_e;

void BEEP_Init(void);	//��ʼ��
void Sound_Init(void);
uint8_t Get_BeepState(void);
void BEEP_Config(uint8_t mState);
void BEEP_Handle(void);
		 				    
#endif

