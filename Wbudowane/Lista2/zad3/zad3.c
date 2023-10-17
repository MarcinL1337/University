#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdbool.h>

#define BTN1 PB2
#define BTN2 PB3
#define BTN3 PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

int BinaryToGray(int num){
    return num ^ (num >> 1); 
}

int main(){
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    int max_gray_num = 256;
    DDRD  = 0b11111111;
    PORTD = 0b00000000;
    BTN_PORT |= _BV(BTN1);
    BTN_PORT |= _BV(BTN2);
    BTN_PORT |= _BV(BTN3);

    int cur_num = 0;

    while(1){
        if(!(BTN_PIN & _BV(BTN1))){
            cur_num = 0;
            _delay_ms(150);
        }
        if(!(BTN_PIN & _BV(BTN2))){
            cur_num = (cur_num + 1) % max_gray_num;
            _delay_ms(150);
        }
        if(!(BTN_PIN & _BV(BTN3))){
            if(cur_num == 0) cur_num = 255;
            else cur_num = (cur_num - 1) % max_gray_num;
            _delay_ms(150);
        }
        PORTD = BinaryToGray(cur_num);
        
    }
}