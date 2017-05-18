#include "AD.h"
#include "led.h"
#include "Project.h"
//#include "spi.h"

extern unsigned long buf_flag ;
extern unsigned char IT_send;
extern unsigned int data_per_pack;
extern u8 tcp_client_databuf_recv[20];   	//发送数据缓存
extern u8 tcp_client_databuf_send[3][305];
extern u8 mode;
extern volatile u8 num_save;
extern volatile u8 num_send;
extern u8 num_sum;
extern u8 ts_set,ts_now;
extern u32 uip_timer;
u8 mode_changed;
u32 sendcount = 0;
unsigned char ad_init[10];
unsigned char ad_init1[10];
unsigned char ad_data[10];
unsigned char ad_data2[10];

unsigned char count[3];

void AD7176_init(void)
{
	    //init AD71761
	    GPIO_ResetBits (GPIOE, ADCS2) ;
	    delay(10);
 	    ad_init[0]=SPI2_ReadWriteByte(0x01);
      ad_init[1]=SPI2_ReadWriteByte(0x20);
  	  ad_init[2]=SPI2_ReadWriteByte(0x08);//?????????

		ad_init[3]=SPI2_ReadWriteByte(0x02);
		ad_init[4]=SPI2_ReadWriteByte(0x08);
		ad_init[5]=SPI2_ReadWriteByte(0x00);  //
	  
	  ad_init[0]=SPI2_ReadWriteByte(0x06);
    ad_init[1]=SPI2_ReadWriteByte(0x00);
  	ad_init[2]=SPI2_ReadWriteByte(0x00);
		
		// ??ADC(?????????????)
		
		ad_init1[0]=SPI2_ReadWriteByte(0x20);
		ad_init1[1]=SPI2_ReadWriteByte(0x10);
		ad_init1[2]=SPI2_ReadWriteByte(0x00);//???????,?????
		
		ad_init1[3]=SPI2_ReadWriteByte(0x38);
		ad_init1[4]=SPI2_ReadWriteByte(0x20);
		ad_init1[5]=SPI2_ReadWriteByte(0x00);
		ad_init1[6]=SPI2_ReadWriteByte(0x00);
	
	  ad_init1[0]=SPI2_ReadWriteByte(0x10);
		ad_init1[1]=SPI2_ReadWriteByte(0x80);
		ad_init1[2]=SPI2_ReadWriteByte(0x43);
		
	  ad_init1[3]=SPI2_ReadWriteByte(0x11);
		ad_init1[4]=SPI2_ReadWriteByte(0x00);
		ad_init1[5]=SPI2_ReadWriteByte(0x00);
		
		ad_init1[6]=SPI2_ReadWriteByte(0x12);
		ad_init1[7]=SPI2_ReadWriteByte(0x00);
		ad_init1[8]=SPI2_ReadWriteByte(0x00);
		
		ad_init[7]=SPI2_ReadWriteByte(0x13);
		ad_init[8]=SPI2_ReadWriteByte(0x00);
		ad_init[9]=SPI2_ReadWriteByte(0x00);
		
		ad_init[6]=SPI2_ReadWriteByte(0x28);
    ad_init[7]=SPI2_ReadWriteByte(0x00);
  	ad_init[8]=SPI2_ReadWriteByte(0x08);
		
}

void AD7176_fast(void)
{
	  char prtbuf[10];
		
	  GPIO_ResetBits (GPIOE, ADCS2) ;
	  ad_data[0]=SPI2_ReadWriteByte(0x44);
		
   	tcp_client_databuf_send[num_save][buf_flag+2] = SPI2_ReadWriteByte(0x00); 
	  tcp_client_databuf_send[num_save][buf_flag+1] = SPI2_ReadWriteByte(0x00); 
	  tcp_client_databuf_send[num_save][buf_flag] = SPI2_ReadWriteByte(0x00);	
	 
		GPIO_SetBits (GPIOE, ADCS2) ;
	 	
	  buf_flag = buf_flag + 3;
    
	  if(buf_flag > data_per_pack * 3 + 3)
		{	
			if(++count[0] == 0) 
			{
				if(++count[1] == 0) 
				{
					++count[2];
				}
			}	
			
			tcp_client_databuf_send[num_save][1] = count[0];
			tcp_client_databuf_send[num_save][2] = count[1]; 
		  tcp_client_databuf_send[num_save][3] = count[2];
			
			tcp_client_databuf_send[num_save][0] = mode;
		  tcp_client_databuf_send[num_save][buf_flag] = 'E';		
			
			buf_flag = 4 ;
			
			if(mode_changed == 'L')
			{
				mode = mode_changed;
				tcp_client_databuf_send[0][0] = 'L';
				tcp_client_databuf_send[1][0] = 'L';
				tcp_client_databuf_send[2][0] = 'L';
				tcp_client_databuf_send[num_save][0] = 'O';
				mode_changed = 0;
			}
			IT_send = 1;	
			//TIM_ITConfig( TIM6,TIM_IT_Update|TIM_IT_Trigger,DISABLE);//使能定时器6更新触发中断
		}
}

void max9939_init(void)
{
	/*GPIO_ResetBits (GPIOB, OPCS1) ;
	ad_init[0]=SPI2_ReadWriteByte(0x00);
	ad_init[0]=SPI2_ReadWriteByte(0x01);
	delay(2);*/
	GPIO_SetBits (GPIOB, OPCS1) ;	
	GPIO_SetBits (GPIOB, OPCS2) ;	
}

void delay(unsigned char t)
{
unsigned int jj=0;
	for(;t>1;t--)
	for(jj=500*35;jj>1;jj--);
}
