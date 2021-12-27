#include "esp8266_mqtt.h"
#include "string.h"
#include "usart.h"

//FreeRTOSϵͳ���ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"

//���ӳɹ���������Ӧ 20 02 00 00
//�ͻ��������Ͽ����� e0 00
const uint8_t parket_connetAck[] = {0x20,0x02,0x00,0x00};
const uint8_t parket_disconnet[] = {0xe0,0x00};
const uint8_t parket_heart[] = {0xc0,0x00};
const uint8_t parket_heart_reply[] = {0xc0,0x00};
const uint8_t parket_subAck[] = {0x90,0x03};

volatile uint16_t MQTT_TxLen;
extern u8 USART3_RX_BUF[], USART3_TX_BUF[];
//MQTT��������
void MQTT_SendBuf(uint8_t *buf,uint16_t len)
{
   int i=0;  
	for(i = 0;i < len;i ++)
	{			
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
			USART_SendData(USART3,buf[i]);	
	}	
}	

//����������
void MQTT_SentHeart(void)
{
	MQTT_SendBuf((uint8_t *)parket_heart,sizeof(parket_heart));
}

//MQTT�������Ͽ�
void MQTT_Disconnect()
{
	MQTT_SendBuf((uint8_t *)parket_disconnet,sizeof(parket_disconnet));
}

//MQTT��ʼ��
void MQTT_Init(uint8_t *prx,uint16_t rxlen,uint8_t *ptx,uint16_t txlen)
{
	memset(USART3_TX_BUF,0,USART3_MAX_SEND_LEN); //��շ��ͻ���
	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN); //��ս��ջ���
	
	//�������������Ͽ�
	MQTT_Disconnect();
    //HAL_Delay(100);
    vTaskDelay(100);
	MQTT_Disconnect();
    //HAL_Delay(100);
    vTaskDelay(100);
}

//MQTT���ӷ������Ĵ������
uint8_t MQTT_Connect(char *ClientID,char *Username,char *Password)
{
	int ClientIDLen = strlen(ClientID);
	int UsernameLen = strlen(Username);
	int PasswordLen = strlen(Password);
	int DataLen;
    int i=0;
    
    uint8_t cnt=1;
    uint8_t wait;
	MQTT_TxLen=0;
	//�ɱ䱨ͷ+Payload  ÿ���ֶΰ��������ֽڵĳ��ȱ�ʶ
    DataLen = 10 + (ClientIDLen+2) + (UsernameLen+2) + (PasswordLen+2);

    //�̶���ͷ
    //���Ʊ�������
    USART3_TX_BUF[MQTT_TxLen++] = 0x10;		//MQTT Message Type CONNECT
	//ʣ�೤��(�������̶�ͷ��)
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		USART3_TX_BUF[MQTT_TxLen++] = encodedByte;
	}while ( DataLen > 0 );
    	
	//�ɱ䱨ͷ
	//Э����
	USART3_TX_BUF[MQTT_TxLen++] = 0;        		// Protocol Name Length MSB    
	USART3_TX_BUF[MQTT_TxLen++] = 4;        		// Protocol Name Length LSB    
	USART3_TX_BUF[MQTT_TxLen++] = 'M';        	// ASCII Code for M    
	USART3_TX_BUF[MQTT_TxLen++] = 'Q';        	// ASCII Code for Q    
	USART3_TX_BUF[MQTT_TxLen++] = 'T';        	// ASCII Code for T    
	USART3_TX_BUF[MQTT_TxLen++] = 'T';        	// ASCII Code for T    
	//Э�鼶��
	USART3_TX_BUF[MQTT_TxLen++] = 4;        		// MQTT Protocol version = 4    
	//���ӱ�־
	USART3_TX_BUF[MQTT_TxLen++] = 0xc2;        	// conn flags 
	USART3_TX_BUF[MQTT_TxLen++] = 0;        	// Keep-alive Time Length MSB    
	USART3_TX_BUF[MQTT_TxLen++] = 60;        	// Keep-alive Time Length LSB  60S������  

	USART3_TX_BUF[MQTT_TxLen++] = BYTE1(ClientIDLen);// Client ID length MSB    
	USART3_TX_BUF[MQTT_TxLen++] = BYTE0(ClientIDLen);// Client ID length LSB  	
	memcpy(&USART3_TX_BUF[MQTT_TxLen],ClientID,ClientIDLen);
	MQTT_TxLen += ClientIDLen;
	
	if(UsernameLen > 0)
	{   
		USART3_TX_BUF[MQTT_TxLen++] = BYTE1(UsernameLen);		//username length MSB    
		USART3_TX_BUF[MQTT_TxLen++] = BYTE0(UsernameLen);    	//username length LSB    
		memcpy(&USART3_TX_BUF[MQTT_TxLen],Username,UsernameLen);
		MQTT_TxLen += UsernameLen;
	}
	
	if(PasswordLen > 0)
	{    
		USART3_TX_BUF[MQTT_TxLen++] = BYTE1(PasswordLen);		//password length MSB    
		USART3_TX_BUF[MQTT_TxLen++] = BYTE0(PasswordLen);    	//password length LSB  
		memcpy(&USART3_TX_BUF[MQTT_TxLen],Password,PasswordLen);
		MQTT_TxLen += PasswordLen; 
	}    
	
    //��ӡ����
    printf("USART3_RX_BUF:\r\n");
    for(i=0;i<MQTT_TxLen;i++)
    {
        printf("%0x", USART3_TX_BUF[i]);
    }
    printf("\r\n");

	while(cnt)
	{
        cnt--;
		memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
		MQTT_SendBuf((uint8_t *)USART3_TX_BUF,MQTT_TxLen);
		wait=30;//�ȴ�3sʱ��
		while(wait--)
		{
			//CONNECT
			if(USART3_RX_BUF[0]==parket_connetAck[0] && USART3_RX_BUF[1]==parket_connetAck[1]) //���ӳɹ�			   
			{
				return 1;//���ӳɹ�
			}
			//HAL_Delay(100);	
            vTaskDelay(100);
		}
	}
	return 0;
}

