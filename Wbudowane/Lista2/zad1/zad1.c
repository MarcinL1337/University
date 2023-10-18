#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

#define buffer_size 100

bool buffer[buffer_size];
int write_idx = 100, read_idx = 1;

int main(){
    for(int i = 0; i < buffer_size; i++)
        buffer[i] = false;

    BTN_PORT |= _BV(BTN);
    LED_DDR |= _BV(LED);
    
    while(1){
        if(BTN_PIN & _BV(BTN)) buffer[write_idx] = false;
        else                   buffer[write_idx] = true;

        if(buffer[read_idx])   LED_PORT |= _BV(LED);
        else                   LED_PORT &= ~_BV(LED);

        read_idx = (read_idx + 1) % buffer_size;
        write_idx = (write_idx + 1) % buffer_size;
        _delay_ms(10);
    }
}