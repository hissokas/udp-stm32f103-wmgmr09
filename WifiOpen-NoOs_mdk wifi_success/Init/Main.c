/******************************
微嵌电子工作室 版权所有
STM32 WIFI 开发板
适用于微嵌电子“灵通壹号WIFI开发板”
详情请登录 http://yuikin.taobao.com
源代码使用协议：本代码仅提供给客户做学习、或者参考WIFI原理之用，你不能在你发布的产品里面
包含本源代码的完整文件或者部分片段），不能出售或者使用本工作室提供的源代码制作相同类型产品，
例如：开发板、学习板，营利性教学讲座等等，否则视为侵权，本工作室保留追究相关法律责任的权利。
*******************************/

//
// Created by csword 2011/01/13
// All rights reserved
//
#include "AD.h"
#include "Project.h"
#include  "timerx.h"
#include "uip.h"
#include "uIP_App.h"
#include "tapdev.h"
#include "clock-arch.h"
#include "lingtone_app.h"
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	

unsigned int data_per_pack = 100;
volatile unsigned int time_per_data = 25;

float Uoltage_fast;
float Uoltage_slow;
unsigned long chang_value_fast;
unsigned long chang_value_slow;
unsigned char counter = 0;
unsigned char zan;
unsigned char IT_flag=0;
unsigned char TIM_flag=0;
unsigned char IT_send=0;
unsigned char ad_flag = 0;
unsigned char data_send = 0;
u8 tcp_client_databuf_send[3][305];
u8 tcp_client_databuf_recv[20];
unsigned long buf_flag = 4;
u8 mode = 'R';
u8 num_send = 0;
volatile u8 num_save = 0;
u8 num_sum = 5;
u8 ts_set,ts_now;
u8 send_count = 100;

UCHAR ucNetworkPktBuffer[KBytes(2)];
volatile unsigned int uIP_RunTime;//__IO == volatile

void main(void)
{
	BOOL bStatus;

	// Bsp Init
	Set_System();
	TraceInit();

#ifdef BUILD_FOR_WRITEFIRMWARE
	If_WriteFirmware ();
	while (1);
#endif

	SysClockInit();
	Board_Init ();

	// Drivers init
	Flash_Init();
	Buzzer_Init ();
	Relay_Init ();
	LED_Init ();
	Console_Init ();
	SPI2_Init();
	SPI2_SetSpeed(2);
  AD7176_init();
	max9939_init(); 
	Console_Print("WiFi Hello world!!\r\n");
	
	// Wifi start!

	bStatus = Wlan_Init(g_pWlanAdapter);
	if (!bStatus) {
		TRACE("Wlan init failed!!\r\n");
		return;
	}

	// uIP Init
	uIP_Net_Init();

	if (1) { // Create default profile
		UCHAR ucIsNewDevice;
	
		Flash_Read(WIFI_PROFILE_ADDR + MEMBER_OFFSET(WiFiProfile, ucIsNewDevice), (PBYTE)&ucIsNewDevice, sizeof(UCHAR));

		if (ucIsNewDevice != 'N') {
			ucIsNewDevice = 'N';
			Flash_Write(WIFI_PROFILE_ADDR + MEMBER_OFFSET(WiFiProfile, ucIsNewDevice), (PBYTE)&ucIsNewDevice, sizeof(UCHAR));
		
			// Config network options
		}
	}
			WiFiCmd_RunCommand (WIFI_CMD_SET_IPADDR);
			WiFiCmd_RunCommand ("set subnet 255.255.255.0");
			WiFiCmd_RunCommand ("set gatewayipaddr 192.168.0.1");
	WiFiCmd_RunCommand ("set ssid liujianlei");
	WiFiCmd_RunCommand ("set networktype infra");
	WiFiCmd_RunCommand ("set authmode open");
	WiFiCmd_RunCommand ("set encrymode disable");
	//WiFiCmd_RunCommand ("set keyascii 1234567890");
	WiFiCmd_RunCommand ("ctrl connect");
	
  TIM6_Int_Init(199,71); //5khz

	while (1) 
	{  
		//
 		UipPro();
		if(mode != 'P')
		{
			if(ad_flag == 1) 
			{
				AD7176_fast();
				ad_flag = 0 ;	
			}	
			if(IT_send)
			{
				IT_send = 0;
				UipSend();
			}
		}
	}
}

