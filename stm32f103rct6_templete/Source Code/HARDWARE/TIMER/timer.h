#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define MAX_TIMER				5
extern u8 gTimer[MAX_TIMER];
#define UART_TIMER				gTimer[0]
#define KEY_TIMER				gTimer[1]
#define RC522_TIMER				gTimer[2]
#define DHT22_TIMER				gTimer[3]
#define MQ2_TIMER				gTimer[4]

#define SYS_50MS				1
#define SYS_100MS				2
#define SYS_200MS				4
#define SYS_500MS				10
#define SYS_1S					20


void TIM3_Int_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);

 
#endif
