#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART_128.h"

#define DIR_DDR_F	    DDRG
#define DIR_DDR_R	    DDRB    
#define DIR_PORT_F	    PORTG
#define DIR_PORT_R	    PORTB
#define  PWM_dir_F      DDRB
#define  PWM_dir_R      DDRE
#define  PWM_dir_R      DDRB

#define L298_DDR      DDRC
#define L298_PORT     PORTC
#define L298_PWM_DIR  DDRA
#define L298_PWM_PORT PORTA

#define DIR_LEFT_F		PING3
#define DIR_RIGHT_F		PING4
#define DIR_LEFT_R		PINB0
#define DIR_RIGHT_R		PINB1

#define dir_L1_1 PINC0
#define dir_L1_2 PINC1
#define dir_L2_1 PINC2
#define dir_L2_2 PINC3
#define dir_R1_1 PINC4
#define dir_R1_2 PINC5
#define dir_R2_1 PINC6
#define dir_R2_2 PINC7

#define PWM_LEFT_F      OCR1B
#define PWM_RIGHT_F     OCR1C
#define PWM_LEFT_R      OCR1A
#define PWM_RIGHT_R     OCR3A

#define PWM_L1 PINA0
#define PWM_L2 PINA1
#define PWM_R1 PINA2
#define PWM_R2 PINA3

void pwm_init1(void);
void pwm_init3(void);
void interrupt_activation(void);
void angle_turn(char);

void forward_drive(int);
void reverse_drive(int);

void L1_forward();
void L1_reverse();
void L2_forward();
void L2_reverse();
void R1_forward();
void R1_reverse();
void R2_forward();
void R2_reverse();

void stop(void); // for driving motors
void STOP(void); // for steering motors


char receive_call;
volatile int a=1,b=1,c=1,d=1;
volatile int A=1,B=1,C=1,D=1;

int main(void)
{
	DIR_DDR_F = 0XFF;
	DIR_DDR_R = 0XFF;
	L298_DDR     =   0XFF;
	L298_PWM_DIR =   0X0F;
	PWM_dir_R = 0XFF;
	sei();
 	USART_Init(8,0);				//57600 baud
 	USART_InterruptEnable(0);
 	_delay_us(10);
	
// 	USART_Init(51,1);
// 	USART_InterruptEnable(1);
// 	_delay_us(10);
	
	 pwm_init1();
	 pwm_init3();
	 interrupt_activation();
	 
	 UCSR0B&=~(1<<TXEN0);

	while (1)
	{	
		PORTD = 0X0F;	
		PORTE = 0XF0;
	} 

}
ISR(USART0_RX_vect)
{
	receive_call = USART_Receive(0);
	//USART_Transmitchar(receive_call,0);
	switch (receive_call)
	{
		case 'B':   
		forward_drive(1);
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(receive_call,0);
		break;
		
		case 'C':	
		forward_drive(2);
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(receive_call,0);
		break;
		
		case 'D':	
		forward_drive(3);
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(receive_call,0);
		break;
		
		case 'E':	
		forward_drive(4);
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(receive_call,0);
		break;
		
		case 'F':	
		reverse_drive(1);
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(receive_call,0);
		break;
		
		case 'G':	
		reverse_drive(2);
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(receive_call,0);
		break;
		
		case 'H':	
		reverse_drive(3);
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(receive_call,0);
		break;
		
		case 'I':	
		reverse_drive(4);
		UCSR0B|=(1<<TXEN0);
		USART_Transmitchar(receive_call,0);
		break;
		
	
		
		case 'J':	
			L1_forward();
			L2_forward();
			R1_forward();
			R2_forward();
			UCSR0B|=(1<<TXEN0);
			USART_Transmitchar(receive_call,0);
			break;
		
		case 'K': //for u_turn
			L1_reverse();
			L2_reverse();
			R1_reverse();
			R2_reverse();
			UCSR0B|=(1<<TXEN0);
			USART_Transmitchar(receive_call,0);
			break;
			
		case 'M': // left angle turn
			angle_turn('L');
			UCSR0B|=(1<<TXEN0);
			USART_Transmitchar(receive_call,0);
			break;
			
		case 'L': // RIGHT angle turn
			angle_turn('R');
			UCSR0B|=(1<<TXEN0);	
			USART_Transmitchar(receive_call,0);
			break;				
		
		case 'A':
			stop();
			STOP();
			UCSR0B|=(1<<TXEN0);
			USART_Transmitchar(receive_call,0);
			break;
			
		default:
			stop();
			STOP();
			UCSR0B|=(1<<TXEN0);
			USART_Transmitchar('z',0);
			break;	
	}	
}


