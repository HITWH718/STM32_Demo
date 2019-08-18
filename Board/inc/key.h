#ifndef _KEY_H_
#define _KEY_H_ 			   
#include "stm32f10x.h"  

//用枚举体进行封装
typedef enum 
{
	KEY1,
	KEY2,
	KEY_ALL
}KEY;


extern void Key_Init(KEY Key);
extern uint8_t Key_Get(KEY Key);

#endif
