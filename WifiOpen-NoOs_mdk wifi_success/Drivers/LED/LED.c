/******************************
΢Ƕ���ӹ����� ��Ȩ����
STM32 WIFI ������
������΢Ƕ���ӡ���ͨҼ��WIFI�����塱
�������¼ http://yuikin.taobao.com
Դ����ʹ��Э�飺��������ṩ���ͻ���ѧϰ�����߲ο�WIFIԭ��֮�ã��㲻�����㷢���Ĳ�Ʒ����
������Դ����������ļ����߲���Ƭ�Σ������ܳ��ۻ���ʹ�ñ��������ṩ��Դ����������ͬ���Ͳ�Ʒ��
���磺�����塢ѧϰ�壬Ӫ���Խ�ѧ�����ȵȣ�������Ϊ��Ȩ���������ұ���׷����ط������ε�Ȩ����
*******************************/

#include "Project.h"

void LED_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED3 | LED4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init (GPIOA, &GPIO_InitStructure);
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = ADCS1 | ADCS2 | ADCS3 | OPCS3 | OPCS4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init (GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = OPCS1 | OPCS2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init (GPIOB, &GPIO_InitStructure);
	
	
	// Turn off all LED first
	LED_TurnOff (LED1);
	LED_TurnOff (LED2);
	LED_TurnOff (LED3);
	LED_TurnOff (LED4);
	
  GPIO_SetBits (GPIOB, OPCS1) ;	
	GPIO_SetBits (GPIOB, OPCS2) ;	
	GPIO_SetBits (GPIOE, ADCS3) ;	
  GPIO_SetBits (GPIOE, ADCS2) ;

	//return TRUE;
}

void LED_TurnOn (unsigned short int usLed)
{
	GPIO_ResetBits (GPIOD, usLed);

	//return TRUE;
}

void LED_TurnOff (unsigned short int usLed)
{
	GPIO_SetBits (GPIOD, usLed);

	//return TRUE;
}

void LED_Flash (UINT16 usLed)
{
	if (GPIO_ReadOutputDataBit(GPIOA, usLed) == Bit_RESET)
		GPIO_SetBits(GPIOA, usLed);
	else
		GPIO_ResetBits(GPIOA, usLed);

//	return TRUE;
}