//MQTT����/ȡ���������ݴ������
//topic       ���� 
//qos         ��Ϣ�ȼ� 
//whether     ����/ȡ�����������
uint8_t MQTT_SubscribeTopic(char *topic,uint8_t qos,uint8_t whether)
{    
	int topiclen;
	int DataLen;
    int i=0;
    
    uint8_t cnt=2;
	uint8_t wait;
    
	MQTT_TxLen=0;
    topiclen = strlen(topic);
    DataLen = 2 + (topiclen+2) + (whether?1:0);//�ɱ䱨ͷ�ĳ��ȣ�2�ֽڣ�������Ч�غɵĳ���
    
    //�̶���ͷ
	//���Ʊ�������
	if(whether) USART3_TX_BUF[MQTT_TxLen++] = 0x82; //��Ϣ���ͺͱ�־����
	else	USART3_TX_BUF[MQTT_TxLen++] = 0xA2;    //ȡ������

	//ʣ�೤��
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		USART3_TX_BUF[MQTT_TxLen++] = encodedByte;
	}while ( DataLen > 0 );	
	
	//�ɱ䱨ͷ
	USART3_TX_BUF[MQTT_TxLen++] = 0;				//��Ϣ��ʶ�� MSB
	USART3_TX_BUF[MQTT_TxLen++] = 0x01;           //��Ϣ��ʶ�� LSB
	//��Ч�غ�
	USART3_TX_BUF[MQTT_TxLen++] = BYTE1(topiclen);//���ⳤ�� MSB
	USART3_TX_BUF[MQTT_TxLen++] = BYTE0(topiclen);//���ⳤ�� LSB   
	memcpy(&USART3_TX_BUF[MQTT_TxLen],topic,topiclen);
	MQTT_TxLen += topiclen;

	if(whether)
	{
		USART3_TX_BUF[MQTT_TxLen++] = qos;//QoS����
	}
    
    //��ӡ����
    printf("USART3_RX_BUF:\r\n");
    for(i=0;i<MQTT_TxLen;i++)
    {
        printf("%0x", USART3_TX_BUF[i]);
    }
    printf("\r\n");
	
	while(cnt)
	{
        cnt--;
		memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
		MQTT_SendBuf((uint8_t *)USART3_TX_BUF,MQTT_TxLen);
		wait=30;//�ȴ�3sʱ��
		while(wait--)
		{
			if(USART3_RX_BUF[0]==parket_subAck[0] && USART3_RX_BUF[1]==parket_subAck[1]) //���ĳɹ�			   
			{
				return 1;//���ĳɹ�
			}
			//HAL_Delay(100);	
            vTaskDelay(100);
		}
        //printf("rx[0]:%d,rx[1]:%d\r\n", USART3_RX_BUF[0], USART3_RX_BUF[1]);
	}
	if(cnt) return 1;	//���ĳɹ�
	return 0;
}

//MQTT�������ݴ������
//topic   ���� 
//message ��Ϣ
//qos     ��Ϣ�ȼ� 
uint8_t MQTT_PublishData(char *topic, char *message, uint8_t qos)
{  
	int topicLength = strlen(topic);    
	int messageLength = strlen(message);     
	static uint16_t id=0;
	int DataLen;
	MQTT_TxLen=0;
	//��Ч�غɵĳ����������㣺�ù̶���ͷ�е�ʣ�೤���ֶε�ֵ��ȥ�ɱ䱨ͷ�ĳ���
	//QOSΪ0ʱû�б�ʶ��
	//���ݳ���             ������   ���ı�ʶ��   ��Ч�غ�
	if(qos)	DataLen = (2+topicLength) + 2 + messageLength;       
	else	DataLen = (2+topicLength) + messageLength;   

    //�̶���ͷ
	//���Ʊ�������
	USART3_TX_BUF[MQTT_TxLen++] = 0x30;    // MQTT Message Type PUBLISH  

	//ʣ�೤��
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		USART3_TX_BUF[MQTT_TxLen++] = encodedByte;
	}while ( DataLen > 0 );	
	
	USART3_TX_BUF[MQTT_TxLen++] = BYTE1(topicLength);//���ⳤ��MSB
	USART3_TX_BUF[MQTT_TxLen++] = BYTE0(topicLength);//���ⳤ��LSB 
	memcpy(&USART3_TX_BUF[MQTT_TxLen],topic,topicLength);//��������
	MQTT_TxLen += topicLength;
        
	//���ı�ʶ��
	if(qos)
	{
			USART3_TX_BUF[MQTT_TxLen++] = BYTE1(id);
			USART3_TX_BUF[MQTT_TxLen++] = BYTE0(id);
			id++;
	}
	memcpy(&USART3_TX_BUF[MQTT_TxLen],message,messageLength);
  MQTT_TxLen += messageLength;
        
	MQTT_SendBuf((uint8_t *)USART3_TX_BUF,MQTT_TxLen);
  return MQTT_TxLen;
}
