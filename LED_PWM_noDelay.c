#include <avr/io.h>
#include <avr/interrupt.h>

#define OVF_PER_STEP 200

volatile unsigned char light = 0;
volatile unsigned char direction = 1;    // 1=밝아지는 중, 0=어두워지는 중
volatile unsigned char pwm_counter = 0; // 소프트웨어 PWM 듀티비 카운터
volatile unsigned char ovf_count = 0;   // 밝기 변화 속도 조절용 카운터

ISR(TIMER1_OVF_vect) {
	// pwm_counter가 light보다 작은 구간 → ON
	// pwm_counter가 light 이상인 구간 → OFF
	pwm_counter++;
	if (pwm_counter >= 255) pwm_counter = 0;

	if (pwm_counter < light) PORTA = 0xFF;  // LED ON
	else                     PORTA = 0x00;  // LED OFF

	// 오버플로우가 78번 쌓여야(≈10ms) 밝기 한 단계 변경
	ovf_count++;
	if (ovf_count >= OVF_PER_STEP) {
		ovf_count = 0;

		if (direction) {          // 밝아지는 방향
			light++;
			if (light == 255) direction = 0;
			} else {                  // 어두워지는 방향
			light--;
			if (light == 0) direction = 1;
		}
	}
}

void timer1_init(void) {
	TCCR1A = (1 << WGM10);
	TCCR1B = (1 << WGM12) | (1 << CS10);
	TIMSK |= (1 << TOIE1);
}

int main(void) {
	DDRA = 0xFF;  

	timer1_init();
	sei();      

	while (1) {

	}
}