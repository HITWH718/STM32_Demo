#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x.h"

/////////////////////按键宏定义////////////////////////
#define KEY_UP_ADC 0.1
#define KEY_UP 1

#define KEY_DOWN_ADC 1.9
#define KEY_DOWN 2

#define KEY_LEFT_ADC 1.16
#define KEY_LEFT 3

#define KEY_RIGHT_ADC 1.88
#define KEY_RIGHT 4

#define KEY_ENDER_ADC 2.6
#define KEY_ENDER 5

extern void ADC_Config(void);			//adc初始化
extern uint16_t Get_Adc_VAl(uint8_t Ch);					//获取一次adc值
extern uint16_t Get_Adc_Ave(uint8_t Ch,uint8_t Times);		//获取任意次adc的平均值
extern int Key_detect(void);						//检测按键

#endif