void stop(void)
{
	PWM_LEFT_F = 0;
	PWM_RIGHT_F = 0;
	PWM_RIGHT_R =0;
	PWM_LEFT_R =0;
}
void STOP()
{
	L298_PWM_PORT = 0X00;
}


void pwm_init1(void)
{
	TCCR1A |= (1<<WGM11)|(1<<COM1A1)|(1<<COM1B1)|(1<<COM1C1);   //Fast PWM,Non Inverting mode
	TCCR1B |=(1<<WGM12)|( 1<<WGM13)|(1<<CS11);
	ICR1 = 10000;
}

void pwm_init3(void)
{
	TCCR3A |=( 1<<WGM31)|(1<<COM3A1);//Fast PWM ,Non Inverting mode   only 3A pin
	TCCR3B |=(1<<WGM32)|( 1<<WGM33)|(1<<CS31);
	ICR3 = 10000;
}

void interrupt_activation(void)
{
	EICRA = 0X00;
	EICRB = 0X00;							//interrupt LOW LEVEL
	EIMSK = 0XFF;						    //interrupt  is enabled
}
		
void forward_drive(int gear)
{
	switch(gear)
	{
		case 1:
			DIR_PORT_F = ((1<<DIR_LEFT_F)|(1<<DIR_RIGHT_F));
			DIR_PORT_R = ((1<<DIR_LEFT_R)|(1<<DIR_RIGHT_R));
		
			PWM_LEFT_F = 2500;
			PWM_RIGHT_F= 2500;
			PWM_LEFT_R = 2500;
			PWM_RIGHT_R= 2500;
			break;
		
		case 2:
			DIR_PORT_F = ((1<<DIR_LEFT_F)|(1<<DIR_RIGHT_F));
			DIR_PORT_R = ((1<<DIR_LEFT_R)|(1<<DIR_RIGHT_R));
			
			PWM_LEFT_F = 5000;
			PWM_RIGHT_F= 5000;
			PWM_LEFT_R = 5000;
			PWM_RIGHT_R= 5000;
			break;	
		
		case 3:
			DIR_PORT_F = ((1<<DIR_LEFT_F)|(1<<DIR_RIGHT_F));
			DIR_PORT_R = ((1<<DIR_LEFT_R)|(1<<DIR_RIGHT_R));
			
			PWM_LEFT_F = 6000;
			PWM_RIGHT_F= 6000;
			PWM_LEFT_R = 6000;
			PWM_RIGHT_R= 6000;
			
			break;	
		
		case 4:
			DIR_PORT_F = ((1<<DIR_LEFT_F)|(1<<DIR_RIGHT_F));
			DIR_PORT_R = ((1<<DIR_LEFT_R)|(1<<DIR_RIGHT_R));
			
			PWM_LEFT_F = 7500;
			PWM_RIGHT_F= 7500;
			PWM_LEFT_R = 7500;
			PWM_RIGHT_R= 7500;
			break;
			
	}
}

void reverse_drive(int re_gear)
{
	switch(re_gear)
	{
		case 1:
		DIR_PORT_F = ~((1<<DIR_LEFT_F)|(1<<DIR_RIGHT_F));
		DIR_PORT_R = ~((1<<DIR_LEFT_R)|(1<<DIR_RIGHT_R));
		
		PWM_LEFT_F = 2500;
		PWM_RIGHT_F= 2500;
		PWM_LEFT_R = 2500;
		PWM_RIGHT_R= 2500;
		break;
		
		case 2:
		DIR_PORT_F = ((1<<DIR_LEFT_F)|(1<<DIR_RIGHT_F));
		DIR_PORT_R = ((1<<DIR_LEFT_R)|(1<<DIR_RIGHT_R));
		
		PWM_LEFT_F = 5000;
		PWM_RIGHT_F= 5000;
		PWM_LEFT_R = 5000;
		PWM_RIGHT_R= 5000;
		break;
		
		case 3:
		DIR_PORT_F = ((1<<DIR_LEFT_F)|(1<<DIR_RIGHT_F));
		DIR_PORT_R = ((1<<DIR_LEFT_R)|(1<<DIR_RIGHT_R));
		
		PWM_LEFT_F = 6000;
		PWM_RIGHT_F= 6000;
		PWM_LEFT_R = 6000;
		PWM_RIGHT_R= 6000;
		break;
		
		case 4:
		DIR_PORT_F = ((1<<DIR_LEFT_F)|(1<<DIR_RIGHT_F));
		DIR_PORT_R = ((1<<DIR_LEFT_R)|(1<<DIR_RIGHT_R));
		
		PWM_LEFT_F = 7500;
		PWM_RIGHT_F= 7500;
		PWM_LEFT_R = 7500;
		PWM_RIGHT_R= 7500;
		break;
		
	}
}

