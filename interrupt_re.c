#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define OCR_G4  2549   /* 392Hz : 솔 */
#define OCR_A4  2271   /* 440Hz : 라 */
#define OCR_C5  1909   /* 523Hz : 도 */
#define OCR_B4  2024   /* 494Hz : 시 */

static const uint16_t melody[6] = {
	OCR_G4, OCR_G4, OCR_A4, OCR_G4, OCR_C5, OCR_B4
};

#define MELODY_LEN   6
/* 음표 하나당 약 500ms = 401틱, gap 약 50ms = 40틱 */
#define NOTE_TICKS     401  /* 401틱 = 500ms */
#define NOTE_GAP_TICKS  40  /* 40틱 = 50ms */

static const uint8_t digit[10] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66,
	0x6D, 0x7C, 0x07, 0x7F, 0x67
};

typedef enum { STATE_COUNTING, STATE_PAUSED, STATE_BUZZING } State;

volatile State    g_state      = STATE_COUNTING;
volatile uint8_t  g_count      = 10;

// g_tick를 16비트로 바꿔줌
volatile uint16_t  g_tick       = 0;

volatile uint8_t  g_note_idx   = 0;
volatile uint16_t g_note_tick    = 0;

volatile uint8_t g_btn_event = 0;  /* INT4 ISR이 세우는 플래그 */

static uint8_t g_fnd_digit = 0;

static void fnd_show(uint8_t n)
{
	if (n >= 10)
	{
		if (g_fnd_digit == 0) { PORTC = digit[n % 10]; PORTG = 0x01; }
		else                  { PORTC = digit[n / 10]; PORTG = 0x02; }
		g_fnd_digit ^= 1;
	}
	else
	{
		PORTC = digit[n];
		PORTG = 0x01;
	}
}

static void buzzer_start(uint16_t ocr)
{
	TCNT1  = 0;
	OCR1A  = ocr;
	TIFR  |= (1 << OCF1A);
	TCCR1B = (1 << WGM12) | (1 << CS11);
}

static void buzzer_stop(void)
{
	TCCR1B = (1 << WGM12);
	PORTB &= ~(1 << PB4);
}

static void timer0_init(void)
{
	// 프리스케일러 128분주였음
	TCCR0  = (1 << WGM01) | (1 << CS02) | (1 << CS00);
	
	OCR0   = 155;
	TIMSK |= (1 << OCIE0);
}

static void timer1_init(void)
{
	TCCR1A = 0x00;
	TCCR1B = (1 << WGM12);
	OCR1A  = OCR_G4;
	TIMSK |= (1 << OCIE1A);
}

static void int4_init(void)
{
	DDRE  &= ~(1 << PE4);
	EICRB  = (1 << ISC41);
	EIMSK |= (1 << INT4);
}

ISR(TIMER0_COMP_vect)
{
	fnd_show(g_count);   /* 타이머가 타이밍을 보장 — main에 _delay_ms 불필요 */
	
	if (g_state == STATE_COUNTING)
	{
		g_tick++;
		
		// 다시 계산했을 때 1초는 약 801번 클럭이 움직여야 함
		if (g_tick >= 801)
		{
			g_tick = 0;
			if (g_count > 0) g_count--;

			if (g_count == 0)
			{
				g_state    = STATE_BUZZING;
				g_note_idx = 0;
				g_note_tick  = 0;
				buzzer_start(melody[0]);
			}
		}
	}
	else if (g_state == STATE_BUZZING)
	{
		g_note_tick += 1;

		if (g_note_tick == (NOTE_TICKS - NOTE_GAP_TICKS))
		buzzer_stop();

		if (g_note_tick >= NOTE_TICKS)
		{
			g_note_tick = 0;
			g_note_idx++;

			if (g_note_idx >= MELODY_LEN)
			{
				buzzer_stop();
				g_state = STATE_PAUSED;
			}
			else
			{
				buzzer_start(melody[g_note_idx]);
			}
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1 << PB4);
}

ISR(INT4_vect)
{
	/* 짧은 디바운스만 하고 바로 빠져나옴 */
	for (volatile uint16_t i = 0; i < 8000; i++);
	
	/* 아직도 LOW면 진짜 눌림 — 플래그만 세우고 끝 */
	if (!(PINE & (1 << PE4)))
	g_btn_event = 1;
}

int main(void)
{
	DDRC  = 0xFF;
	DDRG |= 0x0F;
	DDRB |= (1 << PB4);
	DDRE &= ~(1 << PE4);

	buzzer_stop();

	timer0_init();
	timer1_init();
	int4_init();

	sei();

	while (1)
	{
		if (g_btn_event)
		{
			g_btn_event = 0;  /* 플래그 먼저 내려야 다음 이벤트를 놓치지 않음 */
			
			switch (g_state)
			{
				case STATE_COUNTING:
				g_state = STATE_PAUSED;
				break;
				case STATE_PAUSED:
				if (g_count == 0)
				{
					g_state     = STATE_BUZZING;
					g_note_tick = 0;
					buzzer_start(melody[g_note_idx]);
				}
				else
				{
					g_state = STATE_COUNTING;
				}
				break;
				case STATE_BUZZING:
				buzzer_stop();
				g_state = STATE_PAUSED;
				break;
			}
		}
	}
}