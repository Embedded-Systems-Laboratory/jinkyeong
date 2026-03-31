#include <avr/io.h>
#include <avr/interrupt.h>

#define BUZZER  PB4
/* OCR1A = 16,000,000 / 8 / 1046.6 / 2 - 1 ≈ 955 */
#define OCR_DO  955

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1 << BUZZER);   /* 토글 → '도' 음 발생 */
}

int main(void)
{
	DDRB  |= (1 << BUZZER);
	PORTB &= ~(1 << BUZZER);

	TCCR1A = 0x00;
	TCCR1B = (1 << WGM12) | (1 << CS11);  /* CTC + 8분주 */
	OCR1A  = OCR_DO;
	TIMSK |= (1 << OCIE1A);

	sei();

	while (1);   /* CPU는 놀고, 타이머가 알아서 소리 냄 */
}