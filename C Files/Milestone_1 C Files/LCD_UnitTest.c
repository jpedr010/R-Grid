#include <avr/io.h>
#include "io.c"

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // LCD Control Lines
	DDRD = 0xFF; PORTD = 0x00; // LCD Data Lines
	
	// Initializes the LCD display
	LCD_init();
	
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_DisplayString(1, "Hello World");
	
	while(1) {continue;}
}
