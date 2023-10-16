#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define TIME_BETWEEN_DOTS 				300
#define TIME_BETWEEN_SPACE				1000
#define TIME_BETWEEN_MORSE_ELEMENTS 	TIME_BETWEEN_DOTS
#define SPACE_BETWEEN_CHARS            	3*TIME_BETWEEN_DOTS
#define SPACE_BETWEEN_STRING           	7*TIME_BETWEEN_DOTS

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

char* MORSE_CODE_DICT [26] = { ".-", "-...",
								"-.-.", "-..", ".",
								"..-.", "--.", "....",
								"..", ".---", "-.-",
								".-..", "--", "-.",
								"---", ".--.", "--.-",
								".-.", "...", "-",
								"..-", "...-", ".--",
								"-..-", "-.--", "--.."};


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

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  
	char in[128];
  while(1) {
		scanf("%[^\r\n]%c", in);
		printf("Odczytano: %s\r\n", in);
		int iter = 0;
		char cur_letter;
		int char_in_tab;
		while(in[iter] != '\0') {
			cur_letter = in[iter];
			printf("Odczytano: %c\r\n", cur_letter);
			char_in_tab = cur_letter - 'A';
			int counter = 0;
			char cur_char = MORSE_CODE_DICT[char_in_tab][counter];
			while(cur_char != '\0') {
				printf("Odczytano: %c\r\n", cur_char);
				if(cur_char == '-'){
					LED_PORT |= _BV(LED);
					_delay_ms(TIME_BETWEEN_SPACE);
					LED_PORT &= ~_BV(LED);
					_delay_ms(TIME_BETWEEN_MORSE_ELEMENTS);
				}
				else{
					LED_PORT |= _BV(LED);
					_delay_ms(TIME_BETWEEN_DOTS);
					LED_PORT &= ~_BV(LED);
					_delay_ms(TIME_BETWEEN_MORSE_ELEMENTS);
				}
				counter++;
				cur_char = MORSE_CODE_DICT[char_in_tab][counter];
			}
			iter++;

			_delay_ms(SPACE_BETWEEN_CHARS);
		}
		_delay_ms(SPACE_BETWEEN_STRING);
  }
}
