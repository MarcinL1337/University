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

const uint8_t eeprom_addr = 0xa0;

static inline void write(uint16_t addr, uint8_t len, uint8_t data[]){
    i2cStart();
    i2cCheck(0x08, "I2C start");
    i2cSend(eeprom_addr | ((addr & 0x100) >> 7)); // eeprom_addr | 2;
    i2cCheck(0x18, "I2C EEPROM write request")
    i2cSend(addr & 0xff);
    i2cCheck(0x28, "I2C EEPROM set address")
    for(int i = 0; i < len; i++)
        i2cSend(data[i]);
    i2cStop();
    i2cCheck(0xf8, "I2C stop")
}

static inline uint8_t read(uint16_t addr, uint8_t no_of_bytes){
    // Dummy write
    i2cStart();
    i2cCheck(0x08, "I2C start")
    i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
    i2cCheck(0x18, "I2C EEPROM write request")
    i2cSend(addr & 0xff);
    i2cCheck(0x28, "I2C EEPROM set address")

    i2cStart();
    i2cCheck(0x10, "I2C second start")
    i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));
    i2cCheck(0x40, "I2C EEPROM read request")

    printf(":%.2x|%.4x|", no_of_bytes, addr);

    for(uint8_t i = 0; i < no_of_bytes - 1; i++){
        uint8_t data = i2cReadAck();
        // printf("%x: %"PRId8"\r\n", addr + i, data);
        printf("%.2x|", data);
    }

    uint8_t data = i2cReadNoAck();
    i2cCheck(0x58, "I2C EEPROM read")
    // printf("%x: %"PRId8"\r\n", addr + no_of_bytes-1, data);
    printf("%.2x\r\n", data);
    i2cStop();
    i2cCheck(0xf8, "I2C stop")
    return data;
}

int hex2int(char ch){
    if(ch >= '0' && ch <= '9') return ch - '0';
    if(ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return -1;
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
  
    char command[10];
    uint16_t addr;
    uint8_t data[16];
    uint8_t read_len;
    char i8hex[400];
    uint8_t write_len;
    while(1) {

        scanf("%s", command);
        if(strcmp(command, "write") == 0){
            // printf("write ");
            scanf("%s", i8hex);
            write_len = hex2int(i8hex[1]) * 16 + hex2int(i8hex[2]);
            addr = hex2int(i8hex[3]) * 16 * 16 * 16 + hex2int(i8hex[4]) * 16 * 16 + hex2int(i8hex[5]) * 16 + hex2int(i8hex[6]);
            
            for(int i = 0; i < write_len; i++){
                data[i] = hex2int(i8hex[9 + 2 * i]) * 16 + hex2int(i8hex[10 + 2 * i]);
            }

            write(addr, write_len, data);
        }
        else if(strcmp(command, "read") == 0){
            scanf("%x", &addr);
            scanf("%"SCNu8, &read_len);
            printf("Read addr: %.3x num of bytes: %"PRId8"\r\n", addr, read_len);
            uint8_t res = read(addr, read_len);
        }
        else{
            printf("Invalid command, use <write addr array_of_data> or <read addr how_many_bytes>\r\n");
        }
    }
}