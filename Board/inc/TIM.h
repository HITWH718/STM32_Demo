#ifndef _TIM_H_
#define _TIM_H_

/**********************************注意*****************************************
* TIM1和TIM8为高级定时器，在外设总线APB2上
* TIM2、3、4、5为通用定时器，在外设总线APB1上

* 当使用不同的定时器时，对应的GPIO不一样，需要修改
* 这里使用的TIM8的通道3、4和TIM4的通道4
******************************************************************************/

/*****************高级定时器TIM参数定义，只限TIM1和TIM8***************************/
#define		ADVANCED_TIM					TIM8
#define 	ADVANCED_TIM_APBxClock_FUN		RCC_APB2PeriphClockCmd
#define 	ADVANCED_TIM_CLK				RCC_APB2Periph_TIM8
#define     ADVANCED_TIM_Period            	20000
#define     ADVANCED_TIM_Prescaler         	71

//TIM通道3
#define     ADVANCED_TIM_CH3_GPIO_CLK      	RCC_APB2Periph_GPIOC
#define     ADVANCED_TIM_CH3_PORT          	GPIOC
#define     ADVANCED_TIM_CH3_PIN           	GPIO_Pin_8

//TIM通道4
#define     ADVANCED_TIM_CH4_GPIO_CLK      	RCC_APB2Periph_GPIOC
#define     ADVANCED_TIM_CH4_PORT          	GPIOC
#define     ADVANCED_TIM_CH4_PIN           	GPIO_Pin_9


/*****************通用定时器TIM参数定义，只限TIM2、3、4、5***************************/
#define		GENERAL_TIM						TIM4
#define 	GENERAL_TIM_APBxClock_FUN		RCC_APB1PeriphClockCmd
#define 	GENERAL_TIM_CLK					RCC_APB1Periph_TIM4
#define     GENERAL_TIM_Period            	20000
#define     GENERAL_TIM_Prescaler         	71

//TIM通道4
#define     GENERAL_TIM_CH4_GPIO_CLK      	RCC_APB2Periph_GPIOB
#define     GENERAL_TIM_CH4_PORT          	GPIOB
#define     GENERAL_TIM_CH4_PIN           	GPIO_Pin_9


extern void TIM_PWM_Init(void);

#endif
