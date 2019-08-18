#include "stm32f10x.h"
#include "exit.h"
#include "usart.h"

/**
  *@brief  gpio����
  *@param  None
  *@retval None
  */
static void Key_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(EXIT_GPIO_CLK, ENABLE);		//ʹ��PORTC,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = DEBUG_EXIT_PIN1;		//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//PA0��������������	  
	GPIO_Init(DEBUG_EXIT_PORT1, &GPIO_InitStructure); //��ʼ��GPIOC12
	
	GPIO_InitStructure.GPIO_Pin  = DEBUG_EXIT_PIN2;		//KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//���ó���������
 	GPIO_Init(DEBUG_EXIT_PORT2, &GPIO_InitStructure);	//��ʼ��GPIOE2
}

/**
  *@brief  �ⲿ�ж�����
  *@param  None
  *@retval None
  */
static void Key_EXIT_Config()
{
	EXTI_InitTypeDef EXIT_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);         		//��AFIO�Ķ˿�ʱ��

	//GPIOC.12	  �ж����Լ��жϳ�ʼ������ �½��ش���    //KEY1
	GPIO_EXTILineConfig(DEBUG_EXIT_PORTSOURCE1, DEBUG_EXIT_PINSOURCE1);//ѡ�������� : Selects the GPIO pin used as EXTI Line
  	EXIT_InitStruct.EXTI_Line = DEBUG_EXIT_LINE1;                       //ʹ���ź���
  	EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	               	//�����ж�
  	EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;            	//�½����ж�
  	EXIT_InitStruct.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXIT_InitStruct);	 			//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOE.2	  �ж����Լ��жϳ�ʼ������ �½��ش���    //KEY2
  	GPIO_EXTILineConfig(DEBUG_EXIT_PORTSOURCE2, DEBUG_EXIT_PINSOURCE2);    //ѡ��������
	EXIT_InitStruct.EXTI_Line = DEBUG_EXIT_LINE2;	                       	//ʹ���ź���
	EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	                   //�����ж�
  	EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;               		//�½����ж�
  	EXIT_InitStruct.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXIT_InitStruct);				//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}

/**
  *@brief  �����ж�����
  *@param  None
  *@retval None
  */
static void Key_NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);         	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannel = DEBUG_EXIT_IRQ1;			//EXTI3_IRQn;		//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;			//�����ȼ�1 
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStruct);  	  							//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    NVIC_InitStruct.NVIC_IRQChannel = DEBUG_EXIT_IRQ2;			//EXTI2_IRQn;		//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStruct);								//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

/**
  *@brief  ��ʼ�������ж�
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
  *@brief  �����жϷ�����
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
