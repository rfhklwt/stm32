#include "timer.h"
#include "led.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：通用定时器3配置
*  作者：rfhklwt
*  修改日期：2017/07/14
*  版本：V1.0
*  功能：
***************************************************************/

/***************************************************************
* 名称：   	通用定时器3中断初始化
* 参数：	arr：自动重装值
*			psc：时钟预分频数
* 注意：	这里时钟选择为APB1的2倍，而APB1为36M
* PS:       若arr为4999,psc为7199,则产生(72M/(7199+1)) = 10KHz
*			的频率,即0.1ms一个脉冲，一共计数(4999+1) = 5000次,
*			则5000*0.1ms = 500ms.即该定时器为500ms
****************************************************************/

void TIM3_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 			//时钟使能

	/* 定时器TIM3初始化 */
	TIM_TimeBaseStructure.TIM_Period = arr; 						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 						//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 						//使能指定的TIM3中断,允许更新中断

	/* 中断优先级NVIC设置 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  				//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		//抢占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  			//响应优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  								//初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  										//使能TIMx
}

/***************************************************************
* 名称：   定时器3中断服务程序
* 参数：	无
***************************************************************/
//定时器3中断服务程序
void TIM3_IRQHandler(void)   										//TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  			//检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  				//清除TIMx更新中断标志
		//在此编写程序
		}
}












