#include <avr/io.h>
#define CDS_VALUE 900  // 실측 후 10 Lux 값으로 조정!

void init_adc()
{
	ADMUX = 0x00;
	ADCSRA = 0x87;
}

unsigned short read_adc()
{
	unsigned char adc_low, adc_high;
	unsigned short value;
	ADCSRA |= 0x40;
	while ((ADCSRA & 0x10) != 0x10);
	adc_low  = ADCL;
	adc_high = ADCH;
	value = (adc_high << 8) | adc_low;
	return value;
}

int main()
{
	unsigned short value;
	DDRA = 0xff;
	init_adc();
	while (1)
	{
		value = read_adc();
		if (value > CDS_VALUE)  // 어두우면 (ADC값 높음)
		PORTA = 0xff;       // 가로등 ON
		else                    // 밝으면
		PORTA = 0x00;       // 가로등 OFF
	}
}