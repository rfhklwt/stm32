#include "led.h"
#include "delay.h"
#include "sys.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：LED驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/09
*  版本：V1.0
*  功能：实现led灯不断闪
***************************************************************/
int main(void)
{
	delay_init();	    	 //延时函数初始化（在SYSTEM文件夹内）
	LED_Init();		  		//初始化与LED连接的硬件接口
	while(1)
	{
		GPIO_ResetBits(GPIOA GPIO_Pin_8);
		GPIO_SetBits(GPIOD, GPIO_Pin_2);
		delay_ms(300);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);
		delay_ms(300);
	}
 }

