#ifndef ADC_H
#define ADC_H

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

unsigned short Read_ADC(uint8_t ch)
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN) | (1<< ADPS2);
	ch = ch&0b00000111;
	ADMUX |= ch;
	
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA |= (1<<ADIF);
	
	return ADC;
}



#endif