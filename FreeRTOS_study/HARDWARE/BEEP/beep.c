#include "beep.h"  

static uint8_t gBeep_State;
//��ʼ��PB8Ϊ�����.��ʹ������ڵ�ʱ��		    
//��������ʼ��
void BEEP_Init(void)
{
 
    GPIO_InitTypeDef  GPIO_InitStructure;
    	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_IO;			//BEEP-->PB.8 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //�ٶ�Ϊ50MHz
    GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);	            //���ݲ�����ʼ��GPIOB.8

    GPIO_ResetBits(BEEP_GPIO,BEEP_GPIO_IO);                //���0���رշ��������

}
uint8_t Get_BeepState(void)
{
    return gBeep_State;
}

void BEEP_Config(uint8_t mState)
{
    gBeep_State = mState;
}

void BEEP_Handle(void)
{
    if(gBeep_State)
        GPIO_SetBits(BEEP_GPIO,BEEP_GPIO_IO);
    else
        GPIO_ResetBits(BEEP_GPIO,BEEP_GPIO_IO);
}

void Sound_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = SOUND_GPIO_IO;			
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		    
    GPIO_Init(SOUND_GPIO, &GPIO_InitStructure);	           
}


