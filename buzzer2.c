#include <avr/io.h>
#include <util/delay.h>

#define BUZZER PB4

void buzzer_480hz(unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		PORTB |=  (1 << BUZZER);
		_delay_us(1041);   /* 1/480Hz ÷ 2 ≈ 1041us */
		PORTB &= ~(1 << BUZZER);
		_delay_us(1041);
	}
}

void buzzer_320hz(unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		PORTB |=  (1 << BUZZER);
		_delay_us(1562);   /* 1/320Hz ÷ 2 ≈ 1562us */
		PORTB &= ~(1 << BUZZER);
		_delay_us(1562);
	}
}

int main(void)
{
	DDRB |= (1 << BUZZER);

	while (1)
	{
		for (int i = 0; i < 20; i++)
		{
			buzzer_480hz(12);
			buzzer_320hz(8);
		}
		PORTB &= ~(1 << BUZZER);
		_delay_ms(2000);   /* 2초 묵음 */
	}
}