#include "Project.h"

#include "uIP_App.h"
#include "uip.h"
#include "tapdev.h"
#include "clock-arch.h"
#include "timer.h"
#include "uip_arp.h"
#include "dhcps.h"
#include "lingtone_udp_app.h"
#include "tcp_demo.h"
extern unsigned char IT_send;
unsigned char zan1=0;
/*uIP  stack*/
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
//unsigned char zan1;


void UipSend(void)
{
	 uip_udp_periodic(2);						 
						//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
						//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
	 if(uip_len>0)
	 {
			uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
			tapdev_send();//发送数据到以太网
	 }
}
/*******************************************************************************
*   函数名：UipPro
*   输  入:
*   输  出:
*   功能说明：中断触发读取网络接收缓存
*   注意：当使用uCOS时，不适合用中断。请务必谨记。
********************************************************************/
void UipPro(void)
{
	u8 i;
	//u8 tcp_server_tsta=0xff;
	//u8 tcp_client_tsta=0xff;
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok=0;	 
		
	if(timer_ok==0)//仅初始化一次
	{
		timer_ok = 1;
	  //timer_set(&periodic_timer,CLOCK_SECOND/100);  //创建1个0.01秒的定时器 
	  //timer_set(&arp_timer,CLOCK_SECOND*10);	   	//创建1个10秒的定时器 
	}	
	if(WIFI_GET_FLAG(g_pWlanAdapter, FLAG_PKTRCVED)) 
	{
		WIFI_CLR_FLAG(g_pWlanAdapter, FLAG_PKTRCVED);  //清除标识位
	  uip_len = tapdev_read();    //从网络设备读取一个IP包,返回数据长度
		if(uip_len > 0) //收到数据时长度会变化
		{          
			/* 处理IP数据包(只有校验通过的IP包才会被接收) */
		 	if(BUF->type == htons(UIP_ETHTYPE_IP)) //收到IP包 //IP包校验
			{ 
				uip_arp_ipin();        //去除以太网头结构，更新ARP表
				uip_input();           //IP包处理
				/*
				    因为uip_input();函数调用了回调函数UIP_APPCALL(),即tcp_server_appcall
				    当上面的函数执行后，如果回调函数需要发送数据，则全局变量 uip_len > 0
				    需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)
				*/
				if(uip_len > 0)  //有带外回应数据
				{      
					uip_arp_out();      //加以太网头结构，在主动连接时可能要构造ARP请求
					tapdev_send();      //发送数据到以太网（设备驱动程序）
				}
			}
			/* 处理arp报文 */
			else if(BUF->type == htons(UIP_ETHTYPE_ARP)) 
			{ //是ARP请求包
				uip_arp_arpin();        //如是是ARP回应，更新ARP表；如果是请求，构造回应数据包
				/*
				    当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
				    需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)
				*/
				if(uip_len > 0)         //是ARP请求，要发送回应
				{      
					tapdev_send();      //发ARP回应到以太网上
				}
			}
		}	
		
		}
	
	    /*if(timer_expired(&arp_timer)) 
      {
				timer_reset(&arp_timer);
				uip_arp_timer();
      }*/
	
}

/*******************************************************************************
*   函数名：uIP_Net_Init
*   输  入:
*   输  出:
*   功能说明：初始化网络硬件、UIP协议栈、配置本机IP地址
************************************************************/
void uIP_Net_Init(void)
{
	uip_ipaddr_t ipaddr;
	uip_ipaddr_t addr;
	struct uip_udp_conn *conn = NULL;
	struct uip_udp_conn *conn1 = NULL;
	struct uip_udp_conn *conn2 = NULL;
	ULONG ulIp, ulSubNet, ulGatewayIp;
	UCHAR ucIp[4];

	WlanAdapter_MacAddr(g_pWlanAdapter, uip_ethaddr.addr, FALSE);
	uip_init();
	
	uip_ipaddr(ipaddr, 192,168,0,UIP_SET_IPADDR);	//设置本地设置IP地址
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 192,168,0,1); 	//设置网关IP地址(其实就是你路由器的IP地址)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,255,0);	//设置网络掩码
	uip_setnetmask(ipaddr);

	//uip_listen(HTONS(1200));

	uip_ipaddr(addr, 192,168,0,100);
	conn2 = uip_udp_new(&addr, HTONS(BROADCAST_PORT));
	
	if(conn2 != NULL) 
	{
		uip_udp_bind(conn2, HTONS(BROADCAST_PORT));
		Console_Print("uip_udp_new successfull !\r\n");
  }
	
	uip_ipaddr(addr, 192,168,0,100);
	conn1 = uip_udp_new(&addr, HTONS(LOCAL_PORT));
	
	if(conn1 != NULL) 
	{
		uip_udp_bind(conn1, HTONS(LOCAL_PORT));
		Console_Print("uip_udp_new successfull !\r\n");
  }
	
	uip_ipaddr(addr, 192,168,0,100);
	conn = uip_udp_new(&addr, HTONS(HOST_PORT));
	
	if(conn != NULL) 
	{
		uip_udp_bind(conn, HTONS(1001));
		Console_Print("uip_udp_new successfull !\r\n");
  }
}
