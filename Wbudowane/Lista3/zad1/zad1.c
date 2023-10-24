#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define TONE(step, delay) \
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step) / 2; i++) { \
      BUZZ_PORT |= _BV(BUZZ); \
      _delay_us(step); \
      BUZZ_PORT &= ~_BV(BUZZ); \
      _delay_us(step); \
    }


#define multiplier      1.0595
#define base            240

// #define delay           1000

#define full_note       1000
#define half_note       full_note / 2
#define quarter_note    half_note / 2
#define eighth_note     quarter_note / 2
#define sixteenth_note  eighth_note / 2


#define CC base
#define H CC * multiplier
#define A H  * multiplier * multiplier
#define G A  * multiplier * multiplier
#define F G  * multiplier * multiplier
#define E F  * multiplier
#define D E  * multiplier * multiplier
#define C D  * multiplier * multiplier
#define H_l C * multiplier
#define A_l H_l * multiplier * multiplier

#define tabs_len 28

const int16_t steps[tabs_len] PROGMEM = {A_l,C,E,CC,H,G,A,0,A,CC,A,F,E,0,E,D,C,0,C,G,C,D,0,D,0,D,E,H_l};

const int16_t delays[tabs_len] PROGMEM = {half_note, quarter_note, quarter_note, half_note, quarter_note, quarter_note,
                            eighth_note, eighth_note, quarter_note, quarter_note, quarter_note, full_note, quarter_note,
                            quarter_note, quarter_note, quarter_note, eighth_note, eighth_note, quarter_note, quarter_note, quarter_note,
                            eighth_note, eighth_note, eighth_note, eighth_note, quarter_note, quarter_note, full_note};

int main() {
  BUZZ_DDR |= _BV(BUZZ);
  while (1) {
    for(int i = 0; i < tabs_len; i++){
      uint16_t step = pgm_read_word(&(steps[i]));
      uint16_t delay = pgm_read_word(&(delays[i]));
      if(step == 0){
        _delay_ms(delay);
      }
      else{
        TONE(step, delay);
      }
    }
    // TONE(A_l, half_note);
    // TONE(C, quarter_note);
    // TONE(E, quarter_note);
    // TONE(CC, half_note);
    // TONE(H, quarter_note);
    // TONE(G, quarter_note);
    // TONE(A, eighth_note);
    // _delay_ms(eighth_note);
    // TONE(A, quarter_note);
    // TONE(CC, quarter_note);
    // TONE(A, quarter_note);
    // TONE(F, full_note);
    // TONE(E, quarter_note);
    // _delay_ms(quarter_note);
    // TONE(E, quarter_note);
    // TONE(D, quarter_note);
    // TONE(C, eighth_note);
    // _delay_ms(eighth_note);
    // TONE(C, quarter_note);
    // TONE(G, quarter_note);
    // TONE(C, quarter_note);
    // TONE(D, eighth_note);
    // _delay_ms(eighth_note);
    // TONE(D, eighth_note);
    // _delay_ms(eighth_note);
    // TONE(D, quarter_note);
    // TONE(E, quarter_note);
    // TONE(H_l, full_note);
  }
}
