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
 
#include <util/delay.h>

//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 
#include "io.c"
#include "keypad.h"
#include "ADC.h"

#define P0 0
#define P1 1
#define P2 2
#define P3 3

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

//-------------LCD Variables--------------------------------------------------------------------------------
unsigned char title [32] = {'R', '-', 'G', 'R', 'I', 'D', ' ', 'V', 'A', 'R', 'I', 'A', 'B', 'L', 'E', ' ',
'S', 'T', 'O', 'R', 'A', 'G', 'E', ' ', ' ', 'P', 'R', 'E', 'S', 'S', ' ', 'A'};

unsigned char menu [32] = {'1', '-', 'I', 'N', 'S', 'E', 'R', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
'2', '-', 'R', 'E', 'M', 'O', 'V', 'E', ' ', '3', '-', 'M', 'O', 'R', 'E', ' '};

unsigned char menu2 [32] = {'1', '-', 'C', 'H', 'E', 'C', 'K', 'T', 'E', 'M', 'P', ' ', ' ', ' ', ' ', ' ',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '5', '-', 'B', 'A', 'C', 'K'};


unsigned char insert [32] = {'W', 'H', 'I', 'C', 'H', ' ', 'C', 'E', 'L', 'L', ' ', 'T' , 'O', ' ', ' ', ' ',
'I', 'N', 'S', 'E', 'R', 'T', '?', '(', '5', ',', '6', ',', '8', ',', '9', ')'};

