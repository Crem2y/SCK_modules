// fuse bit : 0xCF 0xEF
// high 0b11001111
// low  0b11101111

// for SCK_PM_V1.1.x Board

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LED_PRT PORTC
#define LED_DDR DDRC
#define LED_PIN 1

#include "led_sk6812.h"

#define LED_COUNT 17
rgbw_color pixel[LED_COUNT];

void pin_init(void);
unsigned char get_jp_state(void);
void get_key_state(void);
void watchdog_enable(void);
void watchdog_disable(void);
void watchdog_reset(void);
void I2C_slave_init(unsigned char address);

volatile unsigned char i2c_data = 0;
volatile unsigned char general_data = 0x00; // general call data (power, none, none, none, none, scroll_lock, caps_lock, num_lock)
volatile bool power_state = true; // led on/off
volatile unsigned char key_state[4] = {0x00, 0x00, 0x00, 0x00}; // key line H1, H2, H3, H4 {---, ---, ---, key_v1, key_v2, key_v3, key_v4, key_v5}
volatile unsigned char key_sending = 0;
  
bool wdt_flag = false; // watchdog timer flag
 
ISR(TWI_vect) {
  
  switch(TWSR & 0xF8) {
    case 0x60:  // Own SLA+W has been received, ACK has been returned
    
    break;
    case 0x70:  // General call address has been received, ACK has been returned
    
    break;
    case 0x80:  // Previously addressed with own SLA+W, data has been received, ACK has been returned
      i2c_data = TWDR;
      power_state = i2c_data & 0x80;
    break;
    case 0x90:  // Previously addressed with general call, data has been received, ACK has been returned
      general_data = TWDR;
    break;
    case 0xA0:	// A STOP condition or repeated START condition has been received while still addressed as Slave
      key_sending = 0;
    break;
    case 0xA8:	// Own SLA+R has been received, ACK has been returned
      TWDR = key_state[key_sending];
    break;
    case 0xB8:  // Data byte in TWDR has been transmitted, ACK has been received
      key_state[key_sending] = 0;
      key_sending++;
      if(key_sending == 3) {
        key_sending = 0;
      } else {
        TWDR = key_state[key_sending];
      }        
    break;
    case 0xC0:  // Data byte in TWDR has been transmitted, NOT ACK has been received
      TWDR = key_state[key_sending];
    break;
  }
}

int main(void) {
  
  pin_init();
  I2C_slave_init(0x18 + get_jp_state());
  
  //watchdog_enable();
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
    
    watchdog_reset();
  }
}

void pin_init(void) {
#error 수정 필요
  DDRB  = 0x07;	// 0b00000111 // ---, ---, ---, ---, ---, key_v5, key_v4, key_v3
  PORTB = 0x07;	// 0b00000111 // high output
  
  DDRC  = 0x02;	// 0b00000010 // ---, reset, SCL, SDA, jp1(2), jp2(1), led_data, ---
  PORTC = 0x0C;	// 0b00001100 // pull-up
  
  DDRD  = 0xC0;	// 0b11000000 // key_v2, key_v1, ---, key_h3, key_h2, key_h1, ---, ---
  PORTD = 0xDC;	// 0b11011100 // pull-up
}

unsigned char get_jp_state(void) {              // 0x00 ~ 0x03
  
  unsigned char temp = 0;
  
  temp += (PINC & 0x0C) >> 2;
  
  return temp;
}

void get_key_state(void) {
#error 수정 필요
  unsigned char temp[3] = {0,};
  
  PORTD &= ~0x40;                               // key_v1 low output
  temp[0] += ((PIND & 0x04) ? 0x00 : 0x10);     // key_h1 check
  temp[1] += ((PIND & 0x08) ? 0x00 : 0x10);     // key_h2 check
  temp[2] += ((PIND & 0x10) ? 0x00 : 0x10);     // key_h3 check
  PORTD |= 0x40;                                // key_v1 high output
  
  PORTD &= ~0x80;                               // key_v2 low output
  temp[0] += ((PIND & 0x04) ? 0x00 : 0x08);
  temp[1] += ((PIND & 0x08) ? 0x00 : 0x08);
  temp[2] += ((PIND & 0x10) ? 0x00 : 0x08);
  PORTD |= 0x80;                                // key_v2 high output
  
  PORTB &= ~0x01;                               // key_v3 low output
  temp[0] += ((PIND & 0x04) ? 0x00 : 0x04);
  temp[1] += ((PIND & 0x08) ? 0x00 : 0x04);
  temp[2] += ((PIND & 0x10) ? 0x00 : 0x04);
  PORTB |= 0x01;                                // key_v3 high output
  
  PORTB &= ~0x02;                               // key_v4 low output
  temp[0] += ((PIND & 0x04) ? 0x00 : 0x02);
  temp[1] += ((PIND & 0x08) ? 0x00 : 0x02);
  temp[2] += ((PIND & 0x10) ? 0x00 : 0x02);
  PORTB |= 0x02;                                // key_v4 high output
  
  PORTB &= ~0x04;                               // key_v5 low output
  temp[0] += ((PIND & 0x04) ? 0x00 : 0x01);
  temp[1] += ((PIND & 0x08) ? 0x00 : 0x01);
  temp[2] += ((PIND & 0x10) ? 0x00 : 0x01);
  PORTB |= 0x04;                                // key_v5 high output
  
  key_state[0] = temp[0];                       // save key state
  key_state[1] = temp[1];
  key_state[2] = temp[2];
}

void watchdog_enable(void) {  // enable watchdog timer @ 0.11s
  
  WDTCR = 0x18;
  WDTCR = 0x1B;
  wdt_flag = true;
}

void watchdog_disable(void) {
  
  WDTCR = 0x18;
  WDTCR = 0x00;
  wdt_flag = false;
}

void watchdog_reset(void) {
  
  if(wdt_flag) {
    asm volatile(" WDR ");
  }
}

void I2C_slave_init(unsigned char address) {

  TWAR = address << 1; // address set
  TWCR = 0x45; // 0b01000101, I2C interrupt set
}
