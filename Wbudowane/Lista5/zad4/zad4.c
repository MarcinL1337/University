#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include <math.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB
#define TOP 15624
uint16_t sample = 0, x;
uint32_t diff;
int8_t Ovf;

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

void timer1_init() {
  OCR1A = TOP; // 16e6 / 37900 = 1 + 421
  TCCR1A =  _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(ICES1) | _BV(CS12) | _BV(CS10); // preskaler 1024
  // ^ Fast PWM Mode, top = OCR1A. ICES1 -> wartość licznika kopiowana do ICR1 przy wzroście sygnału na pinie
  // odmaskowanie przerwania przepełnienia licznika
  TIMSK1 |= _BV(ICIE1);
  // ustaw pin OC1A (PB1) jako wyjście
  // DDRB |= _BV(PB2);
}

ISR(TIMER1_CAPT_vect) {
  x = ICR1;
  Ovf = x < sample ? 1 : 0;
  diff = (Ovf * 15624) + x - sample;
  sample = x;
}

// ICP1 zczytuje wartość licznika jak przychodzi impuls z detektora i zapisuje do ICR1
// Czujnik jak coś wykryje to puszcza sygnał do D8 (ICP1). ICP1 to port zegara i jak przychodzi sygnał, to
// odpowiednio ustawiony (_BV(ICES1)), zapisuje swoją aktualną wartość do ICR1, więc jak ICR1 zmienia wartość, na inną
// to znaczy, że coś wykrył.

int main(){
  timer1_init();
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  set_sleep_mode(SLEEP_MODE_IDLE);
  sei();

  while(1){
    sleep_mode();
    printf("Measurement: %u \r\n", 15625/diff);
  }
}