#include <avr/io.h>
#include <util/delay.h>

unsigned char digit[10] = {0x3f, 0x06,
	0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f,
0x67};

unsigned char fnd_sel[4] = {0x08, 0x04,
0x02, 0x01};

int main (void)
{
	int i = 0;
	DDRC= 0xff;
	DDRG = 0x0f;
	while (1)
	{
		for (i = 0; i < 4; i ++)  {
			PORTC = digit [i+1];
			PORTG = fnd_sel[i];
			_delay_ms(10);
		}
	}
	return 0;
}

// 4개의 FND를 한 바퀴 도는 데 걸리는 시간(각 자릿수 딜레이 × 4)이 33ms보다 훨씬 짧아야 하므로, 
// 안전하게 1/3 수준인 10ms 이내로 전체 루프를 끝내기 위해 