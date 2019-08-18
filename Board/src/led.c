#include "led.h"

/**
  * @brief  LED初始化
  * @param  Led：LED灯（LED1 - LED3）
  * @retval 无
  * @example Led_Init(LED1);
  */
void Led_Init(LED Led)
{
	if(Led==LED1)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
	else if(Led==LED2)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
	else if(Led==LED3)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
	else if(Led==LED_ALL)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
}

/**
  * @brief  设置LED状态
  * @param  Led：LED灯（LED1 - LED3）
  * @param  State： 设置的状态（0、1）
  * @retval 无
  * @example Led_Init(LED1,1);
  */
void Led_Set(LED Led, uint8_t State)
{
	if(Led==LED1)
	{
		if(State==0)
			GPIO_ResetBits(GPIOE,GPIO_Pin_4);
		else if(State==1)
			GPIO_SetBits(GPIOE,GPIO_Pin_4);
	}
	else if(Led==LED2)
	{
		if(State==0)
			GPIO_ResetBits(GPIOE,GPIO_Pin_3);
		else if(State==1)
			GPIO_SetBits(GPIOE,GPIO_Pin_3);
	}
	else if(Led==LED3)
	{
		if(State==0)
			GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		else if(State==1)
			GPIO_SetBits(GPIOE,GPIO_Pin_5);
	}
	else if(Led==LED_ALL)
	{
		if(State==0)
			GPIO_ResetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
		else if(State==1)
			GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	}
}
