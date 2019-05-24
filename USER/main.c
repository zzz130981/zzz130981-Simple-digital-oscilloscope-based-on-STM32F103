#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "exti.h"
#include "stm32f10x_adc.h"

 int main(void)
 {	 
  u16 adcx;
	float temp;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();			 	
 	Adc_Init();		  		//ADC初始化
	 //
	  KEY_Init ();
	 	 POINT_COLOR=RED; 
	 LCD_Display_Dir(1);
	 EXTIX_Init();
	 //LCD_ShowString(0,210,200,24,24,"Vpp=0000mv");
	 //LCD_ShowxNum(284,220,tim ,3,16,0);

//	POINT_COLOR=RED;//设置字体为红色 
	//LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	//LCD_ShowString(60,70,200,16,16,"ADC TEST");	
	//LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	//LCD_ShowString(60,110,200,16,16,"2015/1/14");	
	//显示提示信息
	//POINT_COLOR=BLUE;//设置字体为蓝色
	//LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	//LCD_ShowString(60,150,200,16,16,"ADC_CH0_VOL:0.000V");	   
 //LED0=0;	
// LED1=0;
	LCD_ShowString(40,218,200,16,16,"3.3V");		
		LCD_ShowString(40,415,200,16,16,"0.0V");	
	while(1)
	{
		
	
		adcx=Get_Adc_Average(ADC_Channel_1,10);
		//LCD_ShowxNum(156,130,adcx,4,16,0);//显示ADC的值
		temp=(float)adcx*(3.3/4096);
		adcx=temp;
		//LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
		temp-=adcx;
		temp*=1000;
		//LCD_ShowxNum(172,150,temp,3,16,0X80);
		DrawOscillogramSingle();//画波形
			 //LCD_ShowxNum(320,450,temp1,4,24,0);	//显示峰峰值mv	 		
		//LED0=!LED0;//判断程序是否运行	
	}
 }

