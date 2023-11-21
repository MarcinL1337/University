#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define no_of_measurements 10

float res;

float with_adc[no_of_measurements], without_adc[no_of_measurements];

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

// inicjalizacja ADC
void adc_init()
{
  ADMUX = 0b01001110;
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
  // ADCSRA |= _BV(ADIE); // ADC Conversion Complete Interrupt is activated.
}

ISR(ADC_vect){
  ADCSRA |= _BV(ADSC); // wykonaj konwersję
  while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
  ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
  uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
  res = 1.1 * 1024 / v;
  // printf("Odczytano: %fV\r\n", res);
}

FILE uart_file;

int main()
{
  // SMCR = _BV(SM0) | _BV(SE); //sleep mode ADC + usypianie komendą sleep()
  set_sleep_mode(SLEEP_MODE_ADC);
  uart_init();
  
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  
  adc_init();
  sei();
  while(1) {
    printf("Pomiar normalny:\r\n");
    ADCSRA &= ~_BV(ADIE);
    for(int i = 0; i < no_of_measurements; i++){  
      ADCSRA |= _BV(ADSC); // wykonaj konwersję
      while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
      ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
      uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
      float calculation = 1.1 * 1024 / v;
      without_adc[i] = calculation;
      printf("Odczytano: %fV\r\n", calculation);
      _delay_ms(200);
    }
    printf("Pomiar z ADC Noise Reduction:\r\n");
    ADCSRA |= _BV(ADIE);
    for(int i = 0; i < no_of_measurements; i++){
      sleep_mode();
      with_adc[i] = res;
      printf("\rOdczytano: %fV\r\n", res);
      _delay_ms(200);
    }

    float avg1 = 0.0, avg2 = 0.0;
    for(int i = 0; i < no_of_measurements; i++){
      avg1 += with_adc[i];
      avg2 += without_adc[i];
    }
    avg1 /= no_of_measurements;
    avg2 /= no_of_measurements;

    float war1 = 0.0, war2 = 0.0;
    for(int i = 0; i < no_of_measurements; i++){
      war1 += (with_adc[i] - avg1) * (with_adc[i] - avg1);
      war2 += (without_adc[i] - avg2) * (without_adc[i] - avg2);
    }

    war1 /= no_of_measurements;
    war2 /= no_of_measurements;

    printf("Wariancja dla pomiaru bez ADC Noise Reduction: %f\n\r", war2);
    printf("Wariancja dla pomiaru z ADC Noise Reduction: %f\n\r", war1);

  }
}