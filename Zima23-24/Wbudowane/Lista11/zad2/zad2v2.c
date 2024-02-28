#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>


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


void timer1_init() {
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 010  -- prescaler 8
  // ICR1  = 2048
  // częstotliwość 16e6/(8*(1+2048)) ~ 976 HZ
  // wzór: datasheet 20.12.3 str. 164
  ICR1 = 2048;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS11);
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1);
}

// inicjalizacja ADC
void adc_init() {
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

ISR(TIMER1_CAPT_vect){
    TIMSK1 &= ~_BV(ICIE1);
    ADCSRA |= _BV(ADSC);
}

ISR(TIMER1_OVF_vect){
    ADCSRA |= _BV(ADSC);
    TIMSK1 &= ~_BV(TOIE1);
}

FILE uart_file;

int main() {
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit,NULL, _FDEV_SETUP_RW);
  stdout = stderr = &uart_file;
  sei();
  timer1_init();
  adc_init();
  uint8_t counter=0;
  uint16_t ADC_lower,ADC_upper;
  while (1) {
    ADMUX &= ~_BV(MUX0);
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    OCR1A = 2 * v;
    if(counter==100){
      ADMUX |= _BV(MUX0); 
      counter=0;
      TIMSK1 |= _BV(ICIE1);
      while (!(ADCSRA & _BV(ADIF))); 
      ADCSRA |= _BV(ADIF);
      ADC_upper=ADC;

      TIMSK1 |= _BV(TOIE1);
      while (!(ADCSRA & _BV(ADIF))); 
      ADCSRA |= _BV(ADIF);
      ADC_lower=ADC;

      float f1=(float)ADC_upper*5.0 * 1000 / 1024.0,
            f2=(float)ADC_lower*5.0 * 1000 /1024.0;
      printf("Napięcia H=%.4fmV,L=%.4fmV\r",f1,f2);
    }
    counter++;
    _delay_ms(10);
  }
}