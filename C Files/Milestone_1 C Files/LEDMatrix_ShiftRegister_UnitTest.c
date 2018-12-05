/*	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 
#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 
 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 
#include <util/delay.h>

#define SER 0
#define RCLK 1
#define SRCLK 2
#define SRCLR 3

#define SER2 4
#define RCLK2 5
#define SRCLK2 6
#define SRCLR2 7

unsigned char columns;

unsigned char rectangle [8] = { 0b00000000,
								0b00000000,
								0b00000000,
								0b01111000,
								0b01001000,
								0b01111000,
								0b00000000,
								0b00000000 };
								
char rowsfour [8] =  {0b00000000,
		0b01111110,
		0b01000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b01111110,
	0b00000000};
						
						
void transmit_data(unsigned char data, unsigned int regNum) 
{
	
	if (regNum == 1) 
	{
		PORTB &= ~(1 << RCLK);									//Set RCLK to 0.
		PORTB |= (1 << SRCLR);									//Set SRCLR 1.
	} 
	
	else 
	{
		PORTB &= ~(1 << RCLK2);									//Set RCLK to 0.
		PORTB |= (1 << SRCLR2);									//Set SRCLR 1.
	}
	
	if (regNum == 1)
	{
		
		for (unsigned int i = 0; i < 8; i++)
		{														//Char is 8 bits, so for loop iterates 8 times.
			PORTB &= ~(1 << SRCLK);								// Set SRCLR to 1 allowing data to be set, also clear SRCLK in preparation of sending data
			if ((data >> i) & 0x01)
			{
				PORTB |= (1 << SER);							// set SER = next bit of data to be sent.
			}
			
			else
			{
				PORTB &= ~(1 << SER);
			}
			PORTB |= (1 << SRCLK);	

		}
																// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
	}															// end for each bit of dataf(chip == 1) {
	
	else
	{
		
		for (unsigned int i = 0; i < 8; i++)
		{
			PORTB &= ~(1 << SRCLK2);	
																// Set SRCLR to 1 allowing data to be set, also clear SRCLK in preparation of sending data
			if ((data >> i) & 0x01)
			{
				PORTB |= (1 << SER2);							// set SER = next bit of data to be sent.	
			}

			else
			{
				PORTB &= ~(1 << SER2);
			}
			
			PORTB |= (1 << SRCLK2);

		}
			
	}
		
	if (regNum == 1)
	{
		PORTB |= (1 << RCLK);									// set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" register
	}
	else
	{
		PORTB |= (1 << RCLK2);											
	}

	PORTB = 0x00;												// clears all lines in preparation of a new transmission
	return;
	
}
						
enum LEDState {INIT,L0} led_state;

void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		
		columns = 0x7F;
		
		//Initialize Shift Registers
		transmit_data(0x00, 2);
		PORTD = 0x00;
		break;
		
		case L0:
			
			for (int i = 0; i < 8; i++)
			{
				transmit_data(columns, 2);
				transmit_data(rectangle[i], 1);
				columns = (columns >> 1) | 0x80;
				_delay_ms(10);
			}
			columns = 0x7F;
			
		break;
		
		default:
		break;
	}
	//Transitions
	switch(led_state){
		case INIT:
			led_state = L0;
		break;
		case L0:
			led_state = L0;
		break;
		default:
			led_state = INIT;
		break;
	}
}

void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(1); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRB = 0xFF; PORTB = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}