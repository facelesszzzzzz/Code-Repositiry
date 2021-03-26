#include "run.h"


void System_Init()
{
//	delay_init();	    	 							//延时函数初始化	  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//	//uart_init(115200);	 							//串口初始化为115200
//	OLED_Init();										//显示屏初始化
//	RELAY_Init();										//继电器初始化
//	BEEP_Init();										//蜂鸣器初始化
//	KEY_Init();											//按键初始化
//	//VOICE_Init();								
//	while(DHT22_Init());								//温湿度传感器初始化
//	//VOICE_Send(0x02);
//	RC522_Init();										//RFID初始化
//	MQ2_Init();											//烟雾传感器初始化
//	WIFI_Connect();										//wifi连接
//	OLED_MAIN();										//OLED显示
//	TIM3_Int_Init(499,7199);							//72Mhz/(7199+1)=10Khz的.计数频率，0.1ms,  计数到(499+1)为50ms  
}

void System_Running()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//	Task_ReceiveHandle();
//	Task_UARTHandle();
//	Task_BodyHandle();
//	
//	Task_KEYHandle();
//	Task_RC522Handle();
//	Task_DHT22Handle();
//	Task_MQ2Handle();
}

//void Task_ReceiveHandle()
//{
//	ESP8266_RecToAPP();
//	OLED_SHOWBMP(gState);
//}

//void Task_BodyHandle()
//{
//	BODY_Handle();
//}

//void Task_UARTHandle()
//{
//	if(UART_TIMER) return;
//	UART_TIMER = SYS_50MS;
//	ESP8266_SendToApp();
//}
//void Task_KEYHandle()
//{
//  	if(KEY_TIMER) return;
//  	KEY_TIMER = SYS_100MS;
//	KEY_Handle();
//}
//void Task_RC522Handle()
//{
//	if(RC522_TIMER) return;
//	RC522_TIMER = SYS_200MS;
//	RC522_Handle();
//}
//void Task_DHT22Handle()
//{
//	if(DHT22_TIMER) return;
//	DHT22_TIMER = SYS_500MS;
//	while(DHT22_Read_Data());
//}

//void Task_MQ2Handle()
//{
//	if(MQ2_TIMER) return;
//	MQ2_TIMER = SYS_1S;
//	MQ2_Handle();
//}

