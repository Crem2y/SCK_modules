// fuse bit : 0xCF 0xEF
// high 0b11001111
// low  0b11101111

// for SCK_PM_V1.2.x Board

// DO NOT PUSH ANY KEY WHILE DOWNLOADING!!!

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "led_sk6812.h"
#include "i2c_slave.h"
#include "watchdog.h"

// general call data (power, ---, ---, ---, ---, scroll_lock, caps_lock, num_lock)

#define LED_COUNT 17
rgb_color pixel[LED_COUNT];

void pin_init(void);
unsigned char get_jp_state(void);
void get_key_state(void);
void get_key_state_h(unsigned char mask);

unsigned char my_address = 0x00;                // I2C address
volatile bool power_state = true;              // led on/off
volatile unsigned char key_state[4] = {0x00, 0x00, 0x00, 0x00}; // key line H1, H2, H3, H4 {---, ---, ---, key_v1, key_v2, key_v3, key_v4, key_v5}
unsigned char key_temp[4] = {0,};

int main(void) {
  
  pin_init();
  my_address = 0x18 + get_jp_state();
  I2C_init_slave(my_address);
  
  // boot led start
  PORTB &= ~0x38;
  pixel[0] = (rgb_color){15,15,15};
  led_strip_write(pixel, LED_COUNT);
  _delay_ms(50);
  for(unsigned char i=1; i<LED_COUNT; i++) {
    pixel[i] = (rgb_color){15,15,15};
    pixel[i-1] = (rgb_color){0,0,0};
    led_strip_write(pixel, LED_COUNT);
    _delay_ms(50);
  }
  pixel[LED_COUNT-1] = (rgb_color){0,0,0};
  led_strip_write(pixel, LED_COUNT);
  PORTB |= 0x38;
  // boot led end
  
  TWCR |= 0x80; //clear TWINT
  WDT_enable();
  sei();
  
  while(1) {
    get_key_state();
    for (unsigned char i=0; i<4; i++) {
      I2C_writing_data[i] = key_state[i];
    }
    //power_state = I2C_general_data[0] & 0x80;

    // lock led check
    if(I2C_general_data[0] & 0x01) {
      PORTB &= ~0x20;
    } else {
      PORTB |= 0x20;
    }
    if(I2C_general_data[0] & 0x02) {
      PORTB &= ~0x10;
    } else {
      PORTB |= 0x10;
    }
    if(I2C_general_data[0] & 0x04) {
      PORTB &= ~0x08;
    } else {
      PORTB |= 0x08;
    }
    
    if(power_state) {
      for(unsigned char i=0; i<LED_COUNT; i++) {
        pixel[i] = (rgb_color){15,15,15};
      }
    } else {
      for(unsigned char i=0; i<LED_COUNT; i++) {
        pixel[i] = (rgb_color){0,0,0};
      }
    }
    
    cli();
    led_strip_write(pixel, LED_COUNT);
    sei();
    
    if((TWSR & 0xF8) == 0x00) { // if I2C is error condition
      TWCR &= ~0x10; // clear TWSTO to escape I2C error
      TWAR = (my_address << 1) | 1; // address set + general call enable
    }
    WDT_reset();
    _delay_ms(1);
  }
}

void pin_init(void) {

  DDRB  = 0x3F;	// 0b00111111 // (XTAL2), (XTAL1), led3, led2, led1, key_v5, key_v4, key_v3
  PORTB = 0x3F;	// 0b00111111 // high output
  
  DDRC  = 0x02;	// 0b00000010 // ---, reset, SCL, SDA, jp1(2), jp2(1), led_data, ---
  PORTC = 0x0C;	// 0b00001100 // pull-up
  
  DDRD  = 0xC0;	// 0b11000000 // key_v2, key_v1, key_h4, key_h3, key_h2, key_h1, ---, ---
  PORTD = 0xFC;	// 0b11111100 // pull-up
}

unsigned char get_jp_state(void) {              // 0x00 ~ 0x03
  
  unsigned char temp = 0;
  
  temp += (PINC & 0x0C) >> 2;
  
  return temp;
}

void get_key_state(void) {

  unsigned char i;

  for(i=0; i<4; i++) {
    key_temp[i] = 0;
  }
  
  PORTD &= ~0x40;                               // key_v1
  get_key_state_h(0x10);
  PORTD |= 0x40;
  
  PORTD &= ~0x80;                               // key_v2
  get_key_state_h(0x08);
  PORTD |= 0x80;
  
  PORTB &= ~0x01;                               // key_v3
  get_key_state_h(0x04);
  PORTB |= 0x01;
  
  PORTB &= ~0x02;                               // key_v4
  get_key_state_h(0x02);
  PORTB |= 0x02;
  
  PORTB &= ~0x04;                               // key_v5
  get_key_state_h(0x01);
  PORTB |= 0x04;
  
  for(i=0; i<4; i++) {           // save key state
    key_state[i] = key_temp[i];
  }
}

void get_key_state_h(unsigned char mask) {

  key_temp[0] |= ((PIND & 0x04) ? 0x00 : mask);     // key_h1 check
  key_temp[1] |= ((PIND & 0x08) ? 0x00 : mask);     // key_h2 check
  key_temp[2] |= ((PIND & 0x10) ? 0x00 : mask);     // key_h3 check
  key_temp[3] |= ((PIND & 0x20) ? 0x00 : mask);     // key_h4 check
}