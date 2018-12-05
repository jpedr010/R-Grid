/*
 * Keypad_UnitTest.c
 *
 * Created: 11/20/2018 10:05:03 AM
 * Author : jpedr
 */ 

#include <avr/io.h>
#include "io.c"
#include "keypad.h"

int main(void)
{
	DDRD = 0xFF; PORTD = 0x00; //LCD CONTROL LINES
	DDRC = 0xFF; PORTC = 0x00; //LCD DATA LINES
	DDRA = 0xF0; PORTA = 0x0F; //KEYPAD I/O
	
	unsigned char key = 0x00;
	
	LCD_init();
	
    /* Replace with your application code */
    while (1) 
    {
		key = GetKeypadKey();
		if (key == 'A')
		{
			LCD_DisplayString(1, "A");
		}
		else if (key == 'B')
		{
			LCD_DisplayString(1, "B");
		}
		else if (key == 'C')
		{
			LCD_DisplayString(1, "C");
		}
		else if (key == 'D')
		{
			LCD_DisplayString(1, "D");
		}
		else if (key == '1')
		{
			LCD_DisplayString(1, "1");
		}
		else if (key == '2')
		{
			LCD_DisplayString(1, "2");
		}
		else if (key == '3')
		{
			LCD_DisplayString(1, "3");
		}
		else if (key == '4')
		{
			LCD_DisplayString(1, "4");
		}
		else if (key == '5')
		{
			LCD_DisplayString(1, "5");
		}
		else if (key == '6')
		{
			LCD_DisplayString(1, "6");
		}
		else if (key == '7')
		{
			LCD_DisplayString(1, "7");
		}
		else if (key == '8')
		{
			LCD_DisplayString(1, "8");
		}
		else if (key == '9')
		{
			LCD_DisplayString(1, "9");
		}
		else if (key == '*')
		{
			LCD_DisplayString(1, "*");
		}
		else if (key == '0')
		{
			LCD_DisplayString(1, "0");
		}
		else if (key == '#')
		{
			LCD_DisplayString(1, "#");
		}
    }	
}

