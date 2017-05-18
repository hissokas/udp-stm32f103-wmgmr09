#include "Project.h"
#include "tcp_demo.h"
//#include "sys.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>	   
#include "Led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEKս��STM32������
//uIP TCP Client���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/28
//�汾��V1.0			   								  
//////////////////////////////////////////////////////////////////////////////////
extern u8 mode;
extern volatile u8 num_save;
extern volatile u8 num_send;
extern u8 tcp_client_databuf_send[3][305];   	//�������ݻ���
extern u8 tcp_client_databuf_recv[20];   	//�������ݻ���
extern unsigned int data_per_pack;
extern u8 mode_changed;
extern unsigned char IT_send;
extern unsigned long ts_now;
extern volatile unsigned int time_per_data;
u8 tcp_client_sta;				//�ͻ���״̬
uip_ipaddr_t addr,addr1;
//[7]:0,������;1,�Ѿ�����;
//[6]:0,������;1,�յ��ͻ�������
//[5]:0,������;1,��������Ҫ����

//����һ��TCP �ͻ���Ӧ�ûص�������
//�ú���ͨ��UIP_APPCALL(tcp_demo_appcall)����,ʵ��Web Client�Ĺ���.
//��uip�¼�����ʱ��UIP_APPCALL�����ᱻ����,���������˿�(1400),ȷ���Ƿ�ִ�иú�����
//���� : ��һ��TCP���ӱ�����ʱ�����µ����ݵ�������Ѿ���Ӧ��������Ҫ�ط����¼�
void udp_client_demo_appcall(void)
{		  
 	//���յ�һ���µ�TCP���ݰ� 
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
	//����Ҫ�ط��������ݵ�����ݰ��ʹ���ӽ���ʱ��֪ͨuip�������� 
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
//�������Ǽٶ�Server�˵�IP��ַΪ:192.168.1.103
//���IP�������Server�˵�IP�޸�.
//������������
void tcp_client_reconnect()
{
	uip_ipaddr_t ipaddr;
	uip_ipaddr(&ipaddr,192,168,0,100);	//����IPΪ192.168.1.103
	uip_connect(&ipaddr,htons(1400)); 	//�˿�Ϊ1400
}
//��ֹ����				    
void tcp_client_aborted(void)
{
	tcp_client_sta&=~(1<<7);	//��־û������
	tcp_client_reconnect();		//������������
	uip_log("tcp_client aborted!\r\n");//��ӡlog
}
//���ӳ�ʱ
void tcp_client_timedout(void)
{
	tcp_client_sta&=~(1<<7);	//��־û������	   
	uip_log("tcp_client timeout!\r\n");//��ӡlog
}
//���ӹر�
void tcp_client_closed(void)
{
	tcp_client_sta&=~(1<<7);	//��־û������
	tcp_client_reconnect();		//������������
	uip_log("tcp_client closed!\r\n");//��ӡlog
}	 
//���ӽ���
void tcp_client_connected(void)
{ 
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	tcp_client_sta|=1<<7;		//��־���ӳɹ�
}
//���͵����ݳɹ��ʹ�
void tcp_client_acked(void)
{											    
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	s->textlen=0;//��������
	uip_log("tcp_client acked!\r\n");//��ʾ�ɹ�����		 
}
//�������ݸ������
void tcp_client_senddata(void)
{
	struct	tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	
	//s->textptr:���͵����ݰ�������ָ��
	//s->textlen:���ݰ��Ĵ�С����λ�ֽڣ�		   
	if(s->textlen>0)uip_send(s->textptr, s->textlen);//����TCP���ݰ�	 
}


void tcp_client_send(void)
{ 
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	//tcp_client_sta|=1<<7;		//��־���ӳɹ�
	tcp_client_sta|=1<<5;//�����������Ҫ����
  //	uip_log("tcp_client connected!\r\n");//��ӡlog
	s->state=STATE_CMD; 		//ָ��״̬
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

















