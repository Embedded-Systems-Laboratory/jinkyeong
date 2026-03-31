#include <avr/io.h>
#include <avr/interrupt.h>

#define BUZZER PB4

#define OCR_480HZ   2082
#define OCR_320HZ   3124

#define STATE_480HZ   0
#define STATE_320HZ   1
#define STATE_SILENCE 2

#define TOGGLES_480HZ  24   // 12번 재생 × 2 (토글)
#define TOGGLES_320HZ  16   //  8번 재생 × 2 (토글)
#define PATTERN_REPEAT 20   // 패턴 전체 반복 횟수

// 2초 묵음 = 2,000,000us
// 반주기(480Hz 기준) = 1,000,000 / 480 ≈ 2083us
// 2,000,000 / 2083 ≈ 960번 인터럽트
#define SILENCE_COUNT  960

volatile unsigned char state         = STATE_480HZ;
volatile unsigned int  toggle_count  = 0;
volatile unsigned char pattern_count = 0;
volatile unsigned int  silence_count = 0; 

ISR(TIMER1_COMPA_vect) {

	if (state == STATE_SILENCE) {
		silence_count++;
		if (silence_count >= SILENCE_COUNT) {
			silence_count = 0;
			state = STATE_480HZ;
			OCR1A = OCR_480HZ;
		}
		return;  
	}

	// ── 부저 토글 (재생 구간) ──
	PORTB ^= (1 << BUZZER);
	toggle_count++;

	if (state == STATE_480HZ) {
		if (toggle_count >= TOGGLES_480HZ) {
			toggle_count = 0;
			state = STATE_320HZ;
			OCR1A = OCR_320HZ;
		}

		} else if (state == STATE_320HZ) {
		if (toggle_count >= TOGGLES_320HZ) {
			toggle_count = 0;
			pattern_count++;

			if (pattern_count >= PATTERN_REPEAT) {
				// 20번 반복 완료 → 묵음 진입
				pattern_count = 0;
				state = STATE_SILENCE;
				PORTB &= ~(1 << BUZZER);  // 부저 확실히 OFF
				OCR1A = OCR_480HZ;        // 묵음 카운팅 기준 주파수 유지
				} else {
				// 아직 반복 중 → 480Hz로 복귀
				state = STATE_480HZ;
				OCR1A = OCR_480HZ;
			}
		}
	}
}

void buzzer_init(void) {
	DDRB |= (1 << BUZZER);
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS11);  // CTC 모드, 프리스케일러 ×8
	OCR1A  = OCR_480HZ;
	TIMSK |= (1 << OCIE1A);               // OCR1A 비교일치 인터럽트만 활성화
}

int main(void) {
	buzzer_init();
	sei();
	while (1) {
	}
}