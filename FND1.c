#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0xff;     
	DDRG = 0x0f;     

    // 제일 왼쪽에 7
	PORTG = 0x08;    
	PORTC = 0x27;   
    
    // 제일 오른쪽에 0 
	PORTG = 0x01;    
	PORTC = 0x3f;   

	while (1);       
}