unsigned char inserta [32] = {'P', 'L', 'A', 'C', 'E', ' ', 'I', 'T', 'E', 'M', ' ', 'O' , 'N', ' ', ' ', ' ',
'F', 'S', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

unsigned char insertb [32] = {'C', 'E', 'L', 'L', ' ', 'O', 'C', 'C', 'U', 'P', 'I', 'E' , 'D', ' ', ' ', ' ',
'C', 'H', 'O', 'O', 'S', 'E', ' ', 'D', 'I', 'F', 'F', 'E', 'R', 'N', 'T', ' '};

unsigned char insertc [32] = {'I', 'N', 'S', 'E', 'R', 'T', ' ', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ', ' ',
'S', 'U', 'C', 'C', 'E', 'S', 'S', 'F', 'U', 'L', 'L', ' ', ' ', ' ', ' ', ' '};

unsigned char rem [32] = {'W', 'H', 'I', 'C', 'H', ' ', 'C', 'E', 'L', 'L', ' ', 'T' , 'O', ' ', ' ', ' ',
'R', 'E', 'M', 'O', 'V', 'E', '?', '(', '5', ',', '6', ',', '8', ',', '9', ')'};

unsigned char rema [32] = {'R', 'E', 'M', 'O', 'V', 'E', ' ', 'I', 'T', 'E', 'M', ' ' , 'F', 'R', 'O', 'M',
'F', 'S', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

unsigned char remb [32] = {'N', 'O', ' ', 'I', 'T', 'E', 'M', ' ', 'I', 'N', ' ', ' ' , 'C', 'E', 'L', 'L',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

unsigned char remc [32] = {'R', 'E', 'M', 'O', 'V', 'A', 'L', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ', ' ',
'S', 'U', 'C', 'C', 'E', 'S', 'S', 'F', 'U', 'L', 'L', ' ', ' ', ' ', ' ', ' '};
//----------------------------------------------------------------------------------------------------------

//-------------Keypad Variables-----------------------------------------------------------------------------
unsigned char Choice;
//----------------------------------------------------------------------------------------------------------

//-------------Storage Variables----------------------------------------------------------------------------
int storage[4] = {0, 0, 0, 0};
unsigned char cell = 0;
unsigned char placed = 0;
unsigned char removed = 0;
unsigned char reading = 0;
//----------------------------------------------------------------------------------------------------------

//-----------Opcode Variables-------------------------------------------------------------------------------
int opcode [4] = {0, 0, 0, 0};
int opavail = 0;
//----------------------------------------------------------------------------------------------------------

//-------------Other Variables------------------------------------------------------------------------------
int j = 0;
int p = 0;
int IR = 0; // 1 for insert, 2 for remove;
int T = 4;
int readtemp = 0;
//----------------------------------------------------------------------------------------------------------


//--------------------------Display Out on the LCD----------------------------------------------------------
enum LCDState {INIT,TITLE, MENU, MENU2, INSERT, INSERTA, INSERTB, INSERTC, 
			   REMOVE, REMOVEA, REMOVEB, REMOVEC, TEMP} LCD_state;

void LCD_Init(){
	LCD_state = INIT;
}

void LCD_Tick(){
	//Actions
	switch(LCD_state){
		
		case INIT:
		break;
		
		case TITLE:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(title[j]);
				j++;
			}
			j = 0;
		break;
		
		case MENU:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(menu[j]);
				j++;
			}
			j = 0;
		break;
		
		case MENU2:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(menu2[j]);
				j++;
			}
			j = 0;
		break;
				
		case INSERT:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(insert[j]);
				j++;
			}
			j = 0;
		break;
		
		case INSERTA:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(inserta[j]);
				j++;
			}
			j = 0;
		break;
		
		case INSERTB:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(insertb[j]);
				j++;
			}
			j = 0;
		break;
		
		case INSERTC:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(insertc[j]);
				j++;
			}
			j = 0;
		break;
		
		case REMOVE:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(rem[j]);
				j++;
			}
			j = 0;
		break;
		
		case REMOVEA:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(rema[j]);
				j++;
			}
			j = 0;
		break;
		
		case REMOVEB:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(remb[j]);
				j++;
			}
			j = 0;
		break;
		
		case REMOVEC:
			for (int i = 1; i < 33; i++)
			{
				LCD_Cursor(i);
				LCD_WriteData(remc[j]);
				j++;
			}
			j = 0;
		break;
		
		case TEMP:
			if (T == 7)
			{
				T = 4;
			}
			else
			{
				T = T + 1;
			}
			readtemp = Read_ADC(T);
			readtemp = readtemp * (3300/1024);
			readtemp = readtemp / 10;
			char str[32];
			itoa(readtemp, str, 10);
			LCD_DisplayString(1, str);
		break;
		
		default:
		break;
	}
	//Transitions
	switch(LCD_state){
		case INIT:
			LCD_state = TITLE;
		break;
		
		case TITLE:
			if (Choice != 'A')
			{
				LCD_state = TITLE;
			}
			else
			{
				LCD_state = MENU;
			}

		break;
		
		case MENU:
			if (Choice == '1')
			{
				LCD_state = INSERT;
			}
			else if (Choice == '2')
			{
				LCD_state = REMOVE;
			}
			else if (Choice == '3')
			{
				LCD_state = MENU2;
			}
			else
			{
				LCD_state = MENU;
			}
		break;
		
		case MENU2:
			if (Choice == '5')
			{
				LCD_state = MENU;
			}
			else if (Choice == '1')
			{
				LCD_state = TEMP;
			}
			else
			{
				LCD_state = MENU2;
			}
			
		break;
		
		case INSERT:
			if (Choice == '5')
			{
				if (storage[0] == 0)
				{
					cell = 1;
					IR = 1;
					LCD_state = INSERTA;
				}
				else
				{
					LCD_state = INSERTB;
				}
			}
			else if (Choice == '6')
			{
				if (storage[1] == 0)
				{
					cell = 2;
					IR = 1;
					LCD_state = INSERTA;
				}
				else
				{
					LCD_state = INSERTB;
				}
			}
			else if (Choice == '8')
			{
				if (storage[2] == 0)
				{
					cell = 3;
					IR = 1;
					LCD_state = INSERTA;
				}
				else
				{
					LCD_state = INSERTB;
				}
			}
			else if (Choice == '9')
			{
				if (storage[3] == 0)
				{
					cell = 4;
					IR = 1;
					LCD_state = INSERTA;
				}
				else
				{
					LCD_state = INSERTB;
				}
			}
			else if (Choice == 'B')
			{
				LCD_state = MENU;
			}
			else
			{
				LCD_state = INSERT;
			}
		break;
		
		case INSERTA:
			if (placed == 1)
			{
				LCD_state = INSERTC;
				if (cell == 1)
				{
					opavail = 1;
					storage[0] = 1;
				}
				else if (cell == 2)
				{
					opavail = 2;
					storage[1] = 1;
				}
				else if (cell == 3)
				{
					opavail = 3;
					storage[2] = 1;
				}
				else if (cell == 4)
				{
					opavail = 4;
					storage[3] = 1;
				}
				placed = 0;
				cell = 0;
			}
			else if (Choice == 'B')
			{
				LCD_state = MENU;
			}
			else
			{
				LCD_state = INSERTA;
			}
			
		break;
		
		case INSERTB:
			LCD_state = INSERT;
		break;
		
		case INSERTC:
			LCD_state = MENU;
		break;
		
		case REMOVE:
			if (Choice == '5')
			{
				if (storage[0] == 1)
				{
					cell = 1;
					IR = 2;
					LCD_state = REMOVEA;
				}
				else
				{
					LCD_state = REMOVEB;
				}
			}
			else if (Choice == '6')
			{
				if (storage[1] == 1)
				{
					cell = 2;
					IR = 2;
					LCD_state = REMOVEA;
				}
				else
				{
					LCD_state = REMOVEB;
				}
			}
			else if (Choice == '8')
			{
				if (storage[2] == 1)
				{
					cell = 3;
					IR = 2;
					LCD_state = REMOVEA;
				}
				else
				{
					LCD_state = REMOVEB;
				}
			}
			else if (Choice == '9')
			{
				if (storage[3] == 1)
				{
					cell = 4;
					IR = 2;
					LCD_state = REMOVEA;
				}
				else
				{
					LCD_state = REMOVEB;
				}
			}
			else if (Choice == 'B')
			{
				LCD_state = MENU;
			}
			else
			{
				LCD_state = REMOVE;
			}
			
		break;
		
		case REMOVEA:
			if (removed == 1)
			{
				LCD_state = REMOVEC;
				if (cell == 1)
				{
					opavail = 5;
					storage[0] = 0;
				}
				else if (cell == 2)
				{
					opavail = 6;
					storage[1] = 0;
				}
				else if (cell == 3)
				{
					opavail = 7;
					storage[2] = 0;
				}
				else if (cell == 4)
				{
					opavail = 8;
					storage[3] = 0;
				}
				removed = 0;
				cell = 0;
			}
			else if (Choice == 'B')
			{
				LCD_state = MENU;
			}
			else
			{
				LCD_state = REMOVEA;
			}
		break;
		
		case REMOVEB:
			LCD_state = REMOVE;
		break;
		
		case REMOVEC:
			LCD_state = MENU;
		break;
		
		case TEMP:
			if (Choice == 'B')
			{
				LCD_state = MENU;
			}
			else
			{
				LCD_state = TEMP;	
			}
		break;
		
		default:
			LCD_state = INIT;
		break;
	}
}
//------------------------------------------------------------------------------




