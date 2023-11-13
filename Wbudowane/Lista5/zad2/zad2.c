#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <math.h>

#define BTN PD2
#define BTN_PIN PIND
#define BTN_PORT PORTD

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define R2 2200

float R1;

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

void io_init()
{
  // ustaw pull-up na PD2 i PD3 (INT0 i INT1)
  BTN_PORT |= _BV(BTN);
  EICRA |= _BV(ISC00) | _BV(ISC01); // INT0 wyzwala przerwania zboczem narastającym
  EIMSK |= _BV(INT0); // Odmaskowanie przerwania dla INT0
}

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128

  /*
    When this bit is written to one, Auto Triggering of the ADC is enabled. The ADC will start a conversion on a positive
    edge of the selected trigger signal. The trigger source is selected by setting the ADC Trigger Select bits, ADTS in
    ADCSRB.
  */
  ADCSRA |= _BV(ADATE);
  ADCSRA |= _BV(ADEN); // włącz ADC
  ADCSRA |= _BV(ADIE); // ADC Conversion Complete Interrupt is activated.

  // If ADATE in ADCSRA is written to one, the value of these bits selects which source will trigger an ADC conversion.
  ADCSRB |= _BV(ADTS1); // External Interrupt Request 0
  
}

ISR(ADC_vect){
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    R1 = 1024.0 * R2 / v - R2;
}

ISR(INT0_vect){}

FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  io_init();
  adc_init();
  sei();
  // mierz napięcie
  while(1) {
    printf("R1 = %f\r\n", R1);
    _delay_ms(2000);
  }
}