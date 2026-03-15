/*
 * main.c
 *
 *  Author: nojk2
 */ 

#include <xc.h>
#include <util/delay.h>


int main(void)
{
	DDRA = 0xFF;
	PORTA = 0x00;
    while(1)
    {
        PORTA = 0x00;
		for(int i=0; i<8; i++){
			PORTA = 0x01 << i;
			_delay_ms(100);
		}
    }
}