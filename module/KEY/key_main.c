#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"

/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：LED驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/10
*  版本：V1.0
*  功能：利用按键实现LED的亮灭
***************************************************************/
 int main(void)
 {
 	u8 t;
	delay_init();	    	 			//延时函数初始化
 	LED_Init();			     			//LED端口初始化
	KEY_Init();          				//初始化与按键连接的硬件接口
	LED0 = 0;							//先点亮红灯
	while(1)
	{
 		t = KEY_Scan(0);				//得到键值
	   	if(t)
		{
			switch(t)
			{
				case KEY0_PRES:			//控制LED0翻转
					LED0 = !LED0;
					break;
				case KEY1_PRES:			//控制LED1翻转
					LED1 = !LED1;
					break;
				case WKUP_PRES:			//同时控制LED0,LED1翻转
					LED0 = !LED0;
					LED1 = !LED1;
					break;
			}
		}
		else delay_ms(10);
	}
 }

