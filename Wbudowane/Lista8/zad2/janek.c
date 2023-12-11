#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

#define mainREAD_TASK_PRIORITY 1
#define mainLED_TASK_PRIORITY 2

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

static void vBlinkLed(void * xQueue);
static void vReadNum(void * xQueue);

int uart_transmit(char c, FILE *stream);
int uart_receive(FILE *stream);

FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

// inicjalizacja UART
void uart_init(void) {
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
    xTaskHandle serial_handle;

    // Create xQueue
    QueueHandle_t xQueue = xQueueCreate(10, sizeof(int));

    xTaskCreate
        (
         vReadNum,
         "blink",
         200,
         (void *)xQueue,
         mainREAD_TASK_PRIORITY,
         &blink_handle
        );

    xTaskCreate
        (
         vBlinkLed,
         "serial",
         configMINIMAL_STACK_SIZE,
         (void *)xQueue,
         mainLED_TASK_PRIORITY,
         &serial_handle
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

void vApplicationIdleHook(void){}

static void vReadNum(void * xQueue) {
  QueueHandle_t queue = xQueue;
  uart_init();
  stdin = stdout = stderr = &uart_file;
  int input;
  for ( ;; ) {
    scanf("%d", &input);
    xQueueSend(queue, (void *)&input, 100);
    printf("%d\r\n", input);
  }
}

static void vBlinkLed(void * xQueue) {
  QueueHandle_t queue = xQueue;
  LED_DDR |= _BV(LED);
  while (1) {
    int num = 0;
    if (xQueueReceive(queue, &num, 100)) {
      LED_PORT |= _BV(LED);
      vTaskDelay(num);
    }
    LED_PORT &= ~_BV(LED);
    vTaskDelay(100);
  }
}