#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char Light = 0;
	unsigned char i;

	DDRA = 0xFF;

	while(1)
	{
		/* 점점 밝아짐 */
		for (Light = 0; Light < 255; Light++)
		{
			for (i = 0; i < 255; i++)
			{
				if (i < Light) PORTA = 0xFF;  /* ON */
				else           PORTA = 0x00;  /* OFF */
				_delay_us(4);   /* 상수! */
			}
		}

		/* 점점 어두워짐 */
		for (Light = 255; Light > 0; Light--)
		{
			for (i = 0; i < 255; i++)
			{
				if (i < Light) PORTA = 0xFF;
				else           PORTA = 0x00;
				_delay_us(4);
			}
		}
	}
}