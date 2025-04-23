// fuse bit : 0xCF 0xEF
// high 0b11001111
// low  0b11101111

// for SCK_KM_V1.2.x Board

// DO NOT PUSH ANY KEY WHILE DOWNLOADING!!!

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "i2c_slave.h"
#include "watchdog.h"

// general call data (power, ---, ---, ---, ---, scroll_lock, caps_lock, num_lock)

void pin_init(void);
void adc_init(void);
void get_key_state(void);
void get_key_state_h(unsigned char mask);

unsigned char my_address = 0x00;                // I2C address
volatile bool power_state = false;              // led on/off
volatile unsigned char key_state[14] = {0,}; // key line H1 ~ H14 {---, ---, key_v1, key_v2, key_v3, key_v4, key_v5, key_v6}
unsigned char key_temp[14] = {0,};

int main(void) {
  
  pin_init();
  my_address = 0x10;
  I2C_init_slave(my_address);
  
  TWCR |= 0x80; //clear TWINT
  WDT_enable();
  sei();
  
  adc_init();
  
  _delay_ms(100);
  
  while(1) {
    get_key_state();
    for (unsigned char i=0; i<14; i++) {
      I2C_writing_data[i] = key_state[i];
    }
    power_state = I2C_general_data[0] & 0x80;
    
    if((TWSR & 0xF8) == 0x00) { // if I2C is error condition
      TWCR &= ~0x10; // clear TWSTO to escape I2C error
      TWAR = (my_address << 1) | 1; // address set + general call enable
    }
    WDT_reset();
    //_delay_ms(1);
  }
}

void pin_init(void) {

  DDRB  = 0x3F;	// 0b00111111 // (XTAL2), (XTAL1), key_v6, key_v5, key_v4, key_v3, key_v2, key_v1
  PORTB = 0x3F;	// 0b00111111 // high output
  
  DDRC  = 0x00;	// 0b00000000 // ---, reset, SCL, SDA, key_h6, key_h5, key_h4, key_h3
  PORTC = 0x0F;	// 0b00001111 // pull-up
  
  DDRD  = 0x00;	// 0b00000000 // key_v14, key_v13, key_h12, key_h11, key_h10, key_h9, key_h8, key_h7
  PORTD = 0xFF;	// 0b11111111 // pull-up
} // key_h1 : ADC6, key_h2 : ADC7

void adc_init(void) {
  ADCSRA = 0x84;
  ADMUX = 0x40;
}

void get_key_state(void) {

  unsigned char i;

  for(i=0; i<14; i++) {
    key_temp[i] = 0;
  }

  for(i=0; i<6; i++) {
    PORTB &= ~(0x01 << i);        // key_vn low
    get_key_state_h(0x20 >> i);
    PORTB |= (0x01 << i);         // key_vn high
  }

  for(i=0; i<14; i++) {           // save key state
    key_state[i] = key_temp[i];
  }
}

void get_key_state_h(unsigned char mask) {
  unsigned short temp;
  
  ADMUX = 0x46;   // set ADC6
  //_delay_us(150);
  ADCSRA = 0xD4;  // start ADC
  
  key_temp[2]  |= ((PINC & 0x01) ? 0x00 : mask);     // key_h3  check
  key_temp[3]  |= ((PINC & 0x02) ? 0x00 : mask);     // key_h4  check
  key_temp[4]  |= ((PINC & 0x04) ? 0x00 : mask);     // key_h5  check
  key_temp[5]  |= ((PINC & 0x08) ? 0x00 : mask);     // key_h6  check
  key_temp[6]  |= ((PIND & 0x01) ? 0x00 : mask);     // key_h7  check
  key_temp[7]  |= ((PIND & 0x02) ? 0x00 : mask);     // key_h8  check

  while((ADCSRA & 0x10) != 0x10);  // wait until ADC complete
  // key_h1 check (ADC6)
  temp = ADCW;
  if(temp > 768) // if high
    key_temp[0] |= 0x00;
  else
    key_temp[0] |= mask;
  
  ADMUX = 0x47;   // set ADC7
  //_delay_us(150);
  ADCSRA = 0xD4;  // start ADC
  
  key_temp[8]  |= ((PIND & 0x04) ? 0x00 : mask);     // key_h9  check
  key_temp[9]  |= ((PIND & 0x08) ? 0x00 : mask);     // key_h10 check
  key_temp[10] |= ((PIND & 0x10) ? 0x00 : mask);     // key_h11 check
  key_temp[11] |= ((PIND & 0x20) ? 0x00 : mask);     // key_h12 check
  key_temp[12] |= ((PIND & 0x40) ? 0x00 : mask);     // key_h13 check
  key_temp[13] |= ((PIND & 0x80) ? 0x00 : mask);     // key_h14 check
  
  while((ADCSRA & 0x10) != 0x10);  // wait until ADC complete
  // key_h2 check (ADC7)
  temp = ADCW;
  if(temp > 768) // if high
    key_temp[1] |= 0x00;
  else
    key_temp[1] |= mask;
}