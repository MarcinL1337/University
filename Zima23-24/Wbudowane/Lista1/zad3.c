#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

#define up PD0
#define right_up PD1
#define right_down PD2
#define down PD3
#define left_down PD4
#define left_up PD5
#define middle PD6
#define dot PD7

#define reset() (PORTD = 0b01111111)
#define light_led(which) (PORTD &= ~_BV(which))

/*
PD0 - A
PD1 - B
PD2 - C
PD3 - D
PD4 - E
PD5 - F
PD6 - G
PD7 - DOT
*/

int main() {
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

  DDRD = 0b11111111;
  
  reset();
  _delay_ms(1000);
  light_led(up);
  light_led(right_down);
  light_led(right_up);
  light_led(down);
  light_led(left_down);
  light_led(left_up);
  
  _delay_ms(1000);
  reset();
  light_led(right_down);
  light_led(right_up);

  _delay_ms(1000);
  reset();
  light_led(up);
  light_led(middle);
  light_led(right_up);
  light_led(down);
  light_led(left_down);

  _delay_ms(1000);
  reset();
  light_led(up);
  light_led(right_down);
  light_led(right_up);
  light_led(down);
  light_led(middle);

  _delay_ms(1000);
  reset();
  light_led(right_down);
  light_led(right_up);
  light_led(middle);
  light_led(left_up);

  _delay_ms(1000);
  reset();
  light_led(up);
  light_led(right_down);
  light_led(down);
  light_led(left_up);
  light_led(middle);

  _delay_ms(1000);
  reset();
  light_led(up);
  light_led(right_down);
  light_led(down);
  light_led(left_down);
  light_led(left_up);
  light_led(middle);

  _delay_ms(1000);
  reset();
  light_led(up);
  light_led(right_down);
  light_led(right_up);

  _delay_ms(1000);
  reset();
  light_led(up);
  light_led(right_down);
  light_led(right_up);
  light_led(down);
  light_led(left_down);
  light_led(left_up);
  light_led(middle);

  _delay_ms(1000);
  reset();
  light_led(up);
  light_led(right_down);
  light_led(right_up);
  light_led(down);
  light_led(left_up);
  light_led(middle);
} 