#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

const uint8_t digits[10] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
};

int main(){
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    DDRD = 0b11111111;
    DDRC = 0b11111111;
    while(1){
        for(int i = 0; i < 60; i++){
            for(int j = 0; j < 50; j++){
                PORTC = 0b11111101;
                PORTD = ~digits[i%10];
                _delay_ms(10);
                PORTC = 0b11111110;
                PORTD = ~digits[i/10];
                _delay_ms(10);
            }
        }
    }
}