#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

#define TIME_BETWEEN_DOTS 300
#define TIME_BETWEEN_SPACE 1000
#define TIME_BETWEEN_MORSE_ELEMENTS TIME_BETWEEN_DOTS
#define TIME_BETWEEN_LETTERS 3 * TIME_BETWEEN_DOTS
#define TIME_BETWEEN_STRINGS 7 * TIME_BETWEEN_DOTS

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

const uint8_t morse_code[26] = {0b01001000, 0b10010000, 0b10010100, 0b01110000, 0b00100000, 0b10000100, 0b01111000,
                                0b10000000, 0b01000000, 0b10001110, 0b01110100, 0b10001000, 0b01011000, 0b01010000,
                                0b01111100, 0b10001100, 0b10011010, 0b01101000, 0b01100000, 0b00110000, 0b01100100,
                                0b10000010, 0b01101100, 0b10010010, 0b10010110, 0b10011000};

// inicjalizacja UART
void uart_init()
{
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
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while (!(UCSR0A & _BV(UDRE0)))
    ;
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)))
    ;
  return UDR0;
}

FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  char in[128];
  while (1)
  {
    scanf("%[^\r\n]%c", in);
    printf("Odczytano: %s\r\n", in);
    int iter = 0, char_in_tab, morse_char_len;
    char cur_letter;
    while (in[iter] != '\0')
    {
      cur_letter = in[iter];
      printf("Odczytano: %c\r\n", cur_letter);
      char_in_tab = cur_letter - 'A';
      morse_char_len = (0xE0 & morse_code[char_in_tab]) >> 5;
      for (int i = 0; i < morse_char_len; i++)
      {
        uint8_t cur_morse_char = ((1 << (4 - i)) & morse_code[char_in_tab]) >> (4 - i);
        if (cur_morse_char & 1)
        {
          LED_PORT |= _BV(LED);
          _delay_ms(TIME_BETWEEN_SPACE);
          LED_PORT &= ~_BV(LED);
          _delay_ms(TIME_BETWEEN_MORSE_ELEMENTS);
        }
        else
        {
          LED_PORT |= _BV(LED);
          _delay_ms(TIME_BETWEEN_DOTS);
          LED_PORT &= ~_BV(LED);
          _delay_ms(TIME_BETWEEN_MORSE_ELEMENTS);
        }
      }
      iter++;
      _delay_ms(TIME_BETWEEN_LETTERS);
    }
    _delay_ms(TIME_BETWEEN_STRINGS);
  }
}