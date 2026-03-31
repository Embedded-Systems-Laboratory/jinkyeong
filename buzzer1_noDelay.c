#include <avr/io.h>

#define BUZZER PB4

void buzzer_init(void) {
	DDRB |= (1 << BUZZER);

	// TCCR0 설정
	// COM00=1 → CTC 모드에서 OCR0 도달 시 OC0 핀 토글 (자동 50% 듀티비)
	// WGM01=1 → CTC 모드 선택
	// CS01=1, CS00=1 → 프리스케일러 ×64
	TCCR0 = (1 << COM00) | (1 << WGM01) | (1 << CS01) | (1 << CS00);

	// 500Hz를 위한 OCR0 값
	// (16,000,000 / (2 × 64 × 500)) - 1 = 249
	OCR0 = 249;
}

int main(void) {
	buzzer_init();

	while (1) {
	}
}