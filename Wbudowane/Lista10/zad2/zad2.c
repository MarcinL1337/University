#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "hd44780.c"

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

void upload_symbols(){
  LCD_WriteCommand(HD44780_CGRAM_SET);
  uint8_t aux = 0b00000;
  for(uint8_t i = 0; i < 6; i++){
    aux |= (1 << (5-i));
    for(uint8_t j = 0; j < 8; j++)  
      LCD_WriteData(aux);
  }
}

void progress_bar(uint8_t percent){
  LCD_GoTo(6, 1);
  uint16_t cur_progress = (percent * 100) / 80;
  printf("%.2d", cur_progress);
  putchar(0b00100101);
  // printf("%c", 0b00100101);

  uint8_t new_x = percent / 6;
  uint8_t aux2 = percent % 6;
  
  LCD_GoTo(new_x + 1, 0);

  if(new_x == 13) putchar(aux2 + 3);
  else putchar(aux2);

  // printf("%c", aux2);
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

  upload_symbols();
  while(1){
    for(uint8_t i = 0; i < 81; i++){
      progress_bar(i);
      _delay_ms(100);
    }
    _delay_ms(2000);
    LCD_Clear();
  }
}