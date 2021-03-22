#include "sys.h"	
#include "delay.h"	
#include "led.h" 
#include "beep.h" 
#include "usart.h" 
#include "lsens.h" 
#include "esp8266.h"
#include "onenet.h"
#include "stdio.h"  //NULL

 u8 light;                  //光照度
 const char *topics[]={"smartsub"};
 unsigned short timeCount=0; 
 unsigned char *dataPtr = NULL; 
 int main(void)
 {
	delay_init();	    	//延时函数初始化	  
	LED_Init();		  	 	//初始化与LED连接的硬件接口
	BEEP_Init();         	//初始化蜂鸣器端口
	Usart1_Init(115200);    //初始化串口1
	Usart3_Init(115200);    //串口3和esp8266通信
	Lsens_Init();           //初始化光照传感器	 
	ESP8266_Init();         //初始化esp8266
	
	while(OneNet_DevLink())//接入服务器
   	    delay_ms(500);	   		
		
	OneNet_Subscribe(topics,1);
	while(1)
	{		
		light=Lsens_Get_Val();
		LED0=0;				  
		delay_ms(300);//延时300ms
		LED0=1;	 		
		delay_ms(300);//延时300ms	
		
		
		if(++timeCount >= 10){									//发送间隔5s
		
			//UsartPrintf(USART_DEBUG,"当前光照强度为：%d\n",light);
			
			//UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
				
			OneNet_Publish("smartpub","oh yeah!");
			
			timeCount = 0;
			
			ESP8266_Clear();	
		}
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		delay_ms(10);
	}
 }

