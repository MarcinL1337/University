#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

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
  while (!(UCSR0A & _BV(UDRE0)))
    ;
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)))
    ;
  return UDR0;
}

void timer1_init()
{
  // ustaw tryb licznika
  // WGM1  = 0000 -- normal
  // CS1   = 001  -- prescaler 1
  TCCR1B = _BV(CS10);
}
FILE uart_file;

void foo()
{
  volatile uint16_t add_t0, add_t1, mul_t0, mul_t1, div_t0, div_t1;
  volatile int64_t mul4, div4, add4;
  volatile float   mul5, div5, add5;
  volatile int64_t a4 = 69000, b4 = 4200;
  volatile float a5 = 6.9, b5 = 4.2;

//   volatile int64_t a4 = 69, b4 = 42;
  // volatile float   a5 = 69.0, b5 = 42.0;
  printf("UINT64_T\r\n");
  TCNT1 = 0;
  add_t0 = TCNT1; // wartość licznika przed czekaniem
//   add4 = a4 + b4;
  add4 = a4 + b4;
  add_t1 = TCNT1; // wartość licznika po czekaniu
  TCNT1 = 0;
  mul_t0 = TCNT1; // wartość licznika przed czekaniem
  mul4 = a4 * b4;
  mul_t1 = TCNT1; // wartość licznika przed czekaniem
  TCNT1 = 0;
  div_t0 = TCNT1; // wartość licznika przed czekaniem
  div4 = a4 / b4;
  div_t1 = TCNT1; // wartość licznika przed czekaniem
  printf("Zmierzony czas dodawania : %" PRIu16 " cykli\r\n", add_t1 - add_t0);
  printf("Zmierzony czas mnozenia  : %" PRIu16 " cykli\r\n", mul_t1 - mul_t0);
  printf("Zmierzony czas dzielenia : %" PRIu16 " cykli\r\n", div_t1 - div_t0);

  printf("FLOAT\r\n");
  TCNT1 = 0;
  add_t0 = TCNT1; // wartość licznika przed czekaniem
  add5 = a5 + b5;
  add_t1 = TCNT1; // wartość licznika po czekaniu
  TCNT1 = 0;
  mul_t0 = TCNT1; // wartość licznika przed czekaniem
  mul5 = a5 * b5;
  mul_t1 = TCNT1; // wartość licznika przed czekaniem
  TCNT1 = 0;
  div_t0 = TCNT1; // wartość licznika przed czekaniem
  div5 = a5 / b5;
  div_t1 = TCNT1; // wartość licznika przed czekaniem
  printf("Zmierzony czas dodawania : %" PRIu16 " cykli\r\n", add_t1 - add_t0);
  printf("Zmierzony czas mnozenia  : %" PRIu16 " cykli\r\n", mul_t1 - mul_t0);
  printf("Zmierzony czas dzielenia : %" PRIu16 " cykli\r\n", div_t1 - div_t0);
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj licznik
  timer1_init();
  // program testowy
  volatile uint16_t add_t0, add_t1, mul_t0, mul_t1, div_t0, div_t1;
  volatile int8_t  mul1, div1, add1;
  volatile int16_t mul2, div2, add2;
  volatile int32_t mul3, div3, add3;
  volatile int8_t a1 = 6, b1 = 9;
  volatile int16_t a2 = 69, b2 = 42;
  volatile int32_t a3 = 690, b3 = 42;
  while (1)
  {
    printf("UINT8_T\r\n");
    TCNT1 = 0;
    add_t0 = TCNT1; // wartość licznika przed czekaniem
    add1 = a1 + b1;
    add_t1 = TCNT1; // wartość licznika po czekaniu
    TCNT1 = 0;
    mul_t0 = TCNT1; // wartość licznika przed czekaniem
    mul1 = a1 * b1;
    mul_t1 = TCNT1; // wartość licznika przed czekaniem
    TCNT1 = 0;
    div_t0 = TCNT1; // wartość licznika przed czekaniem
    div1 = a1 / b1;
    div_t1 = TCNT1; // wartość licznika przed czekaniem
    printf("Zmierzony czas dodawania : %" PRIu16 " cykli\r\n", add_t1 - add_t0);
    printf("Zmierzony czas mnozenia  : %" PRIu16 " cykli\r\n", mul_t1 - mul_t0);
    printf("Zmierzony czas dzielenia : %" PRIu16 " cykli\r\n", div_t1 - div_t0);

    printf("UINT16_T\r\n");
    TCNT1 = 0;
    add_t0 = TCNT1; // wartość licznika przed czekaniem
    add2 = a2 + b2;
    add_t1 = TCNT1; // wartość licznika po czekaniu
    TCNT1 = 0;
    mul_t0 = TCNT1; // wartość licznika przed czekaniem
    mul2 = a2 * b2;
    mul_t1 = TCNT1; // wartość licznika przed czekaniem
    TCNT1 = 0;
    div_t0 = TCNT1; // wartość licznika przed czekaniem
    div2 = a2 / b2;
    div_t1 = TCNT1; // wartość licznika przed czekaniem
    printf("Zmierzony czas dodawania : %" PRIu16 " cykli\r\n", add_t1 - add_t0);
    printf("Zmierzony czas mnozenia  : %" PRIu16 " cykli\r\n", mul_t1 - mul_t0);
    printf("Zmierzony czas dzielenia : %" PRIu16 " cykli\r\n", div_t1 - div_t0);

    printf("UINT32_T\r\n");
    TCNT1 = 0;
    add_t0 = TCNT1; // wartość licznika przed czekaniem
    add3 = a3 + b3;
    add_t1 = TCNT1; // wartość licznika po czekaniu
    TCNT1 = 0;
    mul_t0 = TCNT1; // wartość licznika przed czekaniem
    mul3 = a3 * b3;
    mul_t1 = TCNT1; // wartość licznika przed czekaniem
    TCNT1 = 0;
    div_t0 = TCNT1; // wartość licznika przed czekaniem
    div3 = a3 / b3;
    div_t1 = TCNT1; // wartość licznika przed czekaniem
    printf("Zmierzony czas dodawania : %" PRIu16 " cykli\r\n", add_t1 - add_t0);
    printf("Zmierzony czas mnozenia  : %" PRIu16 " cykli\r\n", mul_t1 - mul_t0);
    printf("Zmierzony czas dzielenia : %" PRIu16 " cykli\r\n", div_t1 - div_t0);
    foo();
    _delay_ms(10000);
  }
}
