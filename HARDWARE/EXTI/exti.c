#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "adc.h"
#include "sys.h"

extern u32  tim;
extern u32  bei;

u16   buf[1000];
u16   buf1[1000];
u16   buf2[1000];
u16   buf3[1000];
u16   buf4[1000];
u16   buf5[1000];
u16   table[50]  ={0,25,50,75,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850};
u16		table1[160] ={0,5,10,15,20,30,35,40,45,55,60,65,70,80,85,
										90,95,105,110,115,120,130,135,140,145,155,
										160,165,170,180,185,190,195,205,210,215,220,
										230,235,240,245,250,255,260,265,270,275,280,285,290,295,300,305,
										310,315,320,325,330,335,340,345,350,355,360,365,370,375,
										380,385,390,395,400,405,410,415,420,425,430,435,440,445,450,455,460,465,470,475,
                    480,485,490,495,500,505,510,515,520,525,530,535,540,545,550,555,560,565,570,575,
                    580,585,590,595,600,605,610,615,620,625,630,635,640,645,650,655,660,665,670,675,
                    680,685,690,695,700,705,710,715,720,725};
u16 Ypos1=100,Ypos2=100,Xpos1,Xpos2;
u16 Ypos3=100,Ypos4=100,Xpos3,Xpos4;
u16 Ypos5=100,Ypos6=100,Xpos5,Xpos6;
u16 Ypos7=100,Ypos8=100,Xpos7,Xpos8;
u16 Ypos9=100,Ypos10=100,Xpos9,Xpos10;
u16 Ypos11=100,Ypos12=100,Xpos11,Xpos12;
u16 adcx,dsl;
float temp1;
u16  t=75;
float p;
u16  h=3;
u16 d=13;
u32  tim=1;
u32  bei=1;
u16 Yinit=75;
u8 a;
u8 len;										
int b,c;
void drawgaid() //画网格  
{
	u16 x,y;
	for(x=0;x<301;x=x+25)
		for(y=0;y<201;y=y+5)
		{
			LCD_Fast_DrawPoint(x,y,BLUE );
		}
	for(y=0;y<201;y=y+25)
		for(x=0;x<301;x=x+5)
		{
			LCD_Fast_DrawPoint(x,y,BLUE );
		}
}


