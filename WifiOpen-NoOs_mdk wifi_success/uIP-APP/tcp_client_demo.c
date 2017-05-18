#include "Project.h"
#include "tcp_demo.h"
//#include "sys.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>	   
#include "Led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK战舰STM32开发板
//uIP TCP Client测试 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/28
//版本：V1.0			   								  
//////////////////////////////////////////////////////////////////////////////////
extern u8 mode;
extern volatile u8 num_save;
extern volatile u8 num_send;
extern u8 tcp_client_databuf_send[3][305];   	//发送数据缓存
extern u8 tcp_client_databuf_recv[20];   	//发送数据缓存
extern unsigned int data_per_pack;
extern u8 mode_changed;
extern unsigned char IT_send;
extern unsigned long ts_now;
extern volatile unsigned int time_per_data;
u8 tcp_client_sta;				//客户端状态
uip_ipaddr_t addr,addr1;
//[7]:0,无连接;1,已经连接;
//[6]:0,无数据;1,收到客户端数据
//[5]:0,无数据;1,有数据需要发送

//这是一个TCP 客户端应用回调函数。
//该函数通过UIP_APPCALL(tcp_demo_appcall)调用,实现Web Client的功能.
//当uip事件发生时，UIP_APPCALL函数会被调用,根据所属端口(1400),确定是否执行该函数。
//例如 : 当一个TCP连接被创建时、有新的数据到达、数据已经被应答、数据需要重发等事件
void udp_client_demo_appcall(void)
{		  
 	//接收到一个新的TCP数据包 
	if (uip_newdata())
	{
			/*if(uip_len>199)
			{		   
				((u8*)uip_appdata)[199]=0;
			}*/
			mode_changed = ((u8*)uip_appdata)[0];
			tcp_client_databuf_recv[1] = ((u8*)uip_appdata)[1];
			tcp_client_databuf_recv[2] = ((u8*)uip_appdata)[2];
			switch (mode_changed) 
			{
					case 'R':
					{
						time_per_data = (tcp_client_databuf_recv[1]<<8) + tcp_client_databuf_recv[2];
						mode = mode_changed;
						break;
					}
				  case 'P':
					{
						time_per_data = 25;
						mode = mode_changed;
						break;
					}
					default: break;
			}
	} 
	//当需要重发、新数据到达、数据包送达、连接建立时，通知uip发送数据 
	if(uip_poll())
	{
		//uip_ipaddr(addr, 192,168,0,100);
		//if((uip_ipaddr_cmp(uip_udp_conn->ripaddr, addr))&&(uip_udp_conn->rport == HTONS(HOST_PORT)))
		//{
			if(mode == 'L')
			{
				uip_send(tcp_client_databuf_send[0],data_per_pack*9+15);
				mode = 'R';
			}
			else uip_send(tcp_client_databuf_send[num_save],data_per_pack*3+5);
			
			num_save ++;
			if(num_save >= 3) num_save = 0;
		//}
	}		
}
//这里我们假定Server端的IP地址为:192.168.1.103
//这个IP必须根据Server端的IP修改.
//尝试重新连接
void tcp_client_reconnect()
{
	uip_ipaddr_t ipaddr;
	uip_ipaddr(&ipaddr,192,168,0,100);	//设置IP为192.168.1.103
	uip_connect(&ipaddr,htons(1400)); 	//端口为1400
}
//终止连接				    
void tcp_client_aborted(void)
{
	tcp_client_sta&=~(1<<7);	//标志没有连接
	tcp_client_reconnect();		//尝试重新连接
	uip_log("tcp_client aborted!\r\n");//打印log
}
//连接超时
void tcp_client_timedout(void)
{
	tcp_client_sta&=~(1<<7);	//标志没有连接	   
	uip_log("tcp_client timeout!\r\n");//打印log
}
//连接关闭
void tcp_client_closed(void)
{
	tcp_client_sta&=~(1<<7);	//标志没有连接
	tcp_client_reconnect();		//尝试重新连接
	uip_log("tcp_client closed!\r\n");//打印log
}	 
//连接建立
void tcp_client_connected(void)
{ 
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	tcp_client_sta|=1<<7;		//标志连接成功
}
//发送的数据成功送达
void tcp_client_acked(void)
{											    
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	s->textlen=0;//发送清零
	uip_log("tcp_client acked!\r\n");//表示成功发送		 
}
//发送数据给服务端
void tcp_client_senddata(void)
{
	struct	tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	
	//s->textptr:发送的数据包缓冲区指针
	//s->textlen:数据包的大小（单位字节）		   
	if(s->textlen>0)uip_send(s->textptr, s->textlen);//发送TCP数据包	 
}


void tcp_client_send(void)
{ 
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	//tcp_client_sta|=1<<7;		//标志连接成功
	tcp_client_sta|=1<<5;//标记有数据需要发送
  //	uip_log("tcp_client connected!\r\n");//打印log
	s->state=STATE_CMD; 		//指令状态
	//s->textlen=0;
	//s->textlen=strlen((char *)s->textptr);
	//tcp_client_databuf[0]='S';
	//tcp_client_databuf[1]=ad_data[3];
	//if(tcp_client_databuf[1]==0) tcp_client_databuf[1] = 0x11;
	//tcp_client_databuf[2]=ad_data[2];
	//if(tcp_client_databuf[2]==0) tcp_client_databuf[2] = 0x11;
	//tcp_client_databuf[3]=ad_data[1];
	//if(tcp_client_databuf[3]==0) tcp_client_databuf[3] = 0x11;
	//tcp_client_databuf[4]='E';
	//sprintf(prtbuf,"%d\r\n",uip_timer/100);
	//s->textlen=strlen(prtbuf);
	//strcpy((char*)tcp_client_databuf_send,prtbuf ); //"wifi success test+\r\n"
  //strcpy((char*)tcp_client_databuf, "wifi success test+\r\n" ); //"wifi success test+\r\n"
	
}

















