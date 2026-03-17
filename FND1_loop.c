#include <avr/io.h>
#include <util/delay.h>

unsigned char digit[10] = {0x3f, 0x06,
	0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07,
0x7f, 0x67};

int main (void)
{  int i = 0;
	DDRC= 0xff;
	DDRG = 0x0f;
	PORTG = 0x01;
	while(1){
		for (i = 0; i < 10; i ++)  {
			PORTC = digit [i];
			_delay_ms(1000);
		}
	}
	
	return 0;
}