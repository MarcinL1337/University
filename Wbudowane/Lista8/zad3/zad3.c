#include "FreeRTOS.h"
#include "task.h"
#include <avr/interrupt.h>
#include "queue.h"
#include <stdio.h>

#define mainUART_TASK_PRIORITY 2
#define mainLED_TASK_PRIORITY 1

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

static void vBlinkLed(void* pvParameters);
static void vSerial(void* pvParameters);

int uart_transmit(char data, FILE *stream);
int uart_receive(FILE *stream);

#define buf_size 100

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

QueueHandle_t transmit_queue;
QueueHandle_t receive_queue;

FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

void uart_init() {
  UCSR0A = 0;
  UBRR0 = UBRR_VALUE;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream){
  while(uxQueueSpacesAvailable(receive_queue) == 0) vTaskDelay(1);
  xQueueSendToBack(receive_queue, (void*)&data, 0);
  UCSR0B |= _BV(UDRIE0);
  return 0;
}

ISR(USART_UDRE_vect){
  if(uxQueueSpacesAvailable(receive_queue) == buf_size){
    UCSR0B &= ~_BV(UDRIE0);
    return;
  }
  char data;
  xQueueReceiveFromISR(receive_queue, (void*)&data, NULL);
  UDR0 = data;
}

int uart_receive(FILE *stream){
  while(uxQueueSpacesAvailable(transmit_queue) == buf_size) vTaskDelay(1);
  char data;
  xQueueReceive(transmit_queue, (void*)&data, 0);
  return data;
}

ISR(USART_RX_vect){
  char data = UDR0;
  xQueueSendToBackFromISR(transmit_queue, &data, NULL);
}

int main(void)
{
    sei();
    uart_init();
    stdin = stdout = stderr = &uart_file;

    // Create task.
    xTaskHandle blink_handle;
    xTaskHandle serial_handle;

    // Create xQueue
    receive_queue = xQueueCreate(buf_size, sizeof(char));
    transmit_queue = xQueueCreate(buf_size, sizeof(char));

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
         vSerial,
         "serial",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainUART_TASK_PRIORITY,
         &serial_handle
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

void vApplicationIdleHook(void) {}

static void vBlinkLed(void* pvParameters) {
  LED_DDR |= _BV(LED);
  while (1) {
    LED_PORT |= _BV(LED);
    vTaskDelay(200);
    LED_PORT &= ~_BV(LED);
    vTaskDelay(200);
  } 
}


static void vSerial(void* pvParameters) {
    char input;
    for (;;) {
        scanf("%c", &input);
        printf("You typed: %c\r\n", input); 
    }
}