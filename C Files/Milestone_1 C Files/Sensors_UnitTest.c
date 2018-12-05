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
#include "ADC.h"

unsigned char button;
unsigned char portcheck;
unsigned char scaled; 

enum LEDState {INIT,L0} led_state;

void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
			button = 0;
			portcheck = 0;
		break;
	
		case L0:

			if (portcheck == 3)
			{
				portcheck = 0;
			}
			else
			{
				portcheck++;
			}
			if (portcheck == 0)
			{
				PORTD = 0x01;
			}
			else if (portcheck == 1)
			{
				PORTD = 0x02;
			}
			else if (portcheck == 2)
			{
				PORTD = 0x04;
			}
			else if (portcheck == 3)
			{
				PORTD = 0x08;
			}
			
			if (portcheck < 3)
			{
				scaled = Read_ADC(portcheck);
				PORTB = scaled / 4;
			}
			else if (portcheck == 3)
			{
				PORTB = PINC & 0x01;
			}
			
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
	vTaskDelay(5000); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
	DDRA = 0x00; PORTA = 0xFF; //A IS ADC INPUT
	DDRB = 0xFF; PORTB = 0x00; //B IS OUTPUT
	DDRC = 0x00; PORTC = 0xFF; //C IS BUTTON INPUT
	DDRD = 0xFF; PORTD = 0x00; //D IS OUTPUT
	
	ADC_init();
	
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}