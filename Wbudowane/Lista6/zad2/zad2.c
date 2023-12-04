#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define buf_size 100

int8_t transmit_buffer[buf_size] = {0}, receive_buffer[buf_size] = {0};
volatile uint8_t transmit_write = 1, transmit_read = 0;
volatile uint8_t receive_write = 1, receive_read = 0;

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);

  /*
    RXCIEn:
    Writing this bit to one enables interrupt on the RXCn Flag. A USART Receive Complete interrupt will be generated
    only if the RXCIEn bit is written to one, the Global Interrupt Flag in SREG is written to one and the RXCn bit in
    UCSRnA is set.
  */
  UCSR0B |= _BV(RXCIE0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

/*
  The Data Register Empty (UDREn) Flag indicates whether the transmit buffer is ready to receive new data. This bit
  is set when the transmit buffer is empty, and cleared when the transmit buffer contains data to be transmitted that
  has not yet been moved into the Shift Register.

  When the Data Register Empty Interrupt Enable (UDRIEn) bit in UCSRnB is written to one, the USART Data Reg-
  ister Empty Interrupt will be executed as long as UDREn is set. UDREn is cleared by writing UDRn. 
  When interrupt-driven data transmission is used, the Data Register Empty interrupt routine must either 
  write new data to UDRn in order to clear UDREn or disable the Data Register Empty
  interrupt, otherwise a new interrupt will occur once the interrupt routine terminates.
*/

//WYPISYWANIE
int uart_transmit(char data, FILE *stream)
{
  // Czekaj, jeżeli bufor nadawania jest pełny
  while(transmit_read == transmit_write);
  transmit_buffer[transmit_write] = data;
  transmit_write = (transmit_write + 1) % buf_size;
  if(transmit_read == ((transmit_write - 2) % buf_size)) UCSR0B |= _BV(UDRIE0);
  // UCSR0B |= _BV(UDRIE0);
  return 0;
}

// 0x0026 jmp USART_UDRE ; USART, UDR Empty Handler
// Przerwanie UDRE jest wyzwalane, gdy UART jest gotowy do przyjęcia nowego znaku do nadawania w rejestrze UDR.
ISR(USART_UDRE_vect){
  if(transmit_read == ((transmit_write - 1) % buf_size)){
    UCSR0B &= ~_BV(UDRIE0);
    return;
  }
  UDR0 = transmit_buffer[transmit_read];
  transmit_read = (transmit_read + 1) % buf_size;
}


// WCZYTYWANIE
int uart_receive(FILE *stream)
{
  // Czekaj, jeżeli bufor przyjmowania jest pusty
  while(receive_read == ((receive_write - 1) % buf_size));
  receive_read = (receive_read + 1) % buf_size;
  return receive_buffer[receive_read];
}

ISR(USART_RX_vect){
  receive_buffer[receive_write] = UDR0;
  receive_write = (receive_write + 1) % buf_size;
  // printf("Znak: %c\n", receive_buffer[receive_write-1]);
}

FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // program testowy
  sei();
  printf("Hello world!\r\n");
  printf("Proszę państwa to się transmituje\r\n");

  char wczytaj[100];
  scanf("%s", &wczytaj);
  printf("%s\r\n", wczytaj);
  
  scanf("%s", &wczytaj);
  printf("%s\r\n", wczytaj);

  scanf("%s", &wczytaj);
  printf("%s\r\n", wczytaj);
  // for(int i = 0; i <= strlen(wczytaj); i++){
  //   putc(wczytaj[i], stdout);
  // }
  // printf("\r\n");
  // scanf("%s", &wczytaj);

  // for(int i = 0; i <= strlen(wczytaj); i++){
  //   putc(wczytaj[i], stdout);
  // }
  // printf("\r\n");
  // scanf("%s", &wczytaj);

  // for(int i = 0; i <= strlen(wczytaj); i++){
  //   putc(wczytaj[i], stdout);
  // }

  while(1);
}
