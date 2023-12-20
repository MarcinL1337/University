#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

#define LED PD7
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BTN PD6
#define BTN_PIN PIND
#define BTN_PORT PORTD


void io_init(){
  // pull-up
  BTN_PORT |= _BV(BTN);
  LED_DDR |= _BV(LED);
}

// inicjalizacja UART
void uart_init() {
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream) {
  // czekaj aż transmiter gotowy
  while (!(UCSR0A & _BV(UDRE0)))
    ;
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream) {
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)))
    ;
  return UDR0;
}

FILE uart_file;

void SPI_SlaveInit(void) {
  /* Set MISO output, all others input */
  DDRB = (1 << PB4);
  /* Enable SPI */
  SPCR = (1 << SPE);
}

uint8_t SPI_SlaveReceive(void) {
  /* Wait for reception complete */
  while (!(SPSR & (1 << SPIF)))
    ;
    //sprawdz czy wcisniety przycisk
  SPDR = !(BTN_PIN & _BV(BTN));
  /* Return Data Register */
  return SPDR;
}

int main() {
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  sei();
  SPI_SlaveInit();
  io_init();
  uint8_t data;
  while(1){
    data = SPI_SlaveReceive();
    if (data) LED_PORT |=  _BV(LED);
    else      LED_PORT &= ~_BV(LED);
  }
}