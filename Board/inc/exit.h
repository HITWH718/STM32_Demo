#ifndef _EXIT_H_
#define _EXIT_H_

//EXIT GPIO defination
#define EXIT_GPIO_CLK 						RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE
#define DEBUG_EXIT_PIN1     			GPIO_Pin_12    
#define DEBUG_EXIT_PORT1         	GPIOC									//KEY1
#define DEBUG_EXIT_PIN2     			GPIO_Pin_2    
#define DEBUG_EXIT_PORT2         	GPIOE									//KEY2

//EXIT defination
#define DEBUG_EXIT_PORTSOURCE1   	GPIO_PortSourceGPIOC
#define DEBUG_EXIT_PINSOURCE1    	GPIO_PinSource12
#define DEBUG_EXIT_LINE1         	EXTI_Line12    				//KEY1
#define DEBUG_EXIT_PORTSOURCE2   	GPIO_PortSourceGPIOE
#define DEBUG_EXIT_PINSOURCE2    	GPIO_PinSource2
#define DEBUG_EXIT_LINE2         	EXTI_Line2    				//KEY2

//NVIC definetion
#define DEBUG_EXIT_IRQ1        		EXTI15_10_IRQn
#define KEY_IRQHandle1						EXTI15_10_IRQHandler  //KEY1
#define DEBUG_EXIT_IRQ2        		EXTI2_IRQn
#define KEY_IRQHandle2						EXTI2_IRQHandler  		//KEY2

//KEY defination
#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12) 	//∂¡»°KEY1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) 	//∂¡»°KEY2  

void EXIT_Config(void);

#endif
