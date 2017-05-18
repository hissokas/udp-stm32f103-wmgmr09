/******************************
΢Ƕ���ӹ����� ��Ȩ����
STM32 WIFI ������
������΢Ƕ���ӡ���ͨҼ��WIFI�����塱
�������¼ http://yuikin.taobao.com
Դ����ʹ��Э�飺��������ṩ���ͻ���ѧϰ�����߲ο�WIFIԭ��֮�ã��㲻�����㷢���Ĳ�Ʒ����
������Դ����������ļ����߲���Ƭ�Σ������ܳ��ۻ���ʹ�ñ��������ṩ��Դ����������ͬ���Ͳ�Ʒ��
���磺�����塢ѧϰ�壬Ӫ���Խ�ѧ�����ȵȣ�������Ϊ��Ȩ���������ұ���׷����ط������ε�Ȩ����
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

