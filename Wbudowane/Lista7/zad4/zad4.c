#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#define SPI_PORT PORTD
#define SPI_DDR  DDRD

#define MISO PD4
#define MOSI PD5
#define SCK  PD6
#define SS   PD7

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

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
  /* Return Data Register */
  return SPDR;
}

void sendByBitBanging(uint8_t data){
  SPI_PORT &= ~_BV(SS);
  for(int i = 0; i < 8; i++){
    if((data>>(7-i))&1)
      SPI_PORT |=  _BV(MOSI);
    else
      SPI_PORT &= ~_BV(MOSI);
    
    SPI_PORT |=  _BV(SCK);
    _delay_ms(5);
    SPI_PORT &= ~_BV(SCK);
  }
  SPI_PORT |=  _BV(SS);
}

int main() {
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  
  SPI_SlaveInit();
  SPI_DDR |= _BV(MOSI) | _BV(SCK) | _BV(SS);
  for(int i = 0;; i++){
    sendByBitBanging(i);
    printf("%"PRIu8"\r\n", SPI_SlaveReceive());
  }
}