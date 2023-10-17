#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

#define A 0b11111110
#define B 0b11111101
#define C 0b11111011
#define D 0b11110111
#define E 0b11101111
#define F 0b11011111
#define G 0b10111111
#define O 0b01111111

const uint8_t digits[10] = {
    A & B & C & D & E & F,
    B & C,
    A & B & G & E & D,
    A & B & G & C & D,
    F & G & B & C,
    A & F & G & C & D,
    A & F & G & C & D & E,
    A & B & C,
    A & B & C & D & E & F & G,
    A & B & G & F & C & D
};

int main(){
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    DDRD = 0b11111111;
    DDRC = 0b11111111;
    while(1){
        for(int i = 0; i < 60; i++){
            for(int j = 0; j < 50; j++){
                PORTC = 0b11111101;
                PORTD = (digits[i%10]);
                _delay_ms(10);
                PORTC = 0b11111110;
                PORTD = (digits[i/10]);
                _delay_ms(10);
            }
        }
    }
}