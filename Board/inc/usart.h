#ifndef _USART_H_
#define _USART_H_
#include "stdio.h"	//Ϊ���ض���printf()

#define USART_REC_LEN  			10  	//�����������ֽ��� 200 

#define APB 								2      //ѡ���ʼ����ʱ�ӣ�2��APB2     1��APB1
#define APB_CLK             APB-1  //ת���ɺ궨��
#if APB_CLK
#define USART_CLK                RCC_APB2Periph_USART1   //ѡ��APB2�ϵ����裨����ֻ��USART1��
#else
#define USART_CLK                RCC_APB1Periph_UART4   //ѡ��APB1�ϵ����裨USART2,USART3...��
#endif
#define USART_GPIO_CLK 					RCC_APB2Periph_GPIOA				//USART��Ӧ�Ĺܽŵ�ʱ��
#define DEBUG_USART 				 	USART1											//USART��
#define DEBUG_USART_GPIO_PORT		 	GPIOA                      //USART��Ӧ�Ĺܽŵ�GPIO�����
#define DEBUG_USART_TX 				 	GPIO_Pin_9        					
#define DEBUG_USART_RX 				 	GPIO_Pin_10								//GPIO��Ӧ�����
#define DEBUG_USART_IRQn			 	USART1_IRQn                //USART��Ӧ���жϺ�
#define DEBUG_USART_IRQHandler   		USART1_IRQHandler          //USART��Ӧ���жϷ�����

extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 

void USART_Config(void);
#endif


