#ifndef _TM1637_H_
#define _TM1637_H_

#include "stm32f10x.h"

//TM1637配置参数
#define ADDRESS_AUTO 0x40	//地址自动加1
#define ADDRESS_FIXED 0x44	//固定地址

#define BRIGHT_LEVEL 0x80	//最低亮度

#define DISPLAY_ON 0x88		//开启显示
#define DISPLAY_OFF 0x80	//关闭显示

#define ADRESS_BASE 0xc0	//数码管基地址

//数码管对应的GPIO
#define TUBE_GPIO GPIOB		
#define CLK_PIN	GPIO_Pin_10
#define DIO_PIN GPIO_Pin_11

//设置CLK高电平或者低电平
#define SET_CLK()	GPIO_SetBits(TUBE_GPIO, CLK_PIN)
#define RESET_CLK()	GPIO_ResetBits(TUBE_GPIO, CLK_PIN)

//设置DIO高电平或者低电平
#define SET_DIO()	GPIO_SetBits(TUBE_GPIO, DIO_PIN)
#define RESET_DIO() GPIO_ResetBits(TUBE_GPIO, DIO_PIN)


//数码管分隔符
enum point
{
	POINT_OFF,
	POINT_ON
};

extern void Digital_Tube_Init(void);
extern void Display_Number(uint8_t Pos, int8_t Number, uint8_t Point);
extern void Display_Array(uint8_t Number[], uint8_t Size, uint8_t Point);
extern void Display(void);
extern void Close_Display(void);
extern void Clear_Display(void);

#endif
