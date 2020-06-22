#include "stm32f10x.h"
#include "LCD5110.h"
#include "adc.h"
#include "delay.h"
#include "sys.h"


/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：ADC驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/25
*  版本：V1.0
*  功能：利用ADC获取管脚的电压
***************************************************************/
 int main(void)
 {
 	/*声明变量（包括要输出的十位、个位、小数位）*/
	double adcx = 0;
	double temp = 0;
	u8 unit01 = 0;		//个位数
	u8 dot01_1 = 0;		//小数点后一位
	u8 dot01_2 = 0;		//小数点后两位
	/*第二个ADC使用
	u8 unit02 = 0;
	u8 dot02_1 = 0;
	u8 dot02_2 = 0;	*/

	delay_init();	    	 			//延时函数初始化
	LCD5110_Init ();
	Adc_Init();
	while(1)
	{
		adcx = Get_Adc_Average(ADC_Channel_1, 10);									//获取AD数值（0 ~ 4095）
		temp = (float)adcx * (3.3 / 4096);											//获取相应的电压值

		//adcx = temp;
		tens = (int)temp;
		dot1 = ((int)(temp*10)/1) % 10;
		dot2 = ((int)(temp*100)/1) % 10;
		delay_ms(250);

		LCD_Show_Chinese(6, 2, 12, 22);		    									//广
		LCD_Show_Chinese(18, 2, 12, 23);		  									//东
		LCD_Show_Chinese(30, 2, 12, 24);		  									//工
		LCD_Show_Chinese(42, 2, 12, 25);		  									//业
		LCD_Show_Chinese(54, 2, 12, 26);		  									//大
		LCD_Show_Chinese(66, 2, 12, 27);		  									//学

		LCD_Show_Chinese(0, 4, 12, 12);												//输
		LCD_Show_Chinese(12, 4, 12, 13);											//出
		LCD_Show_Chinese(24, 4, 12, 14);											//电
		LCD_Show_Chinese(36, 4, 12, 21);											//压
		LCD_Show_BigChar(48, 4, 6, 12, ':');
		LCD_Show_BigChar(60, 4, 6, 12, '.');  										//.
		LCD_Show_BigChar(78, 4, 6, 12, 37);  										//v

		/*显示电压值*/
		LCD_Show_BigChar(54, 4, 6, 12, unit01);		 								//个位数
		LCD_Show_BigChar(66, 4, 6, 12, dot01);	 									//小数点后一位
		LCD_Show_BigChar(72, 4, 6, 12, dot02);	 									//小数点后两位



	}
 }

