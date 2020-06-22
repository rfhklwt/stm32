#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
//#include "LCD5110.h"
//#include "pwm.h"

/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：中断配置代码
*  作者：rfhklwt
*  修改日期：2017/07/12
*  版本：V1.0
*  功能：中断函数
***************************************************************/


/***************************************************************
*         外部中断初始化函数（PA13和PC5还有PA15和PA0）
*	注：在key.h已经进行了如下宏定义
*  KEY0	-----	PA13
*  KEY1	-----	PA15
*  KEY2	-----	PC5
*  WK_UP	-----	PA0
***************************************************************/
void EXTIX_Init(void)
{

	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//外部中断，需要使能AFIO时钟

	KEY_Init();													//初始化按键对应IO模式

    //GPIOA.13(key0)  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13);

  	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 							//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//GPIOC.5  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);

	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 							//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.15(key1)  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 							//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器



    //GPIOA.0(WK_UP)  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

   	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//上升沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);								//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

/***************************************************************/
/*               配置中断分组（NVIC），并使能中断              */
/***************************************************************/

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能WA_UP按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	 						 //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能PC5按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2，
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);


   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//使能key0和key1按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2，
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		//子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

}

/***************************************************************/
/*                        中断函数的编写                       */
/***************************************************************/

 //WK_UP中断函数
void EXTI0_IRQHandler(void)
{
  delay_ms(10);    //按键去抖动

	if(WK_UP == 1)
	{
		//添加代码
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  					//清除EXTI0线路挂起位
}
 void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);   //消抖
	if(KEY2 == 0)
	{
		//添加代码
	}
	EXTI_ClearITPendingBit(EXTI_Line5);    				//清除LINE5上的中断标志位
}

//KEY0和KEY1中断函数
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);    //消抖
	if(KEY1 == 0)
	{
		//添加代码
	}
	EXTI_ClearITPendingBit(EXTI_Line15);  					//清除LINE15线路挂起位
	if(KEY0 == 0)
	{
		//添加代码
	}
 	 EXTI_ClearITPendingBit(EXTI_Line13);   				//清除LINE13上的中断标志位
}
