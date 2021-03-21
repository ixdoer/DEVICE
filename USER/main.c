#include "sys.h"	
#include "delay.h"	
#include "led.h" 
#include "beep.h" 
#include "usart.h" 
#include "lsens.h" 

 u8 light;                  //光照度
 int main(void)
 {
	delay_init();	    	//延时函数初始化	  
	LED_Init();		  	 	//初始化与LED连接的硬件接口
	BEEP_Init();         	//初始化蜂鸣器端口
	Usart1_Init(115200);    //初始化串口1
	Lsens_Init();           //初始化光照传感器
	while(1)
	{		
		light=Lsens_Get_Val();
		LED0=0;
		//BEEP=0;		  
		delay_ms(300);//延时300ms
		LED0=1;	  
		//BEEP=1;  
		delay_ms(300);//延时300ms
		//向串口1输出hello
		UsartPrintf(USART1,"当前光照强度为：%d\n",light);
	}
 }

