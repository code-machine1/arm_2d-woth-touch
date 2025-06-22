#include "touch.h"

XPT2046_TOUCH xpt2046_touch;
 
/*
��������:  ��ʼ��
Ӳ������:
T_MOSI--PF9
T_MISO--PB2
T_SCK---PB1
T_PEN---PF10
T_CS----PF11
*/
void XPT2046_TouchInit(void)
{
//    /*1. ʱ�ӳ�ʼ��*/
//    RCC->APB2ENR|=1<<3; //PB
//    RCC->APB2ENR|=1<<7; //PF
//    
//   /*2. ��ʼ��GPIO��*/
//   GPIOB->CRL&=0xFFFFF00F;
//   GPIOB->CRL|=0x00000830;
//  
//   GPIOF->CRH&=0xFFFF000F;
//   GPIOF->CRH|=0x00003830;
//  
//   /*3. ����*/
//   GPIOB->ODR|=0x3<<1;
//   GPIOF->ODR|=0x7<<9;

}
 
 
/*
��������:  SPI�ײ�дһ���ֽ�
*/
void XPT2046_SPI_WriteOneByte(u8 cmd)
{
    u8 i;
    for(i=0;i<8;i++)
    {
        
        if(cmd&0x80)XPT2046_MOSI_Set;
        else XPT2046_MOSI_Clr;
        cmd<<=1;
		
		XPT2046_SCK_Clr; //�͵�ƽд
        XPT2046_SCK_Set; //�ߵ�ƽ��,��֤�������ȶ�
    }
}
 
 
/*
��������:  ��2���ֽ�
˵��: ��ȡ16λ���ݣ����4λ������Ч����Ч�����Ǹ�12λ
*/
u16 XPT2046_ReadData(u8 cmd)
{
    u16 data;
    u8 i;
    XPT2046_CS_Clr;  //ѡ��XPT2046
    XPT2046_MOSI_Clr;
    XPT2046_SCK_Clr;
    XPT2046_SPI_WriteOneByte(cmd);
    
    delay_us(6); //0.008ms ,�ȴ�XPT2046ת����ɡ�
    //����æ�ź�
    XPT2046_SCK_Clr;
    delay_us(1);
    XPT2046_SCK_Set;
    XPT2046_SCK_Clr;
    //������ȡ16λ������
    for(i=0;i<16;i++)
    {
        
        data<<=1;
		XPT2046_SCK_Clr;
		XPT2046_SCK_Set;
        if(XPT2046_MISO)
			data++;
    }
    data>>=4; //�������4λ
    XPT2046_SCK_Set;
    XPT2046_CS_Set; //ȡ��ѡ��
    return (data);
}
 
/*
XPT2046������:
10010000  :����Y������ 0x90
11010000  :����X������ 0xD0
����ֵ:  0��ʾû�ж�ȡ������,1��ʾ��ȡ����ǰ����
//1. �õ����ϽǺ����½ǵ�����XY����ֵ
  x=3831,y=3934
  x=155,y=168
//2. ת������ֵ
  x����:3831~155 -->  3676~0
  y����:3934~168 -->  3766~0
//3. ����б��
  x�����б��: 3676/320=11.4875
  y�����б��: 3766/480=7.84583
//4. �õ�ʵ�ʵ���������
  x����:  320-(ģ����-155)/11.4875
  y����:  480-(ģ����-168)/7.84583
*/
u8 XPT2046_ReadXY(void)
{
    if(XPT2046_PEN==0) //�жϴ������Ƿ���
    {
        /*1. �õ���������*/
        xpt2046_touch.x0=XPT2046_ReadData(0xD0);
        xpt2046_touch.y0=XPT2046_ReadData(0x90);
      
        /*2. �õ���������*/
        xpt2046_touch.x=240-(xpt2046_touch.x0-155)/15.316;
		if(xpt2046_touch.x >= 240)
			xpt2046_touch.x = 240;
		
		if(xpt2046_touch.x <= 0)
			xpt2046_touch.x = 0;
		
        xpt2046_touch.y=320-(xpt2046_touch.y0-168)/11.768;
		 if( xpt2046_touch.y >= 320)
			  xpt2046_touch.y = 320;
		 if( xpt2046_touch.y <= 0)
			  xpt2046_touch.y = 0;
		 
        return 1;
    }
    return 0;
}