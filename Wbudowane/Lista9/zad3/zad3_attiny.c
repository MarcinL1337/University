#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USI_TWI_Master.c"
#include <avr/sleep.h> 
#include <util/delay.h>

// inicjalizacja licznika 1
void timer1_init(){
  // preskaler 1024
  ICR1 = 10;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10) | _BV(CS12);
  // odmaskowanie przerwania przepełnienia licznika
  TIMSK1 |= _BV(TOIE1);
}


#define SLAVE_ADDR  0x7e

#define MESSAGEBUF_SIZE       2

#define TWI_GEN_CALL         0x00  // The General Call address is 0

// Sample TWI transmission commands
#define TWI_CMD_MASTER_WRITE 0x10
#define TWI_CMD_MASTER_READ  0x20

// Sample TWI transmission states, used in the main application.
#define SEND_DATA             0x01
#define REQUEST_DATA          0x02
#define READ_DATA_FROM_BUFFER 0x03


volatile uint8_t data = 0;
// procedura obsługi przerwania przepełnienia licznika
ISR(TIM1_OVF_vect) {
  char messageBuf[2];
  messageBuf[0] = SLAVE_ADDR;
  messageBuf[1] = data;
  USI_TWI_Start_Transceiver_With_Data( messageBuf, MESSAGEBUF_SIZE );
  data++;
  _delay_ms(200);
}


int main()
{
    USI_TWI_Master_Initialise();
    timer1_init();
    set_sleep_mode(SLEEP_MODE_IDLE);
    sei();
    while (1) {
        sleep_mode();
    }
}