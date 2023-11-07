#include <avr/io.h>
#include <avr/pgmspace.h>
#include <time.h>
#include <util/delay.h>


#define LED_R PB2
#define LED_G PB1
#define LED_B PB3
#define LED_DDR DDRB
#define LED_PORT PORTB

typedef struct {
    uint32_t h;  // Kąt (od 0 do 36000 stopni)
    uint32_t s;  // 100
    uint32_t v;  // Jasność (od 0 do 100)
} HSVColor;

typedef struct {
    uint32_t r;  // Składowa czerwona (0-255)
    uint32_t g;  // Składowa zielona (0-255)
    uint32_t b;  // Składowa niebieska (0-255)
} RGBColor;


RGBColor HSVtoRGB(HSVColor hsv) {
    int c = hsv.v;
    int x = c * (60 - abs(hsv.h % 120 - 60)) / 100;
    int m = 0;

    int r, g, b;

    if (hsv.h >= 0 && hsv.h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (hsv.h >= 60 && hsv.h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (hsv.h >= 120 && hsv.h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (hsv.h >= 180 && hsv.h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (hsv.h >= 240 && hsv.h < 300) {
        r = x;
        g = 0;
        b = c;
    } else {
        r = c;
        g = 0;
        b = x;
    }

    RGBColor rgb;
    rgb.r = (r + m) * 255 / 100;
    rgb.g = (g + m) * 255 / 100;
    rgb.b = (b + m) * 255 / 100;

    return rgb;
}


const int16_t tab[] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 24, 24, 25, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 73, 74, 75, 76, 77, 78, 78, 79, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 87, 88, 89, 89, 90, 90, 91, 92, 92, 93, 93, 94, 94, 94, 95, 95, 96, 96, 96, 97, 97, 97, 98, 98, 98, 98, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 98, 98, 98, 98, 97, 97, 97, 97, 96, 96, 95, 95, 95, 94, 94, 93, 93, 92, 92, 91, 91, 90, 89, 89, 88, 87, 87, 86, 85, 85, 84, 83, 83, 82, 81, 80, 79, 79, 78, 77, 76, 75, 74, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 44, 44, 43, 43, 42, 42, 41, 41, 40, 40, 39, 39, 38, 38, 37, 37, 36, 36, 35, 35, 34, 34, 33, 33, 32, 32, 31, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 26, 25, 25, 24, 24, 23, 23, 23, 22, 22, 21, 21, 21, 20, 20, 19, 19, 19, 18, 18, 17, 17, 17, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void timer1_init()
{
  ICR1 = 255;

  TCCR1A = _BV(WGM11) | (1 << COM1B1) | (1 << COM1A1);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS12);

  TCCR2A = _BV(COM2A1) | _BV(WGM20) | _BV(WGM21);
  TCCR2B = _BV(CS21) | _BV(CS22);

  LED_DDR |= _BV(LED_G);
  LED_DDR |= _BV(LED_R);
  LED_DDR |= _BV(LED_B);
}

int main()
{
  // uruchom licznik
  timer1_init();
  OCR1A = ICR1;
  OCR1B = ICR1;
  OCR2A = ICR1;
  uint32_t hue;
  while(1){
    hue = rand() % 360;
    for (int i = 0; i < sizeof(tab)/sizeof(tab[0]); i++) {
      while(TCNT1 != 0);
      uint32_t value = pgm_read_word(&(tab[i]));
      RGBColor rgb = HSVtoRGB((HSVColor){.h = hue, .s = 100, .v = value});
      OCR1A = (255 - rgb.g);
      OCR1B = (255 - rgb.r);
      OCR2A = (255 - rgb.b);
    }
  } 
}