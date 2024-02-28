#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdbool.h>

int main(){
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

    uint8_t pds[8] = {PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7};

    DDRD  = 0b11111111;
    while(1){
        PORTD = 0b00000000;

        for(int i = 0; i < 8; i++){
            PORTD |= _BV(pds[i]);
            _delay_ms(75);
            PORTD &= ~_BV(pds[i]);
        }
        for(int i = 6; i > 0; i--){
            PORTD |= _BV(pds[i]);
            _delay_ms(75);
            PORTD &= ~_BV(pds[i]);
        }
    }
}