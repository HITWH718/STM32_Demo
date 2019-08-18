#include "key.h"

/**
  * @brief  按键初始化（打开对应时钟，配置Key对应GPIO引脚的工作模式）
  * @param  Key：key为枚举体变量，只能是KEY1/2或KEY_ALL
  * @retval 无
  * @example Key_Init(Key1);
  */
void Key_Init(KEY Key)
{
	if(Key==KEY1)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOC ,&GPIO_InitStructure);
	}
	else if(Key==KEY2)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
	else if(Key==KEY_ALL)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOC ,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
}

/**
  * @brief  获取按键状态（判断按键是否按下）
  * @param  Key：key为枚举体变量，只能是KEY1、KEY2或KEY_ALL
  * @retval 无
  * @example Key_Get(KEY1);
  */
uint8_t Key_Get(KEY Key)
{
	if(Key==KEY1)
		return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12);
	else if(Key==KEY2)
		return GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2);
	else
		return 1;
}





