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
  ADCSRA |= _BV(ADEN); // włącz ADC
  ADCSRA |= _BV(ADATE) | _BV(ADIE); // Włącz auto-trigger(automatyczna konwersja) + ADC Conversion Complete Interrupt

  ADCSRB |= _BV(ADTS1) | _BV(ADTS0); // Przerwanie Timera 0 triggeruje konwersję ADC
}

void timer_init() {
    // CTC
    TCCR0A |= (1 << WGM01);
    // Prescaler 64 
    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 25000; // (16MHz / 64 )* 0.1s)
    // Włącz przerwanie porównania (compare match) dla Timera 0
    TIMSK0 |= (1 << OCIE0A);
}

static inline void heater_on() {
  PORTB |= _BV(HEATER_PIN);
}

static inline void heater_off() {
  PORTB &= ~_BV(HEATER_PIN);
}

volatile float temp = 24.0;
const float hysteresis = 1.0;

volatile float adc_reading;
volatile float current_temp;
volatile float voltage;

ISR(TIMER0_COMPA_vect) {
}

ISR(ADC_vect) {
  adc_reading = ADC; // weź zmierzoną wartość (0..1023)
    
  voltage = (adc_reading * 1.1) / 1024.0; // Przeliczenie wartości ADC na napięcie
  current_temp = (voltage - 0.5) / 0.01; // Konwersja napięcia na temperaturę

  if (current_temp >= temp + hysteresis) {
    heater_off();	
  } else if (current_temp <= temp - hysteresis) {
    heater_on();
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
      printf("Current temperature =  %f\r\n", current_temp);
    }
    else if (strcmp(command, "set") == 0)
    {
      scanf("%f", &temp);
      printf("Temperature set to %f\r\n", temp);
    }
    else
      printf("Invalid command. Use set/read instead\r\n");
  }
}
