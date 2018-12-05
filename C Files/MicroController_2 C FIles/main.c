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

unsigned char inpin = 0;

unsigned char modify[8];
unsigned char init [8] = {  0b11111111,
							0b10011001,
							0b10011001,
							0b11111111,
							0b11111111,
							0b10011001,
							0b10011001,
							0b11111111 };
							
unsigned char c1   [8] = {  0b11111111,
							0b11011001,
							0b10111001,
							0b11111111,
							0b11111111,
							0b10011001,
							0b10011001,
						    0b11111111 };
							
unsigned char c2   [8] = {  0b11111111,
							0b10011101,
							0b10011011,
							0b11111111,
							0b11111111,
							0b10011001,
							0b10011001,
						    0b11111111 };
							
unsigned char c3   [8] = {  0b11111111,
							0b10011001,
							0b10011001,
							0b11111111,
							0b11111111,
							0b11011001,
							0b10111001,
						    0b11111111 };
							
unsigned char c4   [8] = {  0b11111111,
							0b10011001,
							0b10011001,
							0b11111111,
							0b11111111,
							0b10011101,
							0b10011011,
							0b11111111 };
							

unsigned char columns;


enum LEDState {INIT,L0} led_state;

void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
			columns = 0x7F;
		break;
		
		case L0:
			for (int i = 0; i < 8; i++)
			{
				PORTA = modify[i];
				PORTD = columns;
				columns = (columns >> 1) | 0x80;
				_delay_ms(5);
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

enum RIState {RIINIT,READ, WAIT} RI_state;

void RI_Init(){
	RI_state = RIINIT;
}

void RI_Tick(){
	//Actions
	switch(RI_state){
		case RIINIT:
			for (int j = 0; j < 8; j++)
			{
				modify[j] = init[j];
			}
		break;
		
		case READ:
			inpin = ~PINB;
			if (inpin == 0x01)
			{
				for (int j = 0; j < 8; j++)
				{
					modify[j] = modify[j] | c1[j];
				}
			}
			else if (inpin == 0x02)
			{

				for (int j = 0; j < 8; j++)
				{
					modify[j] = modify[j] | c2[j];
				}

			}
			else if (inpin == 0x04)
			{
				for (int j = 0; j < 8; j++)
				{
					modify[j] = modify[j] | c3[j];
				}
			}
			else if (inpin == 0x08)
			{
				for (int j = 0; j < 8; j++)
				{
					modify[j] = modify[j] | c4[j];
				}
			}
			else if (inpin == 0x09)
			{
				for (int j = 0; j < 8; j++)
				{
					modify[j] = modify[j] ^ (c1[j] ^ init[j]);
				}
			}
			else if (inpin == 0x0A)
			{
				for (int j = 0; j < 8; j++)
				{
					modify[j] = modify[j] ^ (c2[j] ^ init[j]);
				}
			}
			else if (inpin == 0x0B)
			{
				for (int j = 0; j < 8; j++)
				{
					modify[j] = modify[j] ^ (c3[j] ^ init[j]);
				}
			}
			else if (inpin == 0x0C)
			{
				for (int j = 0; j < 8; j++)
				{
					modify[j] = modify[j] ^ (c4[j] ^ init[j]);
				}
			}
		break;
		
		default:
		break;
	}
	//Transitions
	switch(RI_state){
		case INIT:
			RI_state = READ;
		break;
		
		case READ:
			RI_state = READ;
		break;
		
		default:
			RI_state = RIINIT;
		break;
	}
}

void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(15); 
   } 
}

void RITask()
{
	RI_Init();
	for(;;)
	{
		RI_Tick();
		vTaskDelay(10);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(RITask, (signed portCHAR *)"RITask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
	DDRB = 0x00; PORTB = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0xFF; PORTA = 0x00;
	
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}