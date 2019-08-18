/**********************PWM信号 周期和占空比的计算*********************
* ARR：自动重装载寄存器的值
* CLK_CNT：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
* PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
* 占空比P = CCR/(ARR+1)     
******************************************************************/

#include "TIM.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


/**
  * @brief	通用定时器GPIO配置
  *	@param	无
  *	@retval 无
  */
static void GENERAL_TIM_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO初始化结构体
	//输出比较通道4 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);	//使能外设总线时钟
	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;		//设置GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置输出频率
	GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);	//初始化GPIO
}

/**
  * @brief	高级定时器GPIO配置
  *	@param	无
  *	@retval 无
  */
static void ADVANCED_TIM_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO初始化结构体
	//输出比较通道3、4 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCED_TIM_CH3_GPIO_CLK, ENABLE);	//使能外设总线时钟
	GPIO_InitStructure.GPIO_Pin =  ADVANCED_TIM_CH3_PIN | ADVANCED_TIM_CH4_PIN;	//设置GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置输出频率
	GPIO_Init(ADVANCED_TIM_CH3_PORT, &GPIO_InitStructure);	//初始化GPIO
}

/**
  * @brief	通用定时器PWM模式配置
  *	@param	无
  *	@retval 无
  */
static void GENERAL_TIM_Mode_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//定时器时基结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;	//定时器输出比较结构体
	//使能定时器时钟
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
	
	/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为100K
	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_Period;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	
	// 时钟分频因子 ，配置死区时间需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 设置重复计数器的值为0，不设置重复计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/
	
	//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	//使能输出比较
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//设置初始占空比
	TIM_OCInitStructure.TIM_Pulse = 0;
	//输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//初始化输出比较通道4
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);  
	//使能通道4的CCR4上的预装载寄存器
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);  
	//使能TIM
	TIM_Cmd(GENERAL_TIM, ENABLE);  
}

/**
  * @brief	高级定时器PWM模式配置
  *	@param	无
  *	@retval 无
  */
static void ADVANCED_TIM_Mode_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//定时器时基结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;	//定时器输出比较结构体
	//使能定时器时钟
	ADVANCED_TIM_APBxClock_FUN(ADVANCED_TIM_CLK, ENABLE);
	
	/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为100K
	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCED_TIM_Period;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCED_TIM_Prescaler;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 设置重复计数器的值为0，不设置重复计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCED_TIM, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/
	
	//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	//使能输出比较
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//设置初始占空比
	TIM_OCInitStructure.TIM_Pulse = 0;
	//输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	/**********************通道3*******************************/
	//初始化输出比较通道3
	TIM_OC3Init(ADVANCED_TIM, &TIM_OCInitStructure);  
	//使能通道4的CCR4上的预装载寄存器
	TIM_OC3PreloadConfig(ADVANCED_TIM, TIM_OCPreload_Enable);  	
	
	/**********************通道4*******************************/
	//初始化输出比较通道4
	TIM_OC4Init(ADVANCED_TIM, &TIM_OCInitStructure);  
	//使能通道4的CCR4上的预装载寄存器
	TIM_OC4PreloadConfig(ADVANCED_TIM, TIM_OCPreload_Enable);  
	
	//使能TIM
	TIM_Cmd(ADVANCED_TIM, ENABLE);
	//主输出使能，当使用的是通用定时器，不需要主输出使能
	TIM_CtrlPWMOutputs(ADVANCED_TIM, ENABLE);
}

/**
  * @brief	定时器PWM模式初始化
  *	@param	无
  *	@retval 无
  */
void TIM_PWM_Init()
{
	GENERAL_TIM_GPIO_Config();
	ADVANCED_TIM_GPIO_Config();
	
	GENERAL_TIM_Mode_Config();
	ADVANCED_TIM_Mode_Config();
}

