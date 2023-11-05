#include <avr/io.h>

#define LED_R PB2
#define LED_G PB1
#define LED_B PB3
#define LED_DDR DDRB
#define LED_PORT PORTB

float tab[] = {
    
};

void timer1_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 101  -- prescaler 1024
  // ICR1  = 15624
  // częstotliwość 16e6/(1024*(1+15624)) = 1 Hz
  // wzór: datasheet 20.12.3 str. 164
  ICR1 = 300;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10) | _BV(CS12);
  // ustaw pin OC1A (PB1) jako wyjście
  LED_DDR |= _BV(LED_G);
}

int main()
{
  // uruchom licznik
  timer1_init();
  // ustaw wypełnienie 50%
//   OCR1A = ICR1/2;
  while(1){
    for(int i = 0; i < 629; i++){
        OCR1A = tab[i];
    }
  }
}