#include <avr/io.h>
#include <util/delay.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

void timer1_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 101  -- prescaler 1024
  // ICR1  = 15624
  // częstotliwość 16e6/(1024*(1+15624)) = 1 Hz
  // wzór: datasheet 20.12.3 str. 164
  // ICR1 = 15624;
  OCR1A = 421; // 16000000 / 37900 = 1 + 421
  TCCR1A = _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10) | _BV(ICES1);
  // ^ Fast PWM Mode, top = OCR1A. ICES1 -> wartość licznika kopiowana do ICR1 przy wzroście sygnału na pinie
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB2);
}

// ICP1 zczytuje wartość licznika jak przychodzi impuls z detektora i zapisuje do ICR1
// Czujnik jak coś wykryje to puszcza sygnał do D8 (ICP1). ICP1 to port zegara i jak przychodzi sygnał, to
// odpowiednio ustawiony (_BV(ICES1)), zapisuje swoją aktualną wartość do ICR1, więc jak ICR1 zmienia wartość, na inną
// to znaczy, że coś wykrył.

int main()
{
  // uruchom licznik
  timer1_init();
  uint16_t aux = ICR1;
  OCR1B = 211;
  LED_DDR = _BV(LED);
  while(1){
    for(uint16_t i = 0; i < 6; i++){
      LED_PORT &= ~_BV(LED);
      if(aux != ICR1){ // Detektor coś wykrył (zmieniła się wartość)
        LED_PORT |= _BV(LED);
        aux = ICR1;
      }
      LED_DDR |= _BV(PB2);
      _delay_us(600);
      LED_DDR &= ~_BV(PB2);
      _delay_us(600);
    }
    _delay_ms(100);
  }
}