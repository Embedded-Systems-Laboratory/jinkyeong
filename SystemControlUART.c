#include <avr/io.h>
#include <avr/interrupt.h>

void USART0_init() {
	UCSR0B = 0b00011000;
	UCSR0C = 0b00000110;
	UBRR0H = 0x00;
	UBRR0L = 0x08;
	UCSR0B |= 0x80;  // RX 인터럽트 On
	sei();
}

void putch(unsigned char data) {
	while((UCSR0A & 0x20) == 0);
	UDR0 = data;
}

void putstr(char *str) {
	while(*str != '\0')
	putch(*str++);
}

// PC → LED 제어 (RX 인터럽트)
ISR(USART0_RX_vect) {
	unsigned char data = UDR0;
	putch(data); 
	if(data == '1')
	PORTA = 0xFF;  // LED 전부 ON
	else if(data == '0')
	PORTA = 0x00;  // LED 전부 OFF
}

// 버튼 인터럽트 (SW1=PE4=INT4, SW2=PE5=INT5)
ISR(INT4_vect) {
	putstr("Button 1 pressed!\r\n");
}

ISR(INT5_vect) {
	putstr("Button 2 pressed!\r\n");
}

int main() {
	// LED 출력 설정
	DDRA = 0xFF;
	PORTA = 0x00;

	// 스위치 입력 설정 (PE4, PE5)
	DDRE &= ~(1<<PE4) & ~(1<<PE5);

	// 외부 인터럽트 INT4, INT5 설정 (하강 엣지)
	EICRB = 0b00001010;  // INT4, INT5 하강 엣지
	EIMSK = 0b00110000;  // INT4, INT5 enable

	USART0_init();

	while(1);
	return 0;
}