#include "FreeRTOS.h"
#include "task.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

#define buffer_size 100

bool buffer[buffer_size];
int write_idx = 100, read_idx = 1;


// UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
uint8_t pds[8] = {PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7};


#define mainLED_TASK_PRIORITY   2
#define mainLED_BAR_TASK_PRIORITY 1

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

static void vBlinkLed(void* pvParameters);
static void vLedBar(void* pvParameters);

int uart_transmit(char c, FILE *stream);
int uart_receive(FILE *stream);

FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

// inicjalizacja UART
void uart_init(void)
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

int uart_transmit(char c, FILE *stream) {
  while (!(UCSR0A & _BV(UDRE0))) taskYIELD();
  UDR0 = c;
  return 0;
}

int uart_receive(FILE *stream) {
  while (!(UCSR0A & _BV(RXC0))) taskYIELD();
  return UDR0;
}

int main(void)
{
    // Create task.
    xTaskHandle blink_handle;
    xTaskHandle led_bar_handle;

    xTaskCreate
        (
         vBlinkLed,
         "blink",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainLED_TASK_PRIORITY,
         &blink_handle
        );

    xTaskCreate
        (
         vLedBar,
         "led_bar",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainLED_BAR_TASK_PRIORITY,
         &led_bar_handle
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}


void vApplicationIdleHook(void)
{

}

static void vBlinkLed(void* pvParameters)
{
    for(int i = 0; i < buffer_size; i++)
        buffer[i] = false;

    BTN_PORT |= _BV(BTN);
    LED_DDR |= _BV(LED);

    while(1){
        if(BTN_PIN & _BV(BTN)) buffer[write_idx] = false;
        else                   buffer[write_idx] = true;

        if(buffer[read_idx])   LED_PORT |= _BV(LED);
        else                   LED_PORT &= ~_BV(LED);

        read_idx = (read_idx + 1) % buffer_size;
        write_idx = (write_idx + 1) % buffer_size;
        vTaskDelay(10);
    }
}


static void vLedBar(void* pvParameters)
{
    DDRD  = 0b11111111;
    while(1){
        PORTD = 0b00000000;

        for(int i = 0; i < 8; i++){
            PORTD |= _BV(pds[i]);
            vTaskDelay(75);
            PORTD &= ~_BV(pds[i]);
        }
        for(int i = 6; i > 0; i--){
            PORTD |= _BV(pds[i]);
            vTaskDelay(75);
            PORTD &= ~_BV(pds[i]);
        }
    }
}
