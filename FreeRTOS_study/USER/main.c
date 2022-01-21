#include "delay.h"
#include "sys.h"
#include "usart.h"

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "malloc.h"
#include "Datatype.h"

#include "key.h"
#include "ESP8266.h"
#include "oled.h"
#include "dht22.h"
#include "voice.h"
#include "beep.h"

#define DEFAULT_MODE        0
#define CARE_MODE           1

#define TASK_INIT_STACK_LENGTH				50
#define MAIN_RUNNING_STACK_LENGTH			128
TaskHandle_t Task_Init_Handle;
TaskHandle_t MainRunning_Handle;

TimerHandle_t Timer_Count_Handle;
extern EventGroupHandle_t ESP8266_EventGroup_Handle;

void Timer_Count_Task(TimerHandle_t pxTimer)
{

}

void MainRunning_Task(void *pvParameters)
{
    uint8_t lCurrentMode = DEFAULT_MODE;
    uint8_t *pModeStr[] = {"Mode:Default","Mode:Care"};
    uint8_t lSoundCount = 0;
    uint8_t lSoundState = VOICE_DISABLE;
    /* �ȴ����Ϸ����� */
    while(pdTRUE == xEventGroupWaitBits((EventGroupHandle_t) ESP8266_EventGroup_Handle,
                                        (EventBits_t) ESP8266_CONNECT_BIT,
                                        (BaseType_t) pdTRUE,
                                        (BaseType_t) pdFALSE,
                                        (TickType_t) portMAX_DELAY));
    while(1)
    {
        /* �������� */
        switch(KEY_Scan(1))
        {
            case KEY0_PRES:
                /* ģʽ�л� */
                lCurrentMode = lCurrentMode == DEFAULT_MODE ? CARE_MODE : DEFAULT_MODE;
                break;
            case KEY1_PRES:
                lCurrentMode = DEFAULT_MODE;
                if(Beep_On == Get_BeepState())
                    BEEP_Config(Beep_Off);
                else
                    BEEP_Config(Beep_On);
                break;
            case WKUP_PRES:
                lCurrentMode = DEFAULT_MODE;
//                VOICE_Select(Voice_none);
                break;
            default:
                break;
        }
        DHT22_Read_Data();  
        /* ģʽ���� */
        switch(lCurrentMode)
        {
            case DEFAULT_MODE:
                break;
            case CARE_MODE:
                DHT22_LIMIT();
                if(SOUND_READ){
                    lSoundCount++;
                    if(lSoundCount > 5 && VOICE_DISABLE == lSoundState){
                        lSoundState = VOICE_ENABLE;
                        /* ��⵽������ */
                        VOICE_Select(Voice_one);
                    }
                }else{
                    lSoundState = VOICE_DISABLE;
                    VOICE_Select(Voice_none);
                    lSoundCount = 0;
                }
                break;
            default:
                break;
        }
        BEEP_Handle();
        OLED_ShowString(0, 6, pModeStr[lCurrentMode], 16);
        vTaskDelay(10);
    }
}

void Task_Init(void *pvParameters)
{
//	printf("Task Running\r\n");
	taskENTER_CRITICAL();
    ESP8266_Task_Init();
//   	ESP8266_Queue_Handle = xQueueCreate(1, ESP8266_QUEUE_LEN);
//   	Message_Queue_Handle = xQueueCreate(1, ESP8266_QUEUE_LEN);
//   	ESP8266_EventGroup_Handle = xEventGroupCreate();
//   	SendEventGroup(ESP8266_EventGroup_Handle, 0xFF);
//   	Timer_Count_Handle = xTimerCreate("TimerCount_Task",
//   										1000,
//   										pdTRUE,
//   										0,
//   										(TimerCallbackFunction_t)Timer_Count_Task);
//   	xTimerStart(Timer_Count_Handle, 0);
    xTaskCreate((TaskFunction_t) MainRunning_Task,
				(const char * ) "MainRunning_Task",
				MAIN_RUNNING_STACK_LENGTH,
				(void *) NULL,
				(UBaseType_t) MAIN_RUNNING_PRIORITY,
				(TaskHandle_t *) &MainRunning_Handle);
	vTaskDelete(Task_Init_Handle);
	taskEXIT_CRITICAL();
}

int main (void)
{		 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����NVIC�жϷ���4λ��ռ���ȼ�
	delay_init();
	OLED_Init();
	uart_init(115200);	        //���ڳ�ʼ��Ϊ115200
    usart3_init(115200);
	KEY_Init();                 //��ʼ���밴�����ӵ�Ӳ���ӿ�
	BEEP_Init();
    OLED_ShowString(0, 0, "Bsp_initialize", 16);
	Sound_Init();
    OLED_ShowString(0, 2, "Voice_initialize", 16);
    VOICE_Init();
    OLED_ShowString(0, 4, "DHT22_Initialize", 16);
    while(DHT22_Init());
    OLED_ShowString(0, 6, "OV7725_Initialize", 16);
    while(1)//��ʼ��OV7725_OV7670
	{
		if(OV7725_Init()==0)
		{
            OV7725_Configuration();			
			delay_ms(1500);
			break;
		}
		else
		{
			delay_ms(200);
		}
	} 	  							  
	EXTI15_Init();							
    delay_ms(2000);
    OLED_Clear();
	xTaskCreate((TaskFunction_t) Task_Init,
				(const char * ) "Task_Init",
				TASK_INIT_STACK_LENGTH,
				(void *) NULL,
				(UBaseType_t) TASK_INIT_PRIORITY,
				(TaskHandle_t *) &Task_Init_Handle);
	vTaskStartScheduler();
 	while(1); 
 }

