#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <string.h>
#include "i2c.c"


#define i2cCheck(code, msg) \
    if ((TWSR & 0xf8) != (code)) { \
      printf(msg " failed, status: %.2x\r\n", TWSR & 0xf8); \
      i2cReset(); \
    }


#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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

const uint8_t eeprom_addr = 0xD0;

static inline void write_sec(uint8_t sec){
    i2cStart();
    i2cSend(eeprom_addr); // eeprom_addr | 2;
    i2cSend(0);
    uint8_t high, low;
    high = (sec / 10) << 4;
    low = sec % 10;
    i2cSend(high | low);
    i2cStop();
}

static inline void write_min(uint8_t min){
    i2cStart();
    i2cSend(eeprom_addr); // eeprom_addr | 2;
    i2cSend(1);
    uint8_t high, low;
    high = (min / 10) << 4;
    low = min % 10;
    i2cSend(high | low);
    i2cStop();
}

static inline void write_hour(uint8_t hour){
    i2cStart();
    i2cSend(eeprom_addr); // eeprom_addr | 2;
    i2cSend(2);
    uint8_t high, low;
    high = (hour / 10) << 4;
    low = hour % 10;
    i2cSend(high | low);
    i2cStop();
}

static inline uint8_t read(uint16_t addr){
    // Dummy write
    i2cStart();
    i2cSend(eeprom_addr);
    i2cSend(addr);

    i2cStart();
    i2cSend(eeprom_addr | 0x1);
    
    uint8_t data = i2cReadNoAck();
    i2cStop();
    return data;
}

int main()
{
    // zainicjalizuj UART
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    // zainicjalizuj I2C
    i2cInit();
  
    char command[100];
    while(1){
        scanf("%s", command);
        if(strcmp(command, "set") == 0){
            scanf("%s", command);
            if(strcmp(command, "time") == 0){
                uint8_t hours, minutes, seconds;
                scanf("%"PRIu8" %"PRIu8" %"PRIu8, hours, minutes, seconds);
                write_hour(hours);
                write_min(minutes);
                write_sec(seconds);
            }
            
        }
        else if(strcmp(command, "time") == 0){
            uint8_t hours = read(2);
            uint8_t hours_low = hours & 0x0F;
            hours = hours_low + (hours >> 4);
            uint8_t minutes = read(1);
            uint8_t minutes_low = minutes & 0x0F;
            minutes = minutes_low + (minutes >> 4);
            uint8_t seconds = read(0);
            uint8_t seconds_low = seconds & 0x0F;
            seconds = seconds_low + (seconds >> 4);
            printf("%d:%d:%d", hours, minutes, seconds);
        }
        else if(strcmp(command, "date") == 0){
            uint8_t days = read(4);
            days = days & 0x0F + (days >> 4);
            uint8_t months = read(5);
            months = months & 0x0F + (months >> 4);
            uint8_t years = read(6);
            years = years & 0x0F + (years >> 4);
            printf("%d/%d/%d", days, months, years);
        }
    }
}