#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>
#include "hd44780.c"

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

char old_line[16];

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


// WCZYTYWANIE
int uart_receive(FILE *stream){
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

void shift_old_line(uint8_t length){
  LCD_Clear();
  LCD_GoTo(0, 0);
  for(uint8_t i = 0; i < length; i++){
    putchar(old_line[i]);
    _delay_ms(5);
  }
  // LCD_GoTo(0, 1);
}

FILE hd44780_file;
FILE uart_file;

int main()
{
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  LCD_Clear();
  LCD_WriteCommand(HD44780_CURSOR_ON | HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON);
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
  stdout = stderr = &hd44780_file;
  
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, NULL, uart_receive, _FDEV_SETUP_READ);
  stdin = &uart_file;

  uint8_t idx = 0;
  char input;

  LCD_GoTo(0, 1);

  while(1){
    input = getc(stdin);
    if(input == '\n' || input == '\r'){
      shift_old_line(idx);
      idx = 0;
      continue;
    }
    LCD_GoTo(idx, 1);
    putchar(input);
    old_line[idx++] = input;
    if(idx >= 16){
      shift_old_line(idx);
      idx = 0;
    }
  }
}


