#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE (F_CPU / 16 / BAUD - 1)

// 실측 후 조정 필요!
#define LUX_100  300   // 100 Lux일 때 ADC값 (LED 1개)
#define LUX_1    900   // 1 Lux일 때 ADC값  (LED 8개)

volatile unsigned char flag = 0;

void init_adc()
{
	ADMUX  = 0x00;
	ADCSRA = 0x87;
}

unsigned short read_adc()
{
	unsigned char adc_low, adc_high;
	unsigned short value;
	ADCSRA |= 0x40;
	while ((ADCSRA & 0x10) != 0x10);
	adc_low  = ADCL;
	adc_high = ADCH;
	return (adc_high << 8) | adc_low;
}

void init_uart()
{
	UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
	UBRR0L = (unsigned char)(UBRR_VALUE);
	UCSR0B = 0x08;
	UCSR0C = 0x06;
}

void uart_putchar(char c)
{
	while (!(UCSR0A & 0x20));
	UDR0 = c;
}

void uart_print(const char *str)
{
	while (*str)
	uart_putchar(*str++);
}

void uart_print_number(unsigned short num)
{
	char buf[6];
	buf[5] = '\0';
	buf[4] = '0' + (num % 10); num /= 10;
	buf[3] = '0' + (num % 10); num /= 10;
	buf[2] = '0' + (num % 10); num /= 10;
	buf[1] = '0' + (num % 10); num /= 10;
	buf[0] = '0' + (num % 10);
	uart_print(buf);
}

void init_timer()
{
	TCCR1A = 0x00;
	TCCR1B = 0x0D;
	OCR1A  = 15624;
	TIMSK |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	flag = 1;
}

// ADC값 → LED 개수 결정
unsigned char get_led_pattern(unsigned short value)
{
	// 100 Lux 미만(밝음)이면 LED 없음
	if (value < LUX_100) return 0x00;

	// 1 Lux 이상(완전 어두움)이면 LED 전부
	if (value >= LUX_1)  return 0xFF;

	// 그 사이: 어두워질수록 LED 1개씩 추가
	// LUX_100 ~ LUX_1 구간을 8단계로 나눔
	unsigned short range = LUX_1 - LUX_100;          // 전체 구간
	unsigned short step  = (value - LUX_100) * 8 / range; // 0~7단계

	// step 0 → 0x01 (1개), step 7 → 0xFF (8개)
	unsigned char pattern = 0x00;
	unsigned char i;
	for (i = 0; i <= step; i++)
	pattern |= (1 << i);

	return pattern;
}

int main()
{
	unsigned short value;
	DDRA = 0xFF;
	init_adc();
	init_uart();
	init_timer();
	sei();

	while (1)
	{
		if (flag == 1)
		{
			flag = 0;
			value = read_adc();

			PORTA = get_led_pattern(value);  // 순차적으로 LED ON

			uart_print("ADC Value: ");
			uart_print_number(value);
			uart_print("  LED: ");
			uart_print_number(PORTA);
			uart_print("\r\n");
		}
	}
}