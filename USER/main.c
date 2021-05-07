#include "sys.h"	
#include "delay.h"	
#include "led.h" 
#include "beep.h" 
#include "usart.h" 
#include "lsens.h" 
#include "esp8266.h"
#include "onenet.h"
#include "oled.h"
#include "dht11.h"
#include "stdio.h"  //NULL

 u8 led_status=0;
 u8 light;                 
 u8 temperature;  	    
 u8 humidity;
 
 const char *subtopics[]={"ctrl"}; //订阅的主题
 
 char PUB_BUF[256];                //发布的数据包
 
 unsigned short timeCount=0; 
 unsigned char *dataPtr = NULL; 
 int main(void)
 {
	delay_init();	    	     //延时函数初始化	 	
	LED_Init();		  	 	     //初始化与LED连接的硬件接口
	BEEP_Init();         	     //初始化蜂鸣器端口
	Usart1_Init(115200);         //初始化串口1
	Usart3_Init(115200);    	 //串口3和esp8266通信
	Lsens_Init();           	 //初始化光照传感器	 
	ESP8266_Init();         	 //初始化esp8266	
	OLED_Init();			     //初始化OLED	
    DHT11_Init();				 //初始化OLED
	 
    OLED_ShowString(6,0,"SMART COMMUNITY",16);	   //显示智慧社区
	OLED_ShowString(24,16,"Light:",16);            //显示光照强度
    OLED_ShowString(88,16,"%",16);	
	 
	OLED_ShowString(24,32,"Temp:",16); 		  	   //显示温度
	OLED_ShowString(88,32,"C",16);	
	 
	OLED_ShowString(24,48,"Humi:",16);			   //显示湿度
    OLED_ShowString(88,48,"%",16);
	 
	OLED_Refresh_Gram();		//更新显示到OLED	
	 
	while(OneNet_DevLink())		//接入服务器
   	    delay_ms(500);			
	OneNet_Subscribe(subtopics,1);
	
	while(1)
	{		
		light=Lsens_Get_Val();  					//读取光照度	
		DHT11_Read_Data(&temperature,&humidity);	//读取温湿度
		
		OLED_ShowNum(72,16,light,2,16);  		    //显示光照度
		OLED_ShowNum(64,32,temperature,2,16);		//显示温度值  
		OLED_ShowNum(64,48,humidity,2,16);		    //显示湿度值	  
	    
		OLED_Refresh_Gram();					    //更新显示到OLED	
			
		
		if(++timeCount >= 10){									//发送间隔5s
		
			led_status=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);//读取LED1的状态
			sprintf(PUB_BUF,"{\"Light\":%d,\"Temp\":%d,\"Humi\":%d,\"Led\":%d}",light,temperature,humidity,led_status);					
			OneNet_Publish("data",PUB_BUF);		
			
			timeCount = 0;			
			ESP8266_Clear();	
			
		}
		
		dataPtr = ESP8266_GetIPD(3);                 //接收命令
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);		
		delay_ms(10);
		
	}
 }

