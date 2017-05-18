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
						//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
						//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
	 if(uip_len>0)
	 {
			uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
			tapdev_send();//�������ݵ���̫��
	 }
}
/*******************************************************************************
*   ��������UipPro
*   ��  ��:
*   ��  ��:
*   ����˵�����жϴ�����ȡ������ջ���
*   ע�⣺��ʹ��uCOSʱ�����ʺ����жϡ�����ؽ��ǡ�
********************************************************************/
void UipPro(void)
{
	u8 i;
	//u8 tcp_server_tsta=0xff;
	//u8 tcp_client_tsta=0xff;
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok=0;	 
		
	if(timer_ok==0)//����ʼ��һ��
	{
		timer_ok = 1;
	  //timer_set(&periodic_timer,CLOCK_SECOND/100);  //����1��0.01��Ķ�ʱ�� 
	  //timer_set(&arp_timer,CLOCK_SECOND*10);	   	//����1��10��Ķ�ʱ�� 
	}	
	if(WIFI_GET_FLAG(g_pWlanAdapter, FLAG_PKTRCVED)) 
	{
		WIFI_CLR_FLAG(g_pWlanAdapter, FLAG_PKTRCVED);  //�����ʶλ
	  uip_len = tapdev_read();    //�������豸��ȡһ��IP��,�������ݳ���
		if(uip_len > 0) //�յ�����ʱ���Ȼ�仯
		{          
			/* ����IP���ݰ�(ֻ��У��ͨ����IP���Żᱻ����) */
		 	if(BUF->type == htons(UIP_ETHTYPE_IP)) //�յ�IP�� //IP��У��
			{ 
				uip_arp_ipin();        //ȥ����̫��ͷ�ṹ������ARP��
				uip_input();           //IP������
				/*
				    ��Ϊuip_input();���������˻ص�����UIP_APPCALL(),��tcp_server_appcall
				    ������ĺ���ִ�к�����ص�������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
				    ��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)
				*/
				if(uip_len > 0)  //�д����Ӧ����
				{      
					uip_arp_out();      //����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
					tapdev_send();      //�������ݵ���̫�����豸��������
				}
			}
			/* ����arp���� */
			else if(BUF->type == htons(UIP_ETHTYPE_ARP)) 
			{ //��ARP�����
				uip_arp_arpin();        //������ARP��Ӧ������ARP����������󣬹����Ӧ���ݰ�
				/*
				    ������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
				    ��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)
				*/
				if(uip_len > 0)         //��ARP����Ҫ���ͻ�Ӧ
				{      
					tapdev_send();      //��ARP��Ӧ����̫����
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
*   ��������uIP_Net_Init
*   ��  ��:
*   ��  ��:
*   ����˵������ʼ������Ӳ����UIPЭ��ջ�����ñ���IP��ַ
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
	
	uip_ipaddr(ipaddr, 192,168,0,UIP_SET_IPADDR);	//���ñ�������IP��ַ
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 192,168,0,1); 	//��������IP��ַ(��ʵ������·������IP��ַ)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,255,0);	//������������
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
