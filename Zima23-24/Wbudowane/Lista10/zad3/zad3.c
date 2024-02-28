#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define HEATER_PIN    PB5
#define HEATER_PORT   PORTB
#define HEATER_DDR    DDRB	

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

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0) | _BV(REFS1); // referencja 1.1
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // ADC on
  ADCSRA |= _BV(ADATE) | _BV(ADIE); // Auto-trigger + ADC interrupt enabled

  ADCSRB |= _BV(ADTS1) | _BV(ADTS0); // start adc conversion when timer0 interrupt occurs
}

void timer_init() {
    TCCR0A |= (1 << WGM01);
    // Prescaler 64 
    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 25000; // (16MHz / 64 )* 0.1s)
    // Compare match for timer0
    TIMSK0 |= (1 << OCIE0A);
}

volatile float temp = 30.0;
const float t_h = 1.0;

volatile float adc_val;
volatile float current_temp;
volatile float voltage;

ISR(TIMER0_COMPA_vect) {
}

ISR(ADC_vect) {
  adc_val = ADC; // weź zmierzoną wartość (0..1023)
    
  voltage = (adc_val * 1.1) / 1024.0; // ADC to V
  current_temp = (voltage - 0.5) / 0.01; // V to Celsius

  if (current_temp >= temp + t_h) {
    PORTB &= ~_BV(HEATER_PIN);	
  } else if (current_temp <= temp - t_h) {
    PORTB |= _BV(HEATER_PIN);
  }
}

int main()
{
  sei();

  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  
  // zainicjalizuj ADC
  adc_init();
  timer_init();
  
  // ustaw pin grzałki na output
  HEATER_DDR |= _BV(HEATER_PIN);
  char command[10];
  
  while(1) {
    scanf("%s", command);
    if (strcmp(command, "read") == 0)
    {
      printf("Recently measured temperature is:  %.2fC\r\n", current_temp);
    }
    else if (strcmp(command, "set") == 0)
    {
      scanf("%f", &temp);
      printf("You've set the temperature to: %.2fC\r\n", temp);
    }
    else
      printf("Invalid command. Use one of the following: set/read\r\n");
  }
}
