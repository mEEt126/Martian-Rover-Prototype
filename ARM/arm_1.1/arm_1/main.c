#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART_128.h"
//---------------------------------------PORT--------------------------------------------------------------------------//
#define CYT_DDR   DDRA
#define CYT_PORT  PORTA

#define HER_DDR    DDRB
#define HER_PORT   PORTB

//-----------------------------------------Cytron-------------------------------------------------------------------------//
#define pow_M1  PINA7   
#define pow_M2	PINA5
#define pow_M3	PINA3
#define pow_M4	PINA1
#define dir_M1  PINA6
#define dir_M2	PINA4
#define dir_M3	PINA2
#define dir_M4	PINA0
//----------------------------------------Hercules-------------------------------------------------------------------------//
#define dir1_M5	PINB0
#define dir2_M5	PINB4
#define dir1_M6	PINB5
#define dir2_M6 PINB7
#define pow_M5  PINB1
#define pow_M6	PINB6
//----------------------------------------------------------------------------------------------------------------------------------//
#define int_pin1 PD1
#define int_pin2 PD0
//-----------------------------------------functions---------------------------------------------------------------------------//
void M1_forward();
void M1_reverse();
void M2_forward();
void M2_reverse();
void M3_forward();
void M3_reverse();
void M4_forward();
void M4_reverse();
void M5_forward();
void M5_reverse();
void M6_forward();
void M6_reverse();
void STOP();
void pwm();
//------------------------------------------main--------------------------------------------------------------------//
int main()
{ 
	CYT_DDR = 0xFF;
	HER_DDR = 0xF3;
	sei();
	USART_Init(12,0);												//57600 baud for telemetry
	//USART_Init(12,0);												//38400 baud
	USART_InterruptEnable(0);
	_delay_us(20);
	
	EIMSK = 0X03;													// ex_int2 & ex_int1 enable
	EICRA = 0X00;													// low_level 
	
//-------------------------------------------------PWM Timer1--------------------------------------------------------//
	TCCR1A |= (1<<WGM11)|(1<<COM1B1);
	TCCR1B |= (1<<WGM12)|( 1<<WGM13)|(1<<CS11);
	ICR1=1000;
	
	UCSR0B &= ~(1<<TXEN0);	
	
	while (1)
	{
 		PORTD|=(1<<int_pin1)|(1<<int_pin2);
	}
}

ISR(USART0_RX_vect)
{
	char joint_selection;
	joint_selection = USART_Receive(0);
	switch(joint_selection)
	{
		case 'a':		
		M1_forward();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'b':		
		M1_reverse();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'c':		
		M2_forward();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'd':		
		M2_reverse();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'e':		
		M3_forward();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'f':		
		M3_reverse();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'g':		
		M4_forward();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'h':		
		M4_reverse();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'i':		
		M5_forward();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'j':		
		M5_reverse();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'k':
		M6_forward();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		case 'l':
		M6_reverse();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		
		case 'A':
		STOP();
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(joint_selection,0);
		USART_Transmitchar('\n',0);
		USART_Transmitchar('\r',0);
		break;
		
		default:
		UCSR0B &= ~(1<<TXEN0);
		STOP();
		break;
	}
}



ISR(INT0_vect)
{
	M5_forward();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('y',0);
	USART_Transmitchar('\n',0);
	USART_Transmitchar('\r',0);
	_delay_ms(100);
	HER_PORT=0x00;
	OCR1C=0;
}

ISR(INT1_vect)
{
	M5_reverse();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('z',0);
	USART_Transmitchar('\n',0);
	USART_Transmitchar('\r',0);
	_delay_ms(100);
	HER_PORT=0x00;
	OCR1C=0;
}


void STOP()
{
	CYT_PORT=0x00;
	HER_PORT=0x00;
	OCR1C=0;
}
//---------------------------------------------------Cytron----------------------------------------------------------------//

void M1_forward()
{
	CYT_PORT= (1<<pow_M1);	
	HER_PORT = 0X00; 
}
void M1_reverse()
{
	CYT_PORT=((1<<dir_M1)|(1<<pow_M1));
	HER_PORT = 0X00; 
}
void M2_forward()
{
	CYT_PORT= (1<<pow_M2);
	HER_PORT = 0X00; 	
}
void M2_reverse()
{
	CYT_PORT=((1<<dir_M2)|(1<<pow_M2));
	HER_PORT = 0X00; 
}
void M3_forward()
{
	CYT_PORT= (1<<pow_M3);	
	HER_PORT = 0X00; 
}
void M3_reverse()
{
	CYT_PORT=((1<<dir_M3)|(1<<pow_M3));
	HER_PORT = 0X00; 
}
void M4_forward()
{
	CYT_PORT= (1<<pow_M4);
	HER_PORT = 0X00; 
}
void M4_reverse()
{
	CYT_PORT=((1<<dir_M4)|(1<<pow_M4));
	HER_PORT = 0X00; 
}

//-----------------------------------------------------------Hercules----------------------------------------------------------------------------//

void M5_forward()
{	
	HER_PORT=((1<<dir1_M5)|(1<<pow_M5));
	CYT_PORT = 0X00;
}
void M5_reverse()
{	
	HER_PORT=((1<<dir2_M5)|(1<<pow_M5));
	CYT_PORT = 0X00;
}
void M6_forward()
{
	CYT_PORT = 0X00;
	HER_PORT = 0X00;
	TCCR1A |= (1<<WGM11)|(1<<COM1B1);
	TCCR1B |= (1<<WGM12)|( 1<<WGM13)|(1<<CS11);
	ICR1=1000;
	HER_PORT|=(1<<dir1_M6);
	OCR1B = 650;
}
void M6_reverse()
{
	CYT_PORT = 0X00;
	HER_PORT = 0X00;
	TCCR1A |= (1<<WGM11)|(1<<COM1B1);
	TCCR1B |= (1<<WGM12)|( 1<<WGM13)|(1<<CS11);
	ICR1=1000;
	HER_PORT |= (1<<dir2_M6);
	OCR1B = 650;
	//pwm();
}