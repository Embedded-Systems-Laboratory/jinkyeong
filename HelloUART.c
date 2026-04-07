#include <avr/io.h>

void USART0_init()
{
	DDRE = 0b11111110;    // Rx(입력 0), Tx(출력, 1)

	// UCSR0A = 0x00;        /* 0번째 비트, 즉 MPCMn 를 0으로 세트 (USARTn을 멀티 프로세서 통신모드로 설정) */

	UCSR0B = 0b00011000;  // Rx, Tx enable 3번째, 4번째 비트 세트
	// 2번 비트 UCSZ02 = 0으로 세트

	UCSR0C = 0b00000110;  // 비동기 방식, No Parity bit, 1 Stop bit
	// 1번째, 2번째 비트 세트, 즉 UCSZ0(1:0) = “11”로 세트
	// 데이터 비트 수를 8비트로 결정

	UBRR0H = 0x00;
	UBRR0L = 0x08;        // Baud rate 설정 7.3728 MHz -> 115200 bps
}

void putch(unsigned char data){
	while((UCSR0A & 0x20) == 0);  // 전송준비가 될 때까지 대기
	UDR0 = data;
	UCSR0A |= 0x20;               /* UCSR0A 레지스터 중에서 5번째 비트(UDREn)를 세트하여 UDRE0를 클리어 함. */
}

void main()
{
	unsigned char text[] = "Hello! World!! \r\n";
	unsigned char i = 0;

	USART0_init();

	while(text[i] != '\0')
	putch(text[i++]);

	return;
}