#include "timerx.h"
#include "uIP_App.h"
#include "uip.h"
#include "tapdev.h"
#include "clock-arch.h"
#include "timer.h"
#include "uip_arp.h"
#include "dhcps.h"
#include "lingtone_udp_app.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
u32 uip_timer=0;//uip ��ʱ����ÿ10ms����1.
u32 ad_timer=0;
extern unsigned char zan;
unsigned char li;
extern unsigned char IT_flag;
extern unsigned char TIM_flag;
extern unsigned char ad_flag;
extern unsigned long chang_value_fast;
extern float Uoltage_fast;
extern volatile unsigned int time_per_data;
extern volatile u8 num_save;
extern unsigned long buf_flag ;
//��ʱ��6�жϷ������	 
void TIM6_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		LED_Flash(LED4);
		//uip_timer++;
		ad_timer++;
		if(ad_timer >= time_per_data)
		{
			ad_flag = 1; 
			ad_timer = 0;
		}
	} 
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 			    		  			    	    
}
 
//������ʱ��6�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM6_Int_Init(u16 arr,u16 psc)
{	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  100Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig( TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//ʹ�ܶ�ʱ��6���´����ж�
	
	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	/*NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;  //TIM3�ж�EXTI2_IRQ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
  
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4); //�����жϹܽ�
	EXTI_InitStructure.EXTI_Line = EXTI_Line4 ; //ѡ���ж���·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж����󣬷��¼�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  //ʹ��
	EXTI_Init(&EXTI_InitStructure); 
	
	EXTI_GenerateSWInterrupt(EXTI_Line4);*/
}



