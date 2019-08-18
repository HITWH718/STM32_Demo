#ifndef _USART_H_
#define _USART_H_
#include "stdio.h"	//为了重定向printf()

#define USART_REC_LEN  			10  	//定义最大接收字节数 200 

#define APB 								2      //选择初始化的时钟，2：APB2     1：APB1
#define APB_CLK             APB-1  //转换成宏定义
#if APB_CLK
#define USART_CLK                RCC_APB2Periph_USART1   //选择APB2上的外设（这里只有USART1）
#else
#define USART_CLK                RCC_APB1Periph_UART4   //选择APB1上的外设（USART2,USART3...）
#endif
#define USART_GPIO_CLK 					RCC_APB2Periph_GPIOA				//USART对应的管脚的时钟
#define DEBUG_USART 				 	USART1											//USART号
#define DEBUG_USART_GPIO_PORT		 	GPIOA                      //USART对应的管脚的GPIO外设号
#define DEBUG_USART_TX 				 	GPIO_Pin_9        					
#define DEBUG_USART_RX 				 	GPIO_Pin_10								//GPIO对应的针脚
#define DEBUG_USART_IRQn			 	USART1_IRQn                //USART对应的中断号
#define DEBUG_USART_IRQHandler   		USART1_IRQHandler          //USART对应的中断服务函数

extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 

void USART_Config(void);
#endif


