#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

#define DOT_TIME    500
#define DASH_TIME   1000
#define SPACE_TIME  2000

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

FILE uart_file;

int main()
{
    // zainicjalizuj UART
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    // program testowy
    printf("Hello world!\r\n");

    const char *letters = "**ETIANMSURWDKGOHVF?L?PJBXCYZQ??";

    LED_DDR |= _BV(LED);
    BTN_PORT |= _BV(BTN);

    int time_since_last_press = 0;
    int button_pressed_for = 0;
    int8_t cur_letter = 1;
    while (1)
    {
        /*
            Kropka -> 2*n
            Dash   -> 2*n + 1
        */
        if(BTN_PIN & _BV(BTN)){ //niewciśnięty
            LED_PORT &= ~_BV(LED);
            time_since_last_press++;
            if(time_since_last_press > 2*SPACE_TIME){
                putchar(' ');
                time_since_last_press = 0;
            }
            else if(time_since_last_press == SPACE_TIME && cur_letter > 1){
                char letter = letters[cur_letter];
                putchar(letter);
                cur_letter = 1;
            }
            else if(button_pressed_for > DOT_TIME){
                cur_letter = 2 * cur_letter + 1;
            }
            else if(button_pressed_for > 5){
                cur_letter *= 2;
            }
            button_pressed_for = 0;
        }
        else{ //wciśnięty
            button_pressed_for++;
            time_since_last_press = 0;
            if(button_pressed_for > DOT_TIME) LED_PORT |= _BV(LED);
        }

        _delay_ms(1);
    }
}
