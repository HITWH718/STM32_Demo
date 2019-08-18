/**********************718����ʵ���ҿ���������*********************
*  ��д��718����ʵ����
*  ƽ̨��STM32F103ZET6
*  ˵�����������ߵ�ˮƽ���ޣ����в���֮�����������½⡣
*		 �����Ҷ࿴�������ֲᡣ     
******************************************************************/

#include "stm32f10x.h"
#include "adc.h"
#include "delay.h"

/**
  * @brief  ��ʼ�� ADC,����ͨ�� 0~3
  * @param  None
  * @retval None
  */
static void ADC_Mode_Init()
{ 
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE ); //ʹ�� ADC1 ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //���� ADC ��Ƶ���� 6 (72M/6=12,ADC ���ʱ�䲻�ܳ��� 14M)
	
	ADC_DeInit(ADC1); //��λ ADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC �����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1; //˳����й���ת���� ADC ͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure); //����ָ���Ĳ�����ʼ������ ADCx
	
	ADC_Cmd(ADC1, ENABLE); //ʹ��ָ���� ADC1
	ADC_ResetCalibration(ADC1); //������λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼����
	ADC_StartCalibration(ADC1); //���� AD У׼
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼����
}

/**
  * @brief  ��ʼ�� ADC ��Ӧ�� GPIO �ܽ�
  * @param  None
  * @retval None
  */
static void ADC_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE ); //ʹ�� GPIOA ʱ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA.4
}

/**
  * @brief  ADC��ʼ������
  * @param  None
  * @retval None
  */
void ADC_Config()
{
	ADC_GPIO_Init();
	ADC_Mode_Init();
}

/**
  * @brief  ��� ADC ֵ
  * @param  Ch ��ͨ��ֵ 0~3
  * @retval None
  */
uint16_t Get_Adc_VAl(uint8_t Ch)
{
	//ͨ�� 3,�������˳��ֵΪ 1,����ʱ��Ϊ 239.5 ����
	ADC_RegularChannelConfig(ADC1, Ch, 1, ADC_SampleTime_55Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 						//ʹ�����ת������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));					//�ȴ�ת������
	return ADC_GetConversionValue(ADC1); 							//�������һ�� ADC1 �������ת�����
}

/**
  * @brief  ����ƽ��ֵ
  * @param  Ch 		��ͨ��ֵ 0~3 
			Times ��ȡTimes�ε�ƽ�� 
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
	return temp_val/Times;		//��ƽ��
}
