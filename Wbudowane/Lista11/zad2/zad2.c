#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

void uart_init()
{
  UBRR0 = UBRR_VALUE;
  UCSR0A = 0;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_receive(FILE *stream){
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

int uart_transmit(char data, FILE *stream)
{
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
} 

FILE uart_file;


void adc_init(){
  ADMUX  |= _BV(REFS0); 
  // disable input buffers on pins A0 A1
  DIDR0   = _BV(ADC0D) | _BV(ADC1D); 
  // prescaler 128
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
  // enable ADC
  ADCSRA |= _BV(ADEN); 
}

void timer1_init(){
  // Set OC1A/OC1B on Compare Match when up counting. Clear OC1A/OC1B on Compare Match when downcounting.
  TCCR1A |= _BV(COM1A1) | _BV(COM1A0);
  // PWM, Phase and Frequency Correct, ICR1 TOP | prescaler 8
  TCCR1B |= _BV(WGM13) | _BV(CS11);
  ICR1 = 1024;
  DDRB |= _BV(PB1);
}

ISR(TIMER1_CAPT_vect){
  // disable Timer/Counter1 Input Capture interrupt  
  TIMSK1 &= ~_BV(ICIE1);
  // start ADC conversion
  ADCSRA |= _BV(ADSC);
}

ISR(TIMER1_OVF_vect){
  // start ADC conversion
  ADCSRA |= _BV(ADSC);
  // disable Timer/Counter1 Overflow interrupt
  TIMSK1 &= ~_BV(TOIE1);
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  // zainicjalizuj ADC
  adc_init();
  timer1_init();
  // przerwania
  sei();
  uint8_t counter=0;
  uint16_t ADC_lower,ADC_upper;
  while(1) { 
    // set A0 as input
    ADMUX &= ~_BV(MUX0);  
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    OCR1A = 2*ADC;

    if(counter==100){
      // set A1 as input
      ADMUX |= _BV(MUX0); 
      counter=0;
      // enable Timer/Counter1 Input Capture interrupt
      TIMSK1 |= _BV(ICIE1);
      while (!(ADCSRA & _BV(ADIF))); 
      ADCSRA |= _BV(ADIF);
      ADC_upper=ADC;

      // enable Timer/Counter1 Overflow interrupt
      TIMSK1 |= _BV(TOIE1);
      while (!(ADCSRA & _BV(ADIF))); 
      ADCSRA |= _BV(ADIF);
      ADC_lower=ADC;

      float f1=(float)ADC_upper*5.0 * 1000 / 1024.0,
            f2=(float)ADC_lower*5.0 * 1000 / 1024.0;
      printf("Napięcia H=%.4fmV,L=%.4fmV\r",f1,f2);
    }

    counter++;
    _delay_ms(10);
  }

}