//-------------------Take Input from Keypad---------------------
enum KPState {KPINIT,READ,WAIT} KP_state;

void KP_Init(){
	KP_state = KPINIT;
}

void KP_Tick(){
	//Actions
	switch(KP_state){
		
		case KPINIT:
			Choice = GetKeypadKey();
		break;
		
		case READ:
			Choice = GetKeypadKey();
		break;
		
		case WAIT:
			Choice = GetKeypadKey();
		break;
		
		default:
		break;
	}
	//Transitions
	switch(KP_state){
		case KPINIT:
			KP_state = READ;
		break;
		
		case READ:
			if (Choice == '\0')
			{
				KP_state = READ;
			}
			else
			{
				KP_state = WAIT;
			}
		break;
		
		case WAIT:
			if (Choice != '\0')
			{
				KP_state = WAIT;
			}
			else
			{
				KP_state = READ;
			}
		break;
		
		default:
			KP_state = KPINIT;
		break;
	}
}
//----------------------------------------------------------------------


//----------------------READS FOR STORE AND REMOVE----------------------
enum STOREState {STOREINIT, STIDLE, STREAD} STORE_state;

void STORE_Init(){
	STORE_state = STOREINIT;
}

void STORE_Tick(){
	//Actions
	switch(STORE_state){
		
		case STOREINIT:
		break;
		
		case STIDLE:
		break;
		
		case STREAD:
			if (IR == 1)
			{
				if (cell == 1)
				{
					reading = Read_ADC(0);
					if (reading > 100)
					{
						placed = 1;
						IR = 0;
					}
				}
				else if (cell == 2)
				{
					reading = Read_ADC(1);
					if (reading > 100)
					{
						placed = 1;
						IR = 0;
					}
				}
				else if (cell == 3)
				{
					reading = Read_ADC(2);
					if (reading > 100)
					{
						placed = 1;
						IR = 0;
					}
				}
				else if (cell == 4)
				{
					reading = Read_ADC(3);
					if (reading > 100)
					{
						placed = 1;
						IR = 0;
					}
				}
			}
			else if (IR == 2)
			{
				if (cell == 1)
				{
					reading = Read_ADC(0);
					if (reading < 100)
					{
						removed = 1;
						IR = 0;
					}
				}
				else if (cell == 2)
				{
					reading = Read_ADC(1);
					if (reading < 100)
					{
						removed = 1;
						IR = 0;
					}
				}
				else if (cell == 3)
				{
					reading = Read_ADC(2);
					if (reading < 100)
					{
						removed = 1;
						IR = 0;
					}
				}
				else if (cell == 4)
				{
					reading = Read_ADC(3);
					if (reading < 100)
					{
						removed = 1;
						IR = 0;
					}
				}
			}
		break;
		
		default:
		break;
	}
	//Transitions
	switch(STORE_state){
		case STOREINIT:
			STORE_state = STIDLE;
		break;
		
		case STIDLE:
			if (cell == 0)
			{
				STORE_state = STIDLE;
			}
			else 
			{
				STORE_state = STREAD;
			}
		break;
		
		case STREAD:
			if (cell != 0)
			{
				STORE_state = STREAD;
			}
			else
			{
				STORE_state = STIDLE;
			}
		break;
		
		default:
			STORE_state = STOREINIT;
		break;
	}
}

//---------------------------------------------------------------


//--------------------Transmit Opcode to Wireless----------------
enum OPState {OPINIT,OPREAD} OP_state;

void OP_Init(){
	OP_state = OPINIT;
}

void OP_Tick(){
	//Actions
	switch(OP_state){
		
		case OPINIT:
		break;
		
		case OPREAD:
			p = p + 1;
			if (opavail == 1)
			{
				PORTB = PORTB | 0b00001000;
			}
			else if (opavail == 2)
			{
				PORTB = PORTB | 0b00001001;
			}
			else if (opavail == 3)
			{
				PORTB = PORTB | 0b00001010;
			}
			else if (opavail == 4)
			{
				PORTB = PORTB | 0b00001011;
			}
			else if (opavail == 5)
			{
				PORTB = PORTB | 0b00000100;
			}
			else if (opavail == 6)
			{
				PORTB = PORTB | 0b00000101;
			}
			else if (opavail == 7)
			{
				PORTB = PORTB | 0b00000110;
			}
			else if (opavail == 8)
			{
				PORTB = PORTB | 0b00000111;
			}
			
			if (p == 10)
			{
				p = 0;
				if  (opavail == 1)
				{
					PORTB = PORTB ^ 0b00001000;
					opavail = 0;	
				}
				else if  (opavail == 2)
				{
					PORTB = PORTB ^ 0b00001001;
					opavail = 0;
				}
				else if  (opavail == 3)
				{
					PORTB = PORTB ^ 0b00001010;
					opavail = 0;
				}
				else if  (opavail == 4)
				{
					PORTB = PORTB ^ 0b00001011;
					opavail = 0;
				}
				else if  (opavail == 5)
				{
					PORTB = PORTB ^ 0b00000100;
					opavail = 0;
				}
				else if  (opavail == 6)
				{
					PORTB = PORTB ^ 0b00000101;
					opavail = 0;
				}
				else if  (opavail == 7)
				{
					PORTB = PORTB ^ 0b00000110;
					opavail = 0;
				}
				else if  (opavail == 8)
				{
					PORTB = PORTB ^ 0b00000111;
					opavail = 0;
				}
			}
		break;
			
		default:
		break;
	}
	//Transitions
	switch(OP_state){
		case OPINIT:
			OP_state = OPREAD;
		break;
		
		case OPREAD:
			OP_state = OPREAD;
		break;
		
		default:
			OP_state = OPINIT;
		break;
	}
}
//---------------------------------------------------------

void LCDTask()
{
	LCD_Init();
    for(;;) 
    { 	
		LCD_Tick();
		vTaskDelay(1000); 
    } 
}

void KPTask()
{
	KP_Init();
	for(;;)
	{
		KP_Tick();
		vTaskDelay(25);
	}
}

void STORETask()
{
	STORE_Init();
	for(;;)
	{
		STORE_Tick();
		vTaskDelay(25);
	}
}

void OPTask()
{
	OP_Init();
	for(;;)
	{
		OP_Tick();
		vTaskDelay(1000);
	}
}


void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LCDTask, (signed portCHAR *)"LCDtask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(KPTask, (signed portCHAR *)"KPTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(STORETask, (signed portCHAR *)"STORETask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(OPTask, (signed portCHAR *)"OPTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRB = 0xFF; PORTB = 0x00;  // Data Lines and OpCode Transfer 
   DDRD = 0xFF; PORTD = 0x00;  // LCD Data Lines
   DDRC = 0xF0; PORTC = 0x0F;  // KEYPAD I/O
   DDRA = 0x00; PORTA = 0x00;  // ADC Input
   
   LCD_init(); // Initialize LCD 
   ADC_init(); // Initialize ADC
   
   
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}