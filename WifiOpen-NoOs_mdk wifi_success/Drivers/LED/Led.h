/******************************
微嵌电子工作室 版权所有
STM32 WIFI 开发板
适用于微嵌电子“灵通壹号WIFI开发板”
详情请登录 http://yuikin.taobao.com
源代码使用协议：本代码仅提供给客户做学习、或者参考WIFI原理之用，你不能在你发布的产品里面
包含本源代码的完整文件或者部分片段），不能出售或者使用本工作室提供的源代码制作相同类型产品，
例如：开发板、学习板，营利性教学讲座等等，否则视为侵权，本工作室保留追究相关法律责任的权利。
*******************************/

#ifndef __LED_H__
#define __LED_H__
//
// Macros definitions
//

// Alias name
#define LED1			GPIO_Pin_0
#define LED2			GPIO_Pin_1
#define LED3			GPIO_Pin_2
#define LED4			GPIO_Pin_3

#define ADCS1     GPIO_Pin_11
#define ADCS2     GPIO_Pin_13
#define ADCS3     GPIO_Pin_8

#define OPCS1     GPIO_Pin_11
#define OPCS2     GPIO_Pin_10
#define OPCS3     GPIO_Pin_15
#define OPCS4     GPIO_Pin_14

//
// Exported functions declare
//
void LED_Init (void);
void LED_TurnOn (unsigned short int usLed);
void LED_TurnOff (unsigned short int usLed);
void LED_Flash (unsigned short int usLed);

#endif // __LED_H__

