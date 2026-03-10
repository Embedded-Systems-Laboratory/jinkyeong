#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT4_vect) {                
    _delay_ms(30);              

    
    if (!(PINE & (1 << PE4))) {
        PORTA ^= 0xFF;          

        while (!(PINE & (1 << PE4))) { }
        _delay_ms(30);          
    }
}

int main(void) {
    
    DDRA = 0xFF;
    PORTA = 0x00;               

    DDRE &= ~(1 << PE4);
    PORTE |= (1 << PE4);        

    EICRB |= (1 << ISC41);
    EICRB &= ~(1 << ISC40);

    EIFR  |= (1 << INTF4);     
    EIMSK |= (1 << INT4);       

    sei();                      

    while (1) {
        
    }
}