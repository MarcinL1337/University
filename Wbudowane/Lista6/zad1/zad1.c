#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

char data;

void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // // wyczyść rejestr UCSR0A
  // UCSR0A = 0;


  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);

  /*
    RXCIEn:
    Writing this bit to one enables interrupt on the RXCn Flag. A USART Receive Complete interrupt will be generated
    only if the RXCIEn bit is written to one, the Global Interrupt Flag in SREG is written to one and the RXCn bit in
    UCSRnA is set.

    TXCIEn:
    Writing this bit to one enables interrupt on the TXCn Flag. A USART Transmit Complete interrupt will be generated
    only if the TXCIEn bit is written to one, the Global Interrupt Flag in SREG is written to one and the TXCn bit in UCS-
    RnA is set.
  */
  UCSR0B |= _BV(RXCIE0) | _BV(TXCIE0);
  // UCSR0A = _BV(TXC0) | _BV(RXC0); To się ustawia samo, jak w rejestrze jest nieprzeczytana data / opróżniany jest bufor transmit

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

// Rx -> Receive
// Tx -> Transmit
// USART Rx Complete

/*
  The USART Transmit Data Buffer Register and USART Receive Data Buffer Registers share the same I/O address
  referred to as USART Data Register or UDRn. The Transmit Data Buffer Register (TXB) will be the destination for
  data written to the UDRn Register location. Reading the UDRn Register location will return the contents of the
  Receive Data Buffer Register (RXB).
*/

ISR(USART_RX_vect){
  data = UDR0;
  UDR0 = data;
  // UDR0 = UDR0;
}

ISR(USART_TX_vect){
  
}


int main()
{
  // zainicjalizuj UART
  uart_init();
  sei();
  set_sleep_mode(SLEEP_MODE_IDLE);

  while(1) {
    sleep_mode();
  }
}
