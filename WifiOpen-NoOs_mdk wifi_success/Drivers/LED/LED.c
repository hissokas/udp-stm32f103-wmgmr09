/******************************
微嵌电子工作室 版权所有
STM32 WIFI 开发板
适用于微嵌电子“灵通壹号WIFI开发板”
详情请登录 http://yuikin.taobao.com
源代码使用协议：本代码仅提供给客户做学习、或者参考WIFI原理之用，你不能在你发布的产品里面
包含本源代码的完整文件或者部分片段），不能出售或者使用本工作室提供的源代码制作相同类型产品，
例如：开发板、学习板，营利性教学讲座等等，否则视为侵权，本工作室保留追究相关法律责任的权利。
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
