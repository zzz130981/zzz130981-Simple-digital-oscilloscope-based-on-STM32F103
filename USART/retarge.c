#include <stdio.h>
#include <rt_misc.h>
#pragma import(__use_no_semihosting_swi)
extern int SendChar(int ch); // 声明外部函数，在main文件中定义
extern int GetKey(void);
struct __FILE {
int handle; // Add whatever you need here
};

FILE __stdout;
FILE __stdin;
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