void clear_point(u16 hang)//更新显示屏当前列
{
	float index_clear_lie = 50; 
	POINT_COLOR = BLACK ;
	for(index_clear_lie = 50;index_clear_lie <426;index_clear_lie=index_clear_lie+1)
	{		
		LCD_DrawPoint(hang,index_clear_lie );
		LCD_DrawPoint(hang+1,index_clear_lie );
		LCD_DrawPoint(hang+2,index_clear_lie );
		LCD_DrawPoint(hang+3,index_clear_lie );
		LCD_DrawPoint(hang+4,index_clear_lie );
	}
	if(hang==table[h])//判断hang是否为25的倍数
	{
			for(index_clear_lie = 50;index_clear_lie <426;index_clear_lie=index_clear_lie+5)
			{		
				LCD_Fast_DrawPoint(hang ,index_clear_lie,	GREEN );
			}
			h++;
			if(h>29) h=3;
	}
	if(hang ==table1[d])//判断hang是否为5的倍数
	{
			for(index_clear_lie = 50;index_clear_lie <426;index_clear_lie=index_clear_lie+25)//行数
		{		
				LCD_Fast_DrawPoint(hang ,index_clear_lie,YELLOW );
			}
			d++;
			if(d>135) d=13;//行长度
	}
	
	POINT_COLOR=RED;	
}
void DrawOscillogramSingle()//画波形图
{
	/*
		for(t=15;t<146;t++)//存储AD数值
		{
			buf[t] =Get_Adc(ADC_Channel_10);
			//buf1[t] =Get_Adc(ADC_Channel_11);
			//buf2[t] =Get_Adc(ADC_Channel_12);
			//buf3[t] =Get_Adc(ADC_Channel_13);
			//buf4[t] =Get_Adc(ADC_Channel_14);
			//buf5[t] =Get_Adc(ADC_Channel_15);
			if(tim>1)
				delay_us(tim );//改变AD取样间隔
		}
			for(t=15;t<146;t++)
		{
			p=t*5;
			clear_point(p );	
			
			Ypos2=Yinit+buf[t]*100/4096;//转换坐标
			Ypos2=Ypos2*bei;
			if(Ypos2 >450) Ypos2 =450; //超出范围不显示
			POINT_COLOR=RED; 
			LCD_DrawLine (5*t ,2*Ypos1+75 ,5*(t+1),2*Ypos2+75  );
			Ypos1 =Ypos2 ;
			
			//Ypos4=Yinit+buf1[t]*100/4096;//转换坐标
			//Ypos4=Ypos4*bei;
			//if(Ypos4 >450) Ypos4 =450; //超出范围不显示
			//POINT_COLOR=BLUE; 
			//LCD_DrawLine (5*t ,2*Ypos3+75 ,5*(t+1),2*Ypos4+75  );
			//Ypos3 =Ypos4 ;
			
			//Ypos6=Yinit+buf2[t]*100/4096;//转换坐标
			//Ypos6=Ypos6*bei;
			//if(Ypos6 >450) Ypos6 =450; //超出范围不显示
			//POINT_COLOR=YELLOW; 
			//LCD_DrawLine (5*t ,2*Ypos5+75 ,5*(t+1),2*Ypos6+75  );
			//Ypos5 =Ypos6 ;
			
			//Ypos8=Yinit+buf3[t]*100/4096;//转换坐标
			//Ypos8=Ypos8*bei;
			//if(Ypos8 >450) Ypos8 =450; //超出范围不显示
			//POINT_COLOR=GREEN; 
			//LCD_DrawLine (5*t ,2*Ypos7+75 ,5*(t+1),2*Ypos8+75  );
			//Ypos7 =Ypos8 ;
			
			//Ypos10=Yinit+buf4[t]*100/4096;//转换坐标
			//Ypos10=Ypos10*bei;
			//if(Ypos10 >450) Ypos10 =450; //超出范围不显示
			//POINT_COLOR=BROWN; 
			//LCD_DrawLine (5*t ,2*Ypos9+75 ,5*(t+1),2*Ypos10+75  );
			//Ypos9 =Ypos10 ;
			
			//Ypos12=Yinit+buf5[t]*100/4096;//转换坐标
			//Ypos12=Ypos12*bei;
			//if(Ypos12 >450) Ypos12 =450; //超出范围不显示
			//POINT_COLOR=WHITE; 
			//LCD_DrawLine (5*t ,2*Ypos11+75 ,5*(t+1),2*Ypos12+75  );
			//Ypos11 =Ypos12 ;
		}	
		*/
		Ypos1=100;
		Ypos2=100;
	//LCD_ShowxNum(725,2*Ypos1+75,999,4,24,0);	//显示峰峰值mv	 		
		for(t=10;t<950;t++)//存储AD数值
		{
			buf[t] =Get_Adc(ADC_Channel_10);
			if(tim>1)
				delay_us(tim );//改变AD取样间隔
		}
		
		//LCD_ShowxNum(0,0,buf[15],4,24,0);	//显示峰峰值mv	 		
		//LCD_ShowxNum(150,0,buf[16],4,24,0);	//显示峰峰值mv	 		
		//LCD_ShowxNum(300,0,buf[17],4,24,0);	//显示峰峰值mv	 		
		
		for(b =50; b < 800; b++) {
			if((buf[b] >= 3500) && (buf[b] > buf[b+1])) {
				break;
			}
		}
		t = 15;
		for(c = 0; c < 150; c++) {
			buf[t+c] = buf[b+c];
		}
		
		//LCD_ShowxNum(0,420,buf[15],4,24,0);	//显示峰峰值mv	 		
		//LCD_ShowxNum(150,420,buf[16],4,24,0);	//显示峰峰值mv	 		
		//LCD_ShowxNum(300,420,buf[17],4,24,0);	//显示峰峰值mv	 	
			for(t=15;t<146;t++)
		{
			p=t*5;
			clear_point(p );	
			Ypos2=Yinit+buf[t]*100/4096;//转换坐标
			Ypos2=Ypos2*bei;
			if(Ypos2 >450) Ypos2 =450; //超出范围不显示
			if(t!=15) {
				LCD_DrawLine (5*(t-1) ,2*Ypos1+75 ,5*(t),2*Ypos2+75  );
			
			}
			Ypos1 =Ypos2 ;
		}	
}

