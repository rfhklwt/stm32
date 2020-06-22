#include "pwm.h"
#include "led.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
/***************************************************************
*  ALIENTEK mini STM32开发板
*  名称：PWM  驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/16
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

void TIM1_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	TIM_BDTRInitStructure.TIM_DeadTime = 0x10;

	/*使能TIM1以及PA和PB时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设时钟使能

/***************************************************************
* 名称：   	GPIO初始化
* 描述：	配置TIM1复用输出PWM时用到的I/O
* 通道：	TIM1_CH1
*			TIM1_CH4
*			TIM_CH1N
****************************************************************/

   	/*设置该引脚为复用输出功能,输出TIM1_CH1的PWM脉冲波形*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 				//TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*设置该引脚为复用输出功能,输出TIM1_CH4的PWM脉冲波形*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 				//TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*设置该引脚为复用输出功能,输出TIM_CH1N的PWM脉冲波形*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 				//TIM_CH1N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

/***************************************************************
* 名称：   	Time base configuration
* 描述：	时基初始化
****************************************************************/
	/*当定时器从0计数到arr，为一个定时周期(//80k)*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	/*设置预分频：不分频，即为72MHz*/
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	/*设置时钟分频系数：不分频*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	/*向上计数模式*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

/***************************************************************
* 名称：   	PWM1 Mode configuration
* 描述：	PWM1模式初始化
* 通道：	channel1
*			channel4
****************************************************************/
	/*PWM1 Mode configuration: Channel1*/
	/*配置为PWM模式1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*设置跳变值，当计数器计数到这个值(CCR1_Val = 0)时，电平发生跳变*/
	TIM_OCInitStructure.TIM_Pulse = 0;
	/*当定时器技术直接小于CCR1_Val时为高电平*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*使能通道1*/
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);


	/*PWM1 Mode configuration: Channel4*/
	/*配置为PWM模式1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*设置跳变值，当计数器计数到这个值(CCR1_Val = 0)时，电平发生跳变*/
	TIM_OCInitStructure.TIM_Pulse = 0;
	/*当定时器技术直接小于CCR1_Val时为高电平*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*使能通道4*/
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);


/***************************************************************
* 名称：   	死区时间测试代码PWM输出配置
****************************************************************/

	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; 				//设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;				//使能互补输出端
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; 				//死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;				//死区后互补端输出状态
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);										//按照指定参数初始化
	//死区可刹车功能配置
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;			 	//运行模式下输出选择
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;			 	//空闲模式下输出选择
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;					//锁定设置
	TIM_BDTRInitStructure.TIM_DeadTime = 0x10;									//死区时间设置
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;						//刹车功能使能
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;			//刹车输入极性
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
																																		//自动输出使能
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);


	/*各种使能打开*/

  	TIM_CtrlPWMOutputs(TIM1,ENABLE);										//MOE 主输出使能

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  						//CH1预装载使能
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  						//CH2预装载使能

	TIM_ARRPreloadConfig(TIM1, ENABLE); 									//使能TIMx重载寄存器ARR

	TIM_Cmd(TIM1, ENABLE);  												//使能TIM1


}

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init_hubu(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	TIM_BDTRInitStructure.TIM_DeadTime = 0x10;

	/*使能TIM1以及PA和PB时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设时钟使能

/***************************************************************
* 名称：   	GPIO初始化
* 描述：	配置TIM1复用输出PWM时用到的I/O
* 通道：	TIM1_CH1
*			TIM1_CH2
*			TIM_CH1N
****************************************************************/

   	/*设置该引脚为复用输出功能,输出TIM1_CH1的PWM脉冲波形*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 				//TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*设置该引脚为复用输出功能,输出TIM1_CH4的PWM脉冲波形*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 				//TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*设置该引脚为复用输出功能,输出TIM_CH1N的PWM脉冲波形*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 				//TIM_CH1N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

/***************************************************************
* 名称：   	Time base configuration
* 描述：	时基初始化
****************************************************************/
	/*当定时器从0计数到arr，为一个定时周期(//80k)*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	/*设置预分频：不分频，即为72MHz*/
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	/*设置时钟分频系数：不分频*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	/*向上计数模式*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

/***************************************************************
* 名称：   	PWM1 Mode configuration
* 描述：	PWM1模式初始化
* 通道：	channel1
*			channel2
****************************************************************/
	/*PWM2 Mode configuration: Channel1*/
	/*配置为PWM模式2*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*设置跳变值，当计数器计数到这个值(CCR1_Val = 0)时，电平发生跳变*/
	TIM_OCInitStructure.TIM_Pulse = 0;
	/*当定时器技术直接小于CCR1_Val时为高电平*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*使能通道1*/
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);


	/*PWM1 Mode configuration: Channel4*/
	/*配置为PWM模式2*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*设置跳变值，当计数器计数到这个值(CCR1_Val = 0)时，电平发生跳变*/
	TIM_OCInitStructure.TIM_Pulse = 0;
	/*当定时器技术直接小于CCR1_Val时为高电平*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*使能通道4*/
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

/***************************************************************
* 名称：   	死区时间测试代码PWM输出配置
****************************************************************/

	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; 				//设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;				//使能互补输出端
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; 				//死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;				//死区后互补端输出状态
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);										//按照指定参数初始化
	//死区可刹车功能配置
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;			 	//运行模式下输出选择
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;			 	//空闲模式下输出选择
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;					//锁定设置
	TIM_BDTRInitStructure.TIM_DeadTime = 0x10;									//死区时间设置
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;						//刹车功能使能
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;			//刹车输入极性
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
																																		//自动输出使能
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);


	/*各种使能打开*/

  	TIM_CtrlPWMOutputs(TIM1,ENABLE);										//MOE 主输出使能

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  						//CH1预装载使能
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  						//CH2预装载使能

	TIM_ARRPreloadConfig(TIM1, ENABLE); 									//使能TIMx重载寄存器ARR

	TIM_Cmd(TIM1, ENABLE);  												//使能TIM1
}

