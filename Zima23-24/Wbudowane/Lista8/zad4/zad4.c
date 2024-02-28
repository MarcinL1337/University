#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include "queue.h"
#include "semphr.h"

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

int uart_transmit(char c, FILE *stream);
int uart_receive(FILE *stream);

FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

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

void adc_init(uint8_t mux)
{
  ADMUX   =  _BV(REFS0) | mux;
  DIDR0   = _BV(mux); // wyłącz wejście cyfrowe na mux
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE); // preskaler 128
  ADCSRB |= _BV(ADTS1) | _BV(ACME);
}

xSemaphoreHandle semaphore;
uint16_t adc_res;

ISR(ADC_vect) {
  adc_res = ADC; 
  vTaskNotifyGiveFromISR(xSemaphoreGetMutexHolder(semaphore), NULL);
}

uint16_t ADC_aux(uint8_t mux) {
  uint16_t result;

  while(xSemaphoreTake(semaphore, 100) == pdFALSE);

  adc_init(mux);
  ADCSRA |=  _BV(ADEN); // ADC enable
  ADCSRA |= _BV(ADSC); // zacznij konwersję adc
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  ADCSRA |= _BV(ADIF);
  result = adc_res;
  
//   switch(mux){
//     case 1: printf("Termostat -> %d\n\r", result);
//     case 2: printf("Fotorezystor -> %d\n\r", result);
//     case 3: printf("Potencjometr -> %d\n\r", result);
//   }

  if(mux == 1) printf("Termostat -> %d\n\r", result);
  else if(mux == 2) printf("Fotorezystor -> %d\n\r", result);
  else if(mux == 3) printf("Potencjometr -> %d\n\r", result);
  
  ADCSRA &=  ~_BV(ADEN);

  xSemaphoreGive(semaphore);

  return result;
}


void adcTask(void *pvParameters) {
    uint8_t mux = (uint8_t) pvParameters;
    for(;;){
        uint16_t result = ADC_aux(mux);
        vTaskDelay(mux << 10);
    }
}

int main(void)
{
    sei();
    uart_init();
    stdin = stdout = stderr = &uart_file;
    semaphore = xSemaphoreCreateMutex();
    // Create task.
    xTaskHandle adcTask1;
    xTaskHandle adcTask2;
    xTaskHandle adcTask3;

    xTaskCreate
        (
         adcTask,
         "adcTask1",
         configMINIMAL_STACK_SIZE,
         (void *)1,
         1,
         &adcTask1
        );
    xTaskCreate
        (
         adcTask,
         "adcTask2",
         configMINIMAL_STACK_SIZE,
         (void *)2,
         1,
         &adcTask2
        );
    xTaskCreate
        (
         adcTask,
         "adcTask3",
         configMINIMAL_STACK_SIZE,
         (void *)3,
         1,
         &adcTask3
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}


void vApplicationIdleHook(void)
{

}