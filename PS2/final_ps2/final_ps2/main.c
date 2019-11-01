#define F_CPU 1000000UL   //single tasking

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART_32.h"
#include <avr/sfr_defs.h>


#include "PS2.h"

//#define BAUD 103			//set after at commands mode
//#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)


enum {select, leftStick, rightStick, start, up, right, down, left}; //3rd byte from ps2 controller
enum {leftFront2, rightFront2, leftFront1, rightFront1, triangle_up, circle_right, cross_down, square_left}; // 4th byte from ps2 controller


uint8_t x,y,LEFT_JOYSTICK_Y,LEFT_JOYSTICK_X,RIGHT_JOYSTICK_Y,RIGHT_JOYSTICK_X,m,n;
int pressed[40]={0};

int isPressed(uint8_t dataByte, uint8_t dataBit);
void datapack(void);


int main(void)
{
	init_PS2();				
	_delay_us(2000);
	

	USART_Init(12);													//baud rate 38400
	USART_InterruptEnable();
	_delay_us(50);
	
	while (1)
	{
		scan_PS2();
		_delay_us(500);
		datapack();
		
		if(isPressed(x,start))
		{
			if(pressed[0]==0)
			{
				USART_Transmitchar('A');												//STOP COMMAND
				pressed[0]++;
			}	
		}
		else
		{
			pressed[0]=0;
				
		}		
									//---------------------------------------------DRIVING------------------------------------------------------//
		
															/* byte x specifies direction of motor and byte y specifies its speed*/
		
		if ((isPressed(x,up)) && (isPressed(y,triangle_up)))
		{	
			if(pressed[1]==0)
			{
				USART_Transmitchar('B');
				pressed[1]++;
			}
		}
		else
		{
			pressed[1]=0;	
		
		}
			
		if ((isPressed(x,up)) && (isPressed(y,circle_right)))	
		{
			if(pressed[2]==0)
			{
				USART_Transmitchar('C');
				pressed[2]++;
				
			}
		}
		else
		{
			pressed[2]=0;
		}
		
		if ((isPressed(x,up)) && (isPressed(y,cross_down)))	
		{
			if(pressed[3]==0)
			{
				USART_Transmitchar('D');
				pressed[3]++;
			}
		}
		else
		{
			pressed[3]=0;
		}
		
		if ((isPressed(x,up)) && (isPressed(y,square_left)))
		{
			if(pressed[4]==0)
			{
				USART_Transmitchar('E');
				pressed[4]++;
			}
		}
		else
		{
			pressed[4]=0;
		}
			
		if ((isPressed(x,down)) && (isPressed(y,triangle_up)))
		{
			if(pressed[5]==0)
			{
				USART_Transmitchar('F');
				pressed[5]++;
			}
		}
		else
		{
			pressed[5]=0;
		}
		
		if ((isPressed(x,down)) && (isPressed(y,circle_right)))	
		{
			if(pressed[6]==0)
			{
				USART_Transmitchar('G');
				pressed[6]++;
				
			}
		}
		else
		{
			pressed[6]=0;
		}
		
		if ((isPressed(x,down)) && (isPressed(y,cross_down)))	
		{
			if(pressed[7]==0)
			{
				USART_Transmitchar('H');
				pressed[7]++;
			}
		}
		else
		{
			pressed[7]=0;
		}
		
		if ((isPressed(x,down)) && (isPressed(y,square_left)))	
		{
			if(pressed[8]==0)
			{
				USART_Transmitchar('I');
				pressed[8]++;
			}
		}
			else
			{
				pressed[8]=0;
			}
			
		if ((isPressed(x,leftStick)) && (isPressed(x,rightStick)))  //reset for steering
		{
			if(pressed[35]==0)
			{
				USART_Transmitchar('I');
				pressed[35]++;
			}
		}                            
		else
		{
			pressed[35]=0;
		}
			
			
		
							//------------------------------------------------------------------TURNING--------------------------------------------------------------------------//
		
		
		if ((isPressed(x,left)) && (isPressed(y,triangle_up)))
		 	{
		 		if(pressed[9]==0)
		 		{
		 			USART_Transmitchar('J');
		 			pressed[9]++;
		 		}
		 	}
		 else
		 	{
		 		pressed[9]=0;
		 	}
		
		 	if ((isPressed(x,left)) && (isPressed(y,cross_down)))
		 	{
		 		if(pressed[10]==0)
		 		{
		 			USART_Transmitchar('K');
		 			pressed[10]++;
		 		}
		 	}
		 		else
		 		{
		 			pressed[10]=0;
		 		}
		
		 	if((isPressed(x,left)) && (isPressed(y,circle_right)))	
		 	{
		 		if(pressed[11]==0)
		 		{
		 			USART_Transmitchar('L');
		 			pressed[11]++;
		 		}
		 	}
		 	else
		 	{
		 		pressed[11]=0;
		 	}
		
		 	if ((isPressed(x,left)) && (isPressed(y,square_left)))
		 	{
		 		if(pressed[12]==0)
		 		{
		 			USART_Transmitchar('M');
		 			pressed[12]++;
		 		}
		 	}
		 		else
		 		{
		 			pressed[12]=0;
		 		}	
		
							//---------------------------------------------------------------------ARM------------------------------------------------------------------//
							
		
		if ((isPressed(y,leftFront1)) && (isPressed(y,triangle_up)))	
		{
			if(pressed[13]==0)
			{
				USART_Transmitchar('a');
				pressed[13]++;
			}
		}
			else
			{
				pressed[13]=0;
			}
		
		if ((isPressed(y,leftFront1)) && (isPressed(y,cross_down)))	
		{
			if(pressed[14]==0)
			{
				USART_Transmitchar('b');
				pressed[14]++;
			}
		}
			else
			{
				pressed[14]=0;
			}
		
		if ((isPressed(y,leftFront1)) && (isPressed(y,circle_right)))	
		{
			if(pressed[15]==0)
			{
				USART_Transmitchar('c');
				pressed[15]++;
			}
		}
			else
			{
				pressed[15]=0;
			}
		
		if ((isPressed(y,leftFront1)) && (isPressed(y,square_left)))	
		{
			if(pressed[16]==0)
			{
				USART_Transmitchar('d');
				pressed[16]++;
			}
		}
			else
			{
				pressed[16]=0;
			}
		
		if ((isPressed(y,leftFront1)) && (isPressed(x,right)))	
		{
			if(pressed[17]==0)
			{
				USART_Transmitchar('e');
				pressed[17]++;
			}
		}
			else
			{
				pressed[17]=0;
			}
		
		if ((isPressed(y,leftFront1)) && (isPressed(x,left)))	
		{
			if(pressed[18]==0)
			{
				USART_Transmitchar('f');
				pressed[18]++;
			}
		}
			else
			{
				pressed[18]=0;
			}
		
		if ((isPressed(y,rightFront1)) && (isPressed(y,triangle_up)))	
		{
			if(pressed[19]==0)
			{
				USART_Transmitchar('g');
				pressed[19]++;
			}
		}
			else
			{
				pressed[19]=0;
			}
		
		if ((isPressed(y,rightFront1)) && (isPressed(y,cross_down)))	
		{
			if(pressed[20]==0)
			{
				USART_Transmitchar('h');
				pressed[20]++;
			}
		}
			else
			{
				pressed[20]=0;
			}
		
		if ((isPressed(y,rightFront1)) && (isPressed(y,circle_right)))	
		{
			if(pressed[21]==0)
			{
				USART_Transmitchar('i');
				pressed[21]++;
			}
		}
			else
			{
				pressed[21]=0;
			}
		
		if ((isPressed(y,rightFront1)) && (isPressed(y,square_left)))	
		{
			if(pressed[22]==0)
			{
				USART_Transmitchar('j');
				pressed[22]++;
			}
		}
			else
			{
				pressed[22]=0;
			}
		
		if ((isPressed(y,rightFront1)) && (isPressed(x,right)))	
		{
			if(pressed[23]==0)
			{
				USART_Transmitchar('k');
				pressed[23]++;
			}
		}
			else
			{
				pressed[23]=0;
			}
		
		if ((isPressed(y,rightFront1)) && (isPressed(x,left)))	
		{
			if(pressed[24]==0)
			{
				USART_Transmitchar('l');
				pressed[24]++;
			}
		}
			else
			{
				pressed[24]=0;
			}
		
									//------------------------------------------------------------SENSOR--------------------------------------------------------------------------//
									
		
		if ((isPressed(y,rightFront2)) && (isPressed(y,triangle_up)))	
		{
			if(pressed[25]==0)
			{
				USART_Transmitchar('0');
				pressed[25]++;
			}
		}
			else
			{
				pressed[25]=0;
			}
		
		if ((isPressed(y,rightFront2)) && (isPressed(y,circle_right)))	
		{
			if(pressed[26]==0)
			{
				USART_Transmitchar('1');
				pressed[26]++;
			}0
		}
			else
			{
				pressed[26]=0;
			}
		
		if ((isPressed(y,rightFront2)) && (isPressed(y,cross_down)))	
		{
			if(pressed[27]==0)
			{
				USART_Transmitchar('2');
				pressed[27]++;
			}
		}
			else
			{
				pressed[27]=0;
			}
		
		if ((isPressed(y,rightFront2)) && (isPressed(y,square_left)))	
		{
			if(pressed[28]==0)
			{
				USART_Transmitchar('3');
				pressed[28]++;
			}
		}
			else
			{
				pressed[28]=0;
			}
		
		if ((isPressed(y,rightFront2)) && (isPressed(x,up)))
		{
			if(pressed[29]==0)
			{
				USART_Transmitchar('4');
				pressed[29]++;
			}
		}
			else
			{
				pressed[29]=0;
			}
		
		if ((isPressed(y,rightFront2)) && (isPressed(x,down)))
		{
			if(pressed[30]==0)
			{
				USART_Transmitchar('5');
				pressed[30]++;
			}
		}
			else
			{
				pressed[30]=0;
			}
			
		if ((isPressed(y,rightFront2))&&(isPressed(x,right)))
		{
			if(pressed[36]==0)
			{
				USART_Transmitchar('T');
				pressed[35]++;
			}
		}
			else
			{
				pressed[35]=0;
			}

							//-------------------------------------------------------CAMERA----------------------------------------------------------------//
		
		
		if((isPressed(y,leftFront2))&&(isPressed(y,triangle_up)))
		{
			if(pressed[31]==0)
			{
				USART_Transmitchar('6');
				pressed[31]++;
			}
		}
			else
			{
				pressed[31]=0;
			}
		
		if((isPressed(y,leftFront2))&&(isPressed(y,cross_down)))
		{
			if(pressed[32]==0)
			{
				USART_Transmitchar('7');
				pressed[32]++;
			}
		}
			else
			{
				pressed[32]=0;
			}
		
		if((isPressed(y,leftFront2))&&(isPressed(y,circle_right)))
		{
			if(pressed[33]==0)
			{
				USART_Transmitchar('8');
				pressed[33]++;
			}
		}
			else
			{
				pressed[33]=0;
			}
		
		if((isPressed(y,leftFront2))&&(isPressed(y,square_left)))
		{
			if(pressed[34]==0)
			{
				USART_Transmitchar('9');
				pressed[34]++;
			}
		}
			else
			{
				pressed[34]=0;
			}
	}
}


int isPressed(uint8_t dataByte, uint8_t dataBit)
{
	return ((dataByte & (1 << dataBit)) ? 1 : 0);
}

void datapack(void)
{
	x = ~data_array[3];
	y = ~data_array[4];
	RIGHT_JOYSTICK_X = ~data_array[5];
	RIGHT_JOYSTICK_Y = ~data_array[6];
	LEFT_JOYSTICK_X  = ~data_array[7];
	LEFT_JOYSTICK_Y  = ~data_array[8];
}

