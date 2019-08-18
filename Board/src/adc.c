/**********************718创新实验室开发板例程*********************
*  编写：718创新实验室
*  平台：STM32F103ZET6
*  说明：由于作者的水平有限，若有不足之处，还请大家谅解。
*		 建议大家多看看数据手册。     
******************************************************************/

#include "stm32f10x.h"
#include "adc.h"
#include "delay.h"

/**
  * @brief  初始化 ADC,开启通道 0~3
  * @param  None
  * @retval None
  */
static void ADC_Mode_Init()
{ 
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE ); //使能 ADC1 通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置 ADC 分频因子 6 (72M/6=12,ADC 最大时间不能超过 14M)
	
	ADC_DeInit(ADC1); //复位 ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC 独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的 ADC 通道的数目
	ADC_Init(ADC1, &ADC_InitStructure); //根据指定的参数初始化外设 ADCx
	
	ADC_Cmd(ADC1, ENABLE); //使能指定的 ADC1
	ADC_ResetCalibration(ADC1); //开启复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束
	ADC_StartCalibration(ADC1); //开启 AD 校准
	while(ADC_GetCalibrationStatus(ADC1)); //等待校准结束
}

/**
  * @brief  初始化 ADC 对应的 GPIO 管脚
  * @param  None
  * @retval None
  */
static void ADC_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE ); //使能 GPIOA 时钟
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化 GPIOA.4
}

/**
  * @brief  ADC初始化函数
  * @param  None
  * @retval None
  */
void ADC_Config()
{
	ADC_GPIO_Init();
	ADC_Mode_Init();
}

/**
  * @brief  获得 ADC 值
  * @param  Ch ：通道值 0~3
  * @retval None
  */
uint16_t Get_Adc_VAl(uint8_t Ch)
{
	//通道 3,规则采样顺序值为 1,采样时间为 239.5 周期
	ADC_RegularChannelConfig(ADC1, Ch, 1, ADC_SampleTime_55Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 						//使能软件转换功能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));					//等待转换结束
	return ADC_GetConversionValue(ADC1); 							//返回最近一次 ADC1 规则组的转换结果
}

/**
  * @brief  计算平均值
  * @param  Ch 		：通道值 0~3 
			Times ：取Times次的平均 
  * @retval None
  */
uint16_t Get_Adc_Ave(uint8_t Ch,uint8_t Times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<Times;t++)
	{ 
		temp_val+=Get_Adc_VAl(Ch);
		delay_ms(5);
	}
	return temp_val/Times;		//求平均
}
