/***************************************************************
 * ˵�����������ʹ�õ�оƬ��TM1637�� ����IICͨ��Э��������ʾ�����ݡ�
		�����̽��ṩ�˲��ֹ��ܣ���ϸ����������ο�TM1637�����ֲᡣ
		����IICЭ�飺https://baike.baidu.com/item/I2C%E6%80%BB%E7%BA%BF
 ***************************************************************/

/*******************����IICʹ�������ʾ�Ĳ���*****************************
*	1. ���ö�/дģʽ
*	2. ����д��/��ȡ�ĵ�ַ
*	3. ����д�������/��ȡ����
*	4. ʹ����ʾ
*******************����IICʹ�������ʾ�Ĳ���*****************************/


#include "TM1637.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

const uint8_t DELAY_TIME = 3;	//IICͨ����ʱʱ��:3us

const uint8_t TUBE_TAB[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f,0x77,0x7c,
                           0x39,0x5e,0x79,0x71};	//0~9,A,b,C,d,E,F

						   
/**
  * @brief  �����ʹ�õ�GPIO����
  * @param  ��
  * @retval ��
  */
static void Digital_Tube_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	 //ʹ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = CLK_PIN | DIO_PIN;		 //CLK��DIO�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(TUBE_GPIO, &GPIO_InitStructure);				 //�����趨������ʼ��
	
	GPIO_ResetBits(TUBE_GPIO, CLK_PIN | DIO_PIN);			 //��ʼ���õ͵�ƽ
}

/**
  * @brief  �ȴ�IICӦ��
  * @param  ��
  * @retval ��
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
  * @brief  IICͨ�ſ�ʼ
  * @param  ��
  * @retval ��
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
  * @brief  IICͨ�Ž���
  * @param  ��
  * @retval ��
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
  * @brief  ����һ���ֽ�
  * @param  Byte�������͵��ֽ�
  * @retval ��
  */
static void Write_Byte(uint8_t Byte)
{
	uint8_t bit;
	for(bit = 0; bit<8; bit++)
	{
		RESET_CLK();
		if(Byte & 0x01)	//ȡ��λ
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
  * @brief 	����ܿ�����ʾ
  * @param  ��
  * @retval ��
  */
void Display()
{
	IIC_Start();
	Write_Byte(0x8f);	//����ʾ���������
	IIC_Ask();
	IIC_Stop();
}

/**
  * @brief  ����ܹر���ʾ
  * @param  ��
  * @retval ��
  */
void Close_Display()
{
	IIC_Start();
	Write_Byte(0x80);
	IIC_Ask();
	IIC_Stop();
}

/**
  * @brief  ����������һ�ε����ݣ� ������м��书�ܣ��ᱣ����һ�ε�����
  * @param  ��
  * @retval ��
  */
void Clear_Display()
{
	Display_Number(0, -1, POINT_OFF);
	Display_Number(1, -1, POINT_OFF);
	Display_Number(2, -1, POINT_OFF);
	Display_Number(3, -1, POINT_OFF);
}

/**
  * @brief  �������ʾһ������
  * @param  Pos����ʾ��λ�ã�0-3��
  * @param  Number����ʾ�����֣�0-15��
  * @param  Point���Ƿ���ʾ�ָ���.
					POINT_ON:	��ʾ
					POINT_OFF:	����ʾ
  * @retval ��
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
	Write_Byte(ADDRESS_FIXED); //���ù̶���ַ
	IIC_Ask();
	IIC_Stop();
	
	IIC_Start();
	Write_Byte(ADRESS_BASE + Pos);	//����д���ַ
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
  * @brief  �������ʾһ������
  * @param  Number����ʾ�����飨0-15��
  * @param  Size�� ����Ĵ�С��0-3��
  * @param  Point���Ƿ���ʾ�ָ���
					POINT_ON:	��ʾ
					POINT_OFF:	����ʾ
  * @retval ��
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
	Write_Byte(ADDRESS_AUTO); //����д�룬��ַ�Զ���һ
	IIC_Ask();
	IIC_Stop();
	
	IIC_Start();
	Write_Byte(ADRESS_BASE);	//����Ϊ����ַ
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
  * @brief  ��������ó�ʼ��
  * @param  ��
  * @retval ��
  */
void Digital_Tube_Init()
{
	Digital_Tube_GPIO_Config();
	delay_init();
}
