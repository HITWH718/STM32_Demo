#include "stm32f10x.h"
#include "exit.h"
#include "usart.h"

/**
  *@brief  gpio配置
  *@param  None
  *@retval None
  */
static void Key_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(EXIT_GPIO_CLK, ENABLE);		//使能PORTC,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = DEBUG_EXIT_PIN1;		//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//PA0设置上拉成输入	  
	GPIO_Init(DEBUG_EXIT_PORT1, &GPIO_InitStructure); //初始化GPIOC12
	
	GPIO_InitStructure.GPIO_Pin  = DEBUG_EXIT_PIN2;		//KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//设置成上拉输入
 	GPIO_Init(DEBUG_EXIT_PORT2, &GPIO_InitStructure);	//初始化GPIOE2
}

/**
  *@brief  外部中断配置
  *@param  None
  *@retval None
  */
static void Key_EXIT_Config()
{
	EXTI_InitTypeDef EXIT_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);         		//打开AFIO的端口时钟

	//GPIOC.12	  中断线以及中断初始化配置 下降沿触发    //KEY1
	GPIO_EXTILineConfig(DEBUG_EXIT_PORTSOURCE1, DEBUG_EXIT_PINSOURCE1);//选择输入线 : Selects the GPIO pin used as EXTI Line
  	EXIT_InitStruct.EXTI_Line = DEBUG_EXIT_LINE1;                       //使能信号线
  	EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	               	//产生中断
  	EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;            	//下降沿中断
  	EXIT_InitStruct.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXIT_InitStruct);	 			//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOE.2	  中断线以及中断初始化配置 下降沿触发    //KEY2
  	GPIO_EXTILineConfig(DEBUG_EXIT_PORTSOURCE2, DEBUG_EXIT_PINSOURCE2);    //选择输入线
	EXIT_InitStruct.EXTI_Line = DEBUG_EXIT_LINE2;	                       	//使能信号线
	EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	                   //产生中断
  	EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;               		//下降沿中断
  	EXIT_InitStruct.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXIT_InitStruct);				//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
}

/**
  *@brief  按键中断配置
  *@param  None
  *@retval None
  */
static void Key_NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);         	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_InitStruct.NVIC_IRQChannel = DEBUG_EXIT_IRQ1;			//EXTI3_IRQn;		//使能按键KEY1所在的外部中断通道
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;			//子优先级1 
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  	NVIC_Init(&NVIC_InitStruct);  	  							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    NVIC_InitStruct.NVIC_IRQChannel = DEBUG_EXIT_IRQ2;			//EXTI2_IRQn;		//使能按键KEY2所在的外部中断通道
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;			//子优先级2
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  	NVIC_Init(&NVIC_InitStruct);								//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

/**
  *@brief  初始化按键中断
  *@param  None
  *@retval None
  */
void EXIT_Config()
{
	Key_GPIO_Config();
	Key_EXIT_Config();
	Key_NVIC_Config();
} 

/**
  *@brief  按键中断服务函数
  *@param  None
  *@retval None
  **/
void KEY_IRQHandle1(void)
{
	if(EXTI_GetITStatus (DEBUG_EXIT_LINE1) != RESET)
	{
		printf("up\n");
		
		EXTI_ClearITPendingBit(DEBUG_EXIT_LINE1);
	}
}
