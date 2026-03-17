#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

ISR(INT4_vect)
{
	PORTA = 0xff;
	_delay_ms(1);
}
ISR(INT5_vect)
{
	PORTA = 0x00;
	_delay_ms(1);
}
int main(void)
{
	DDRA=0xff;
	DDRE=0x00;
	EICRB=0x0a;
	EIMSK=0x30;
	sei();
	while(1);
}
