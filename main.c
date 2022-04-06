#include <STC15F2K60S2.h>
#include <iic.h>

#define uchar unsigned char 
#define uint unsigned int
	
uchar code dpnum[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar code dpdotnum[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x3f,0x7f};
uchar RDdat;
uint Vout;

void System_Init();
void delayms(uchar ms);
void display_bit(uchar pos,uchar dat);
void display();
uchar PCF8591_Read(uchar add);
void PCF8591_Write(uchar add,uchar dat);

void main()
{
	System_Init();
	while(1)
	{
		RDdat=PCF8591_Read(0x03);
		Vout=RDdat*500.0/255;
		PCF8591_Write(0x40,RDdat);
		display();
	}
}

void System_Init()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
}
void delayms(uchar ms)
{
	uint i,j;
	for(i=ms;i>0;i--)
		for(j=810;j>0;j--);
}
void display_bit(uchar pos,uchar dat)
{
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x01 << pos;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = dat;
}
void display()
{
	display_bit(0,dpnum[11]);
	delayms(1);
	display_bit(1,dpnum[11]);
	delayms(1);
	display_bit(2,dpnum[11]);
	delayms(1);
	display_bit(3,dpnum[11]);
	delayms(1);
	display_bit(4,dpnum[11]);
	delayms(1);
	display_bit(5,dpdotnum[Vout/100]);
	delayms(1);
	display_bit(6,dpnum[Vout/10%10]);
	delayms(1);
	display_bit(7,dpnum[Vout%10]);
	delayms(1);
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
}
uchar PCF8591_Read(uchar add)
{
	uchar temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	
	temp=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	
	return temp;
}
void PCF8591_Write(uchar add,uchar dat)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	
	IIC_SendByte(add);
	IIC_WaitAck();
	
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}