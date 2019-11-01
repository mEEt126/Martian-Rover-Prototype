#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART_128.h"

void interrupt_activation(void);
void stop(void);
void take_u_turn(int u,int i,int o,int p);
void take_turn(int q,int w,int e,int r);

char receive_call;
volatile int a=1,b=1,c=1,d=1;
volatile int A=1,B=1,C=1,D=1;	

int main()
{
	DDRE = 0X00;
	DDRD = 0X00;
	DDRC = 0XFF;    // direction=variable// pwm=variable // 0-3 pins
	sei();

		
	interrupt_activation();
																																													
	USART_Init(51,0);				//9600 baud
	_delay_us(10);
	
	//USART_Transmitchar('1',0);
	USART_Init(51,1);
    _delay_us(10);
	//USART_Transmitchar('1',1);
	
	while(1)
		{
			//USART_Transmitchar('1',1);
			
			//USART_Transmitchar(receive_call,1);
			//receive_call='c';	
		//	USART_Transmitchar('q',1);
		
			receive_call=USART_Receive(0);
			if(receive_call=='c')
			{
				take_turn(a,b,c,d);
				//if(a==0 && b==0 && c==0 && d==0)
				//{
					//a=1;b=1;c=1;d=1;
				//}
				//USART_Transmitchar('C',1);
			}
			else if(receive_call=='r')
			{
				take_u_turn(A,B,C,D);
				if(A==0 && B==0 && C==0 && D==0)
				{
					A=1;B=1;C=1;D=1;
				}
				///USART_Transmitchar('R',1);
			}
			else
			{
				//stop();
				USART_Transmitchar('S',0);	
			}
		}
}
void interrupt_activation(void)
{
	EICRA = 0XFF;
	EICRB = 0XFF;							//interrupt  on rising edge
	EIMSK = 0XFF;						    //interrupt  is enabled
} 

void take_turn(int q,int w,int e,int r)
{
	int l=0;
	USART_Transmitchar('a',0);
	l= q*8	+  w*4  +  e*2 +  r*1 ; // a*8 + b*4 + c*2 + d*1
	_delay_us(10);
	USART_TransmitNumber(l,0);
	switch(l)
	{
		case 15:
		PORTC = 0XFF;
		USART_Transmitchar('F',0);
		break;
		
		case 14:
		PORTC = 0XEF;
		USART_Transmitchar('E',0);
		break;
		
		case 13:
		PORTC = 0XDF;
		USART_Transmitchar('D',0);		
		break;
		
		case 12:
		PORTC = 0XCF;
		USART_Transmitchar('C',0);
		break;
		
		case 11:
		PORTC = 0XBF;
		USART_Transmitchar('B',0);
		break;
		
		case 10:
		PORTC = 0XAF;
		USART_Transmitchar('A',0);
		break;
		
		case 9:
		PORTC = 0X9F;
		USART_Transmitchar('G',0);
		break;
		
		case 8:
		PORTC = 0X8F;
		USART_Transmitchar('H',0);
		break;
		
		case 7:
		PORTC = 0X7F;
		USART_Transmitchar('I',0);
		break;
		
		case 6:
		PORTC = 0X6F;
		USART_Transmitchar('J',0);
		break;
		
		case 5:
		PORTC = 0X5F;
		USART_Transmitchar('K',0);
		break;
		
		case 4:
		PORTC = 0X4F;
		USART_Transmitchar('L',0);
		break;
		
		case 3:
		PORTC = 0X3F;
		USART_Transmitchar('M',0);
		break;
		
		case 2:
		PORTC = 0X2F;
		USART_Transmitchar('N',0);
		break;
		
		case 1:
		PORTC = 0X1F;
		USART_Transmitchar('V',0);
		break;
		
		case 0:
		PORTC = 0X0F;
		USART_Transmitchar('i',0);
		break;
		
		default:
		USART_Transmitchar('w',0);
	}
}
void take_u_turn(int u,int i,int o,int p)
{
	int k=0;
	USART_Transmitchar('g',0);
	k= u*8	+  i*4  +  o*2 +  p*1;  // a*8 + b*4 + c*2 + d*1
	USART_TransmitNumber(k,0);
	switch(k)
	{
		case 15:
		PORTC = 0XF0;
		USART_Transmitchar('a',0);
		break;
		
		case 14:
		PORTC = 0XE0;
		USART_Transmitchar('b',0);
		break;
		
		case 13:
		PORTC = 0XD0;
		USART_Transmitchar('c',0);
		break;
		
		case 12:
		PORTC = 0XC0;
		USART_Transmitchar('d',0);
		break;
		
		case 11:
		PORTC = 0XB0;
		USART_Transmitchar('e',0);
		break;
		
		case 10:
		PORTC = 0XA0;
		USART_Transmitchar('f',0);
		break;
		
		case 9:
		PORTC = 0X90;
		USART_Transmitchar('g',0);
		break;
		
		case 8:
		PORTC = 0X80;
		USART_Transmitchar('h',0);
		break;
		
		case 7:
		PORTC = 0X70;
		USART_Transmitchar('i',0);
		break;
		
		case 6:
		PORTC = 0X60;
		USART_Transmitchar('j',0);
		break;
		
		case 5:
		PORTC = 0X50;
		USART_Transmitchar('k',0);
		break;
		
		case 4:
		PORTC = 0X40;
		USART_Transmitchar('m',0);
		break;
		
		case 3:
		PORTC = 0X30;
		USART_Transmitchar('n',0);
		break;
		
		case 2:
		PORTC = 0X20;
		USART_Transmitchar('o',0);
		break;
		
		case 1:
		PORTC = 0X10;
		USART_Transmitchar('p',0);
		break;
		
		case 0:
		PORTC = 0X00;
		USART_Transmitchar('q',0);
		break;
	}
}

void stop(void)
{
	// controll for steering system (complete closed)
}

ISR(INT0_vect)
{
	a=0;
}
ISR(INT1_vect)
{
	b=0;
}
ISR(INT2_vect)
{
	c=0;
}
ISR(INT3_vect)
{
	d=0;
}
ISR(INT4_vect)
{
	A=0;
}
ISR(INT5_vect)
{
	B=0;
}
ISR(INT6_vect)
{
	C=0;
}
ISR(INT7_vect)
{
	D=0;
}