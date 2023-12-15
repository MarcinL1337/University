#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "hd44780.c"

FILE uart_file;

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

void upload_symbols(){
  LCD_WriteCommand(HD44780_CGRAM_SET);
  for(uint8_t i = 0; i < 6; i++)
    LCD_WriteData(0x1F << i);
}

void progress_bar(uint8_t percent){
  LCD_GoTo(6, 1);
  uint16_t cur_progress = (percent * 100) / 80;
  printf("%.2d", cur_progress);
  putchar(0b00100101);

  uint8_t aux = percent / 6;
  uint8_t aux2 = percent % 6;
  
  LCD_GoTo(aux + 1, 0);
  putchar(aux2);
  

  // putchar() tu skończyłem xd
}

FILE hd44780_file;

int main()
{
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  LCD_Clear();
  
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
  stdout = stderr = &hd44780_file;

  while(1){
    for(uint8_t i = 0; i < 81; i++){
      progress_bar(i);
      _delay_ms(100);
    }
    _delay_ms(2000);
    LCD_Clear();
  }
}


