#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BUZZER PB4

int main(void)
{
	DDRB |= (1 << BUZZER);

	while (1)
	{
		PORTB |=  (1 << BUZZER);  /* ON  */
		_delay_ms(1);              /* 1ms */
		PORTB &= ~(1 << BUZZER);  /* OFF */
		_delay_ms(1);              /* 1ms */
		/* 주기 2ms = 500Hz */
	}
}