void DrawOscillogramDouble()//画波形图
{
	/*
		for(t=15;t<146;t++)//存储AD数值
		{
			buf[t] =Get_Adc(ADC_Channel_10);
			buf1[t] =Get_Adc(ADC_Channel_11);
			//buf2[t] =Get_Adc(ADC_Channel_12);
			//buf3[t] =Get_Adc(ADC_Channel_13);
			//buf4[t] =Get_Adc(ADC_Channel_14);
			//buf5[t] =Get_Adc(ADC_Channel_15);
			if(tim>1)
				delay_us(tim );//改变AD取样间隔
		}
			for(t=15;t<146;t++)
		{
			p=t*5;
			clear_point(p );	
			
			Ypos2=Yinit+buf[t]*100/4096;//转换坐标
			Ypos2=Ypos2*bei;
			if(Ypos2 >450) Ypos2 =450; //超出范围不显示
			POINT_COLOR=RED; 
			LCD_DrawLine (5*t ,2*Ypos1+75 ,5*(t+1),2*Ypos2+75  );
			Ypos1 =Ypos2 ;
			
			Ypos4=Yinit+buf1[t]*100/4096;//转换坐标
			Ypos4=Ypos4*bei;
			if(Ypos4 >450) Ypos4 =450; //超出范围不显示
			POINT_COLOR=BLUE; 
			LCD_DrawLine (5*t ,2*Ypos3+75 ,5*(t+1),2*Ypos4+75  );
			Ypos3 =Ypos4 ;
			
			//Ypos6=Yinit+buf2[t]*100/4096;//转换坐标
			//Ypos6=Ypos6*bei;
			//if(Ypos6 >450) Ypos6 =450; //超出范围不显示
			//POINT_COLOR=YELLOW; 
			//LCD_DrawLine (5*t ,2*Ypos5+75 ,5*(t+1),2*Ypos6+75  );
			//Ypos5 =Ypos6 ;
			
			//Ypos8=Yinit+buf3[t]*100/4096;//转换坐标
			//Ypos8=Ypos8*bei;
			//if(Ypos8 >450) Ypos8 =450; //超出范围不显示
			//POINT_COLOR=GREEN; 
			//LCD_DrawLine (5*t ,2*Ypos7+75 ,5*(t+1),2*Ypos8+75  );
			//Ypos7 =Ypos8 ;
			
			//Ypos10=Yinit+buf4[t]*100/4096;//转换坐标
			//Ypos10=Ypos10*bei;
			//if(Ypos10 >450) Ypos10 =450; //超出范围不显示
			//POINT_COLOR=BROWN; 
			//LCD_DrawLine (5*t ,2*Ypos9+75 ,5*(t+1),2*Ypos10+75  );
			//Ypos9 =Ypos10 ;
			
			//Ypos12=Yinit+buf5[t]*100/4096;//转换坐标
			//Ypos12=Ypos12*bei;
			//if(Ypos12 >450) Ypos12 =450; //超出范围不显示
			//POINT_COLOR=WHITE; 
			//LCD_DrawLine (5*t ,2*Ypos11+75 ,5*(t+1),2*Ypos12+75  );
			//Ypos11 =Ypos12 ;
		}	
		*/
		Ypos1=100;
		Ypos2=100;
	//LCD_ShowxNum(725,2*Ypos1+75,999,4,24,0);	//显示峰峰值mv	 		
		for(t=10;t<950;t++)//存储AD数值
		{
			buf[t] =Get_Adc(ADC_Channel_10);
			buf1[t] =Get_Adc(ADC_Channel_11);
			if(tim>1)
				delay_us(tim );//改变AD取样间隔
		}
		
		//LCD_ShowxNum(0,0,buf[15],4,24,0);	//显示峰峰值mv	 		
		//LCD_ShowxNum(150,0,buf[16],4,24,0);	//显示峰峰值mv	 		
		//LCD_ShowxNum(300,0,buf[17],4,24,0);	//显示峰峰值mv	 		
		
		for(b =50; b < 800; b++) {
			if((buf[b] >= 3500) && (buf[b] > buf[b+1])) {
				break;
			}
		}
		t = 15;
		for(c = 0; c < 150; c++) {
			buf[t+c] = buf[b+c];
		}
		
		for(b =50; b < 800; b++) {
			if((buf1[b] >= 3500) && (buf1[b] > buf1[b+1])) {
				break;
			}
		}
		t = 15;
		for(c = 0; c < 150; c++) {
			buf1[t+c] = buf1[b+c];
		}
		
		//LCD_ShowxNum(0,420,buf[15],4,24,0);	//显示峰峰值mv	 		
		//LCD_ShowxNum(150,420,buf[16],4,24,0);	//显示峰峰值mv	 		
		//LCD_ShowxNum(300,420,buf[17],4,24,0);	//显示峰峰值mv	 	
			for(t=15;t<146;t++)
		{
			p=t*5;
			clear_point(p );	
			Ypos2=Yinit+buf[t]*100/4096;//转换坐标
			Ypos2=Ypos2*bei;
			Ypos4=Yinit+buf1[t]*100/4096;//转换坐标
			if(Ypos2 >450) Ypos2 =450; //超出范围不显示
			if(Ypos4 >450) Ypos4 =450; //超出范围不显示
			if(t!=15) {
				POINT_COLOR=RED; 
				LCD_DrawLine (5*(t-1) ,2*Ypos1+75 ,5*(t),2*Ypos2+75  );
				POINT_COLOR=BLUE; 
				LCD_DrawLine (5*(t-1) ,2*Ypos3+75 ,5*(t),2*Ypos4+75  );
			
			}
			Ypos1 =Ypos2 ;
			Ypos3 =Ypos4 ;
		}	
}
 
//外部中断初始化函数
void EXTIX_Init(void)
{
 
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

		KEY_Init();//初始化按键对应io模式
  	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

	  

    //GPIOC.5 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级1
		
  	
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}


void EXTI4_IRQHandler(void)
{
  delay_ms(10);    //消抖			
  if(KEY0==0)	
	{
		while(1) {
		  DrawOscillogramDouble();
		}
		
	}
	 EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE15线路挂起位
}
