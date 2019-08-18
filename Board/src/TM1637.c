/***************************************************************
 * 说明：该数码管使用的芯片是TM1637， 利用IIC通信协议设置显示的内容。
		该例程仅提供了部分功能，详细功能设置请参考TM1637数据手册。
		关于IIC协议：https://baike.baidu.com/item/I2C%E6%80%BB%E7%BA%BF
 ***************************************************************/

/*******************利用IIC使数码管显示的步骤*****************************
*	1. 设置读/写模式
*	2. 设置写入/读取的地址
*	3. 设置写入的数据/读取数据
*	4. 使能显示
*******************利用IIC使数码管显示的步骤*****************************/


#include "TM1637.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

const uint8_t DELAY_TIME = 3;	//IIC通信延时时间:3us

const uint8_t TUBE_TAB[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f,0x77,0x7c,
                           0x39,0x5e,0x79,0x71};	//0~9,A,b,C,d,E,F

						   
/**
  * @brief  数码管使用的GPIO配置
  * @param  无
  * @retval 无
  */
static void Digital_Tube_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	 //使能时钟
	
	GPIO_InitStructure.GPIO_Pin = CLK_PIN | DIO_PIN;		 //CLK和DIO端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //设置推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(TUBE_GPIO, &GPIO_InitStructure);				 //根据设定参数初始化
	
	GPIO_ResetBits(TUBE_GPIO, CLK_PIN | DIO_PIN);			 //初始设置低电平
}

/**
  * @brief  等待IIC应答
  * @param  无
  * @retval 无
  */
static void IIC_Ask()
{
	RESET_CLK();
	delay_us(5);
	while(GPIO_ReadInputDataBit(TUBE_GPIO, DIO_PIN));
	SET_CLK();
	delay_us(DELAY_TIME);
	RESET_DIO();
}

/**
  * @brief  IIC通信开始
  * @param  无
  * @retval 无
  */
static void IIC_Start()
{
	SET_CLK();
	SET_DIO();
	delay_us(DELAY_TIME);
	RESET_DIO();
	RESET_CLK();
}

/**
  * @brief  IIC通信结束
  * @param  无
  * @retval 无
  */
static void IIC_Stop()
{
	RESET_CLK();
	delay_us(DELAY_TIME);
	RESET_DIO();
	delay_us(DELAY_TIME);
	SET_CLK();
	delay_us(DELAY_TIME);
	SET_DIO();
}

/**
  * @brief  发送一个字节
  * @param  Byte：待发送的字节
  * @retval 无
  */
static void Write_Byte(uint8_t Byte)
{
	uint8_t bit;
	for(bit = 0; bit<8; bit++)
	{
		RESET_CLK();
		if(Byte & 0x01)	//取低位
		{
			SET_DIO();
		}
		else
		{
			RESET_DIO();
		}
		delay_us(DELAY_TIME);
		Byte>>=1;
		SET_CLK();
		delay_us(DELAY_TIME);
	}		
}

/**
  * @brief 	数码管开启显示
  * @param  无
  * @retval 无
  */
void Display()
{
	IIC_Start();
	Write_Byte(0x8f);	//开显示，最大亮度
	IIC_Ask();
	IIC_Stop();
}

/**
  * @brief  数码管关闭显示
  * @param  无
  * @retval 无
  */
void Close_Display()
{
	IIC_Start();
	Write_Byte(0x80);
	IIC_Ask();
	IIC_Stop();
}

/**
  * @brief  清除数码管上一次的内容， 数码管有记忆功能，会保留上一次的设置
  * @param  无
  * @retval 无
  */
void Clear_Display()
{
	Display_Number(0, -1, POINT_OFF);
	Display_Number(1, -1, POINT_OFF);
	Display_Number(2, -1, POINT_OFF);
	Display_Number(3, -1, POINT_OFF);
}

/**
  * @brief  数码管显示一个数字
  * @param  Pos：显示的位置（0-3）
  * @param  Number：显示的数字（0-15）
  * @param  Point：是否显示分隔符.
					POINT_ON:	显示
					POINT_OFF:	不显示
  * @retval 无
  */
void Display_Number(uint8_t Pos, int8_t Number, uint8_t Point)
{
	uint8_t point_data;
	
	if(Pos >3)
	{
		Pos = 0;
	}
	
	if(Number >15)
	{
		Number = 15;
	}
	
	IIC_Start();
	Write_Byte(ADDRESS_FIXED); //设置固定地址
	IIC_Ask();
	IIC_Stop();
	
	IIC_Start();
	Write_Byte(ADRESS_BASE + Pos);	//设置写入地址
	IIC_Ask();
	
	if(Point == POINT_ON)
		point_data = 0x80;
	else
		point_data = 0;
	
	if(Number == -1)
		Write_Byte(0x00 + point_data);
	else
		Write_Byte(TUBE_TAB[Number] + point_data);
	
	IIC_Ask();
	IIC_Stop();	
	Display();
}

/**
  * @brief  数码管显示一组数字
  * @param  Number：显示的数组（0-15）
  * @param  Size： 数组的大小（0-3）
  * @param  Point：是否显示分隔符
					POINT_ON:	显示
					POINT_OFF:	不显示
  * @retval 无
  */
void Display_Array(uint8_t Number[], uint8_t Size, uint8_t Point)
{
	uint8_t num;
	uint8_t i;
	uint8_t point_data;
	
	if(Size > 4)
	{
		Size = 4;
	}
	
	IIC_Start();
	Write_Byte(ADDRESS_AUTO); //连续写入，地址自动加一
	IIC_Ask();
	IIC_Stop();
	
	IIC_Start();
	Write_Byte(ADRESS_BASE);	//设置为基地址
	IIC_Ask();
	
	if(Point == POINT_ON)
		point_data = 0x80;
	else
		point_data = 0;
	
	for(i=0; i<Size; i++)
	{
		num = Number[i];
		if(num >15)
			num = 15;
		Write_Byte(TUBE_TAB[num] + point_data);
		IIC_Ask();
	}	
	IIC_Stop();
	Display();
}

/**
  * @brief  数码管配置初始化
  * @param  无
  * @retval 无
  */
void Digital_Tube_Init()
{
	Digital_Tube_GPIO_Config();
	delay_init();
}
