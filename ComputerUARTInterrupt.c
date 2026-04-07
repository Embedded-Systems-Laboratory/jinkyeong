#include <avr/io.h>
#include <avr/interrupt.h>

void USART0_init() {
	UCSR0B = 0b00011000;
	UCSR0C = 0b00000110;
	UBRR0H = 0x00;
	UBRR0L = 0x08;
	UCSR0B |= 0x80;  // RX complete 인터럽트 On
	sei();           // 전역 인터럽트 enable
}

void putch(unsigned char data) {
	while((UCSR0A & 0x20) == 0);
	UDR0 = data;
}

ISR(USART0_RX_vect) {
	unsigned char Data;
	Data = UDR0;   // 수신 데이터 읽기
	putch(Data);   // 받은 거 그대로 송신 (에코)
}

int main() {
	unsigned char text[] = "\r\n Welcome! This is an UART EXAMPLE\r\n USART 0 Test Program.\r\n";
	unsigned char i = 0;
	USART0_init();
	while(text[i] != '\0')
	putch(text[i++]);
	while(1);  // 인터럽트 대기
	return 0;
}