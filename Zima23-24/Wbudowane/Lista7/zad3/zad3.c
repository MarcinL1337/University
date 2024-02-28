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

static inline void write(uint16_t addr, uint8_t data){
    i2cStart();
    i2cSend(eeprom_addr);
    i2cSend(addr);
    data = (((data) / 10) << 4) | ((data) % 10);
    i2cSend(data);
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
                scanf("%"PRIu8" %"PRIu8" %"PRIu8, &hours, &minutes, &seconds);
                write(2, hours);
                write(1, minutes);
                write(0, seconds);
            }
            else if(strcmp(command, "date") == 0){
                uint8_t years, months, days;
                scanf("%"PRIu8" %"PRIu8" %"PRIu8, &years, &months, &days);
                write(6, years);
                write(5, months);
                write(4, days);
            }
            else{
                printf("Invalid command. Type one of: time, date, set time, set date\r\n");
            }
        }
        else if(strcmp(command, "time") == 0){
            uint8_t hours = read(2);
            uint8_t hours_low = hours & 0x0F;
            uint8_t hours_high = (hours >> 4) * 10;
            hours = hours_low + hours_high;
            uint8_t minutes = read(1);
            uint8_t minutes_low = minutes & 0x0F;
            uint8_t minutes_high = (minutes >> 4) * 10;
            minutes = minutes_low + minutes_high;
            uint8_t seconds = read(0);
            uint8_t seconds_low = seconds & 0x0F;
            uint8_t second_high = (seconds >> 4) * 10;
            seconds = seconds_low + second_high;
            printf("%d:%d:%d\r\n", hours, minutes, seconds);
        }
        else if(strcmp(command, "date") == 0){
            uint8_t days = read(4);
            days = days & 0x0F + ((days >> 4) & 0x3) * 10;
            uint8_t months = read(5);
            months = (months & 0x0F) + ((months >> 4) & 0x1) * 10;
            uint8_t years = read(6);
            years = (years & 0x0F) + ((years >> 4) * 10);
            printf("%d/%d/%d\r\n", days, months, years);
        }
        else{
            printf("Invalid command. Type one of: time, date, set time, set date\r\n");
        }
    }
}