// fuse bit : 0xCF 0xEF
// high 0b11001111
// low  0b11101111

// for SCK_MM_V1.x.x Board

// DO NOT PUSH ANY KEY WHILE DOWNLOADING!!!

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "led_sk6812.h"
#include "rotery_sw.h"
#include "i2c_status_code.h"
#include "i2c_slave.h"
#include "watchdog.h"

#define LED_COUNT 5
rgbw_color pixel[LED_COUNT];

void pin_init(void);
unsigned char get_jp_state(void);               // 0x00 ~ 0x0F
void get_key_state(void);                       // 0x00 ~ 0x7F or 0xBF


int main(void) {
  
  pin_init();
  rotery_init();
  I2C_init_slave(0x20 + get_jp_state());

  pixel[0] = (rgbw_color){16,16,16,16};
  led_strip_write(pixel, LED_COUNT);
  _delay_ms(1000);
  
  for(unsigned char i=1; i<LED_COUNT; i++) {
    pixel[i] = (rgbw_color){16,16,16,16};
    pixel[i-1] = (rgbw_color){0,0,0,0};
    led_strip_write(pixel, LED_COUNT);
    _delay_ms(1000);
  }
  pixel[LED_COUNT-1] = (rgbw_color){0,0,0,0};
  led_strip_write(pixel, LED_COUNT);
  
  TWCR |= 0x80; //clear TWINT
  WDT_enable();
  sei();
  
  while(1) {
    get_key_state();
    
    if(power_state) {
      for(unsigned char i=0; i<LED_COUNT; i++) {
        pixel[i] = (rgbw_color){16,16,16,16};
      }
    } else {
      for(unsigned char i=0; i<LED_COUNT; i++) {
        pixel[i] = (rgbw_color){0,0,0,0};
      }
    }
    
    cli();
    led_strip_write(pixel, LED_COUNT);
    sei();
    if((TWSR & 0xF8) == 0x00) { // if I2C is error condition
      TWCR &= ~0x10; // clear TWSTO to escape I2C error
    }    
    WDT_reset();
    _delay_ms(1);
  }
}

void pin_init(void) {
  
  DDRB  = 0x01;	// 0b 0000 0001 // (XTAL2), (XTAL1), key5, key4, key3, key2, key1, led_data
  PORTB = 0x3E;	// 0b 0011 1110 // pull-up set
  
  DDRC  = 0x00;	// 0b 0000 0000 // ---, reset, SCL, SDA, jp1(8), jp2(4), jp3(2), jp4(1)
  PORTC = 0x0F;	// 0b 0000 1111 // pull-up set
  
  DDRD  = 0x00;	// 0b 0000 0000 // ---, ---, ---, rsw_sw, rsw_b, rsw_a, (TXD), (RXD)
  PORTD = 0x1C;	// 0b 0001 1100 // pull-up set
}

unsigned char get_jp_state(void) {              // 0x00 ~ 0x0F

  return ~PINC & 0x0F;
}

void get_key_state(void) {                      // 0x00 ~ 0x7F or 0xBF
  
  unsigned char temp = 0;
  
  temp += (~PINB & 0x3E) >> 1;                  // key 5~1
  temp += (~PIND & 0x10) << 1;                  // rsw_sw

  key_state = temp;
}