void angle_turn(char angle)
{
	if(angle=='L')
	{	DIR_PORT_F = 1<<DIR_RIGHT_F;
		DIR_PORT_R = 1<<DIR_RIGHT_R;
		DIR_PORT_F &= ~(1<<DIR_LEFT_F);
		DIR_PORT_R &= ~(1<<DIR_LEFT_R);
		PWM_LEFT_F = 5000;
		PWM_RIGHT_F= 5000;
		PWM_LEFT_R = 5000;
		PWM_RIGHT_R= 5000;
	}
	else 
	{
	DIR_PORT_F = (1<<DIR_LEFT_F);
	DIR_PORT_R = (1<<DIR_LEFT_R);
	DIR_PORT_F = ~(1<<DIR_RIGHT_F);
	DIR_PORT_R = ~(1<<DIR_RIGHT_R);
		PWM_LEFT_F = 5000;
		PWM_RIGHT_F= 5000;
		PWM_LEFT_R = 5000;
		PWM_RIGHT_R= 5000;
		
	}
}	

ISR(INT0_vect)
{
	L1_forward();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('0',0);
	_delay_ms(200);
	L298_PWM_PORT &= ~(1<<PWM_L1);
}

ISR(INT1_vect)
{
	L1_reverse();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('1',0);
	_delay_ms(200);
	L298_PWM_PORT &= ~(1<<PWM_L1);
}

ISR(INT2_vect)
{
	L2_forward();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('2',0);
	_delay_ms(100);
	L298_PWM_PORT &= ~(1<<PWM_L2);
}

ISR(INT3_vect)
{
	L2_reverse();
	
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('3',0);
	_delay_ms(100);
	L298_PWM_PORT &= ~(1<<PWM_L2);
}

ISR(INT4_vect)
{
	
	R1_reverse();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('4',0);
	_delay_ms(100);
	L298_PWM_PORT &= ~(1<<PWM_R1);
}

ISR(INT5_vect)
{
	R1_forward();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('5',0);
	_delay_ms(100);
	L298_PWM_PORT &= ~(1<<PWM_R1);
}

ISR(INT6_vect)
{
	R2_forward();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('6',0);
	_delay_ms(100);
	L298_PWM_PORT &= ~(1<<PWM_R2);
}

ISR(INT7_vect)
{
	
	R2_reverse();
	UCSR0B|=(1<<TXEN0);
	USART_Transmitchar('7',0);
	_delay_ms(100);
	L298_PWM_PORT &= ~(1<<PWM_R2);
}

void L1_forward()
{
	L298_PORT |= 1<<dir_L1_1;
	L298_PORT &= ~(1<<dir_L1_2);
	L298_PWM_PORT |=  1<<PWM_L1;
}

void L1_reverse()
{
	L298_PORT |= 1<<dir_L1_2;
	L298_PORT &= ~(1<<dir_L1_1);
	L298_PWM_PORT |=  1<<PWM_L1;
}

void L2_forward()
{
	L298_PORT |= 1<<dir_L2_1;
	L298_PORT &= ~(1<<dir_L2_2);
	L298_PWM_PORT |=  1<<PWM_L2;
}

void L2_reverse()
{
	L298_PORT |= 1<<dir_L2_2;
	L298_PORT &= ~(1<<dir_L2_1);
	L298_PWM_PORT |=  1<<PWM_L2;
}

void R1_forward()
{
	L298_PORT |= 1<<dir_R1_1;
	L298_PORT &= ~(1<<dir_R1_2);
	L298_PWM_PORT |=  1<<PWM_R1;
}

void R1_reverse()
{
	L298_PORT |= 1<<dir_R1_2;
	L298_PORT &= ~(1<<dir_R1_1);
	L298_PWM_PORT |=  1<<PWM_R1;
}

void R2_forward()
{
	L298_PORT |= 1<<dir_R2_1;
	L298_PORT &= ~(1<<dir_R2_2);
	L298_PWM_PORT |=  1<<PWM_R2;
}

void R2_reverse()
{
	L298_PORT |= 1<<dir_R2_2;
	L298_PORT &= ~(1<<dir_R2_1);
	L298_PWM_PORT |=  1<<PWM_R2;
}
	