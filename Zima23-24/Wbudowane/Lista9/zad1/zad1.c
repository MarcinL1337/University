#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdbool.h>

#define LED PB2
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PA7
#define BTN_PIN PINA
#define BTN_PORT PORTA

#define buffer_size 100

bool buffer[buffer_size];
int write_idx = 100, read_idx = 1;

void io_init(){
  // pull-up
  BTN_PORT |= _BV(BTN);
  LED_DDR |= _BV(LED);
}

void timer1_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 101  -- prescaler 1024
  // ICR1  = 15624
  // częstotliwość 16e6/(1024*(1+155)) = 100 Hz -> 10ms
  // wzór: datasheet 20.12.3 str. 164
  ICR1 = 10;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS12) | _BV(CS10);
  TIMSK1 |= _BV(TOIE1); // Overflow Interrupt Enable
}

// procedura obsługi przerwania przepełnienia licznika
ISR(TIM1_OVF_vect) {
  if(BTN_PIN & _BV(BTN)) buffer[write_idx] = false;
  else                   buffer[write_idx] = true;

  if(buffer[read_idx])   LED_PORT |= _BV(LED);
  else                   LED_PORT &= ~_BV(LED);

  read_idx = (read_idx + 1) % buffer_size;
  write_idx = (write_idx + 1) % buffer_size;
}

int main() {
  for(int i = 0; i < buffer_size; i++)
    buffer[i] = false;
    
  // zainicjalizuj wejścia/wyjścia
  io_init();
  timer1_init();
  // ustaw tryb uśpienia na tryb bezczynności
  set_sleep_mode(SLEEP_MODE_IDLE);
  // odmaskuj przerwania
  sei();
  // program testowy
  while(1) {
      sleep_mode();
  }
}