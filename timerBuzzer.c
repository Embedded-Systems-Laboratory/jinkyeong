#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1 << PB4);
}

static void buzzer_on(unsigned int ocr)
{
	OCR1A  = ocr;
	TIMSK |= (1 << OCIE1A);
}

static void buzzer_off(void)
{
	TIMSK &= ~(1 << OCIE1A);
	PORTB &= ~(1 << PB4);
}

int main(void)
{
	DDRB  |= (1 << PB4);
	DDRA   = 0xFF;
	DDRE   = 0x00;
	PORTE  = 0xFF;

	TCCR1A = 0x00;
	TCCR1B = (1 << WGM12) | (1 << CS10);
	TCNT1  = 0;

	sei();

	while(1)
	{
		unsigned char btn = ~PINE;
		PORTA = ~PINE;

		if      (btn & (1 << PE4)) buzzer_on(15267);  /* SW1 → 도  523Hz */
		else if (btn & (1 << PE5)) buzzer_on(12119);  /* SW2 → 미  659Hz */
		else                       buzzer_off();
	}
}