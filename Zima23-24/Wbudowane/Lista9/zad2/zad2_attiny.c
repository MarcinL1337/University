#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED PB2
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PA7
#define BTN_PIN PINA
#define BTN_PORT PORTA

// inicjalizacja licznika 1
void timer1_init(){
  // preskaler 1024
  ICR1 = 10;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10) | _BV(CS12);
  // odmaskowanie przerwania przepełnienia licznika
  TIMSK1 |= _BV(TOIE1);
}

// Wysyła do slave'a i odbiera od slave'a
uint8_t spi_transfer(uint8_t data)
{
    // załaduj dane do przesłania
    USIDR = data;
    // wyczyść flagę przerwania USI
    USISR = _BV(USIOIF);
    // póki transmisja nie została ukończona, wysyłaj impulsy zegara
    while (!(USISR & _BV(USIOIF))) {
        // wygeneruj pojedyncze zbocze zegarowe
        // zostanie wykonane 16 razy
        USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
    }
    // zwróć otrzymane dane
    return USIDR;
}

volatile uint8_t data = 0;
// procedura obsługi przerwania przepełnienia licznika
ISR(TIM1_OVF_vect) {
  data = spi_transfer(!(BTN_PIN & _BV(BTN)));
  if (data) LED_PORT |=  _BV(LED);
  else      LED_PORT &= ~_BV(LED);
}

void spi_init()
{
    // ustaw piny MOSI i SCK jako wyjścia
    DDRA = _BV(DDA4) | _BV(DDA5);
    // ustaw USI w trybie trzyprzewodowym (SPI)
    USICR = _BV(USIWM0);
}

int main()
{
    BTN_PORT |= _BV(BTN);
    LED_DDR |= _BV(LED);
    
    timer1_init();
    spi_init();
    set_sleep_mode(SLEEP_MODE_IDLE);
    sei();
    while (1) {
        sleep_mode();
    }
}