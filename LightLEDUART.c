#include <avr/io.h>
#include <avr/interrupt.h>
#define CDS_VALUE 950
#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE (F_CPU / 16 / BAUD - 1)

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
	value = (adc_high << 8) | adc_low;
	return value;
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

int main()
{
	unsigned short value;
	DDRA = 0xff;
	init_adc();
	init_uart();
	init_timer();
	sei();

	while (1)
	{
		if (flag == 1)  // 1초마다만 실행!
		{
			flag = 0;
			value = read_adc();

			uart_print("ADC Value: ");
			uart_print_number(value);
			uart_print("\r\n");

			if (value >= CDS_VALUE)
			PORTA = 0xff;   // ON
			else
			PORTA = 0x00;   // OFF
		}
	}
}