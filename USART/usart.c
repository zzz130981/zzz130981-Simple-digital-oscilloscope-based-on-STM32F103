#include "usart.h"
#include "rt_misc.h"
#pragma import(__use_no_semihosting_swi)
extern int GetKey(void);
struct __FILE {
int handle; 
};

FILE __stdout;
FILE __stdin;

int SendChar (int ch) 
{
	while((USART1->SR&USART_SR_TXE)==0);
	USART1->DR = (ch & 0x1FF);
	return (ch);
}

int fputc(int ch, FILE *f) 
{
	return (SendChar(ch));
}



void _ttywrch(int ch) 
{
	SendChar (ch);
}

int ferror(FILE *f) 
{ // Your implementation of ferror
	return EOF;
}

void _sys_exit(int return_code) 
{
label: goto label; // endless loop
}


void USART1_Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  														//复位串口1
	
	//USART1_TX发射引脚   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 						//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); 							//初始化PA9

	//USART1串口初始化设置
	USART_InitStructure.USART_BaudRate = baud;						
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 						//初始化串口
 	USART_Cmd(USART1, ENABLE);                    					//使能串口 
}


