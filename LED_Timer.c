#include <avr/io.h>
#include <avr/interrupt.h>
#define LED8 PORTA

uint16_t  cnt_overflow = 0;

ISR(TIMER0_OVF_vect)
{
	cli();
	if (cnt_overflow == 1000 )
	{
		cnt_overflow = 0;
	LED8 = ~LED8; }
	else{
		cnt_overflow++;
		TCNT0 = 5;
		sei();
	}
}
int main(void)
{
	DDRA = 0xFF;
	LED8 = 0x00;
	TCCR0 = 0b00000100;
	TIMSK |= 1 << TOIE0;
	TIFR |=1 << TOV0;
	TCNT0 = 5;
	sei();
	while(1);
}