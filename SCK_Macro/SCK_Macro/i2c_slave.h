#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "i2c_status_code.h"

void I2C_init_slave(unsigned char address); // init I2C to slave

volatile unsigned char I2C_data = 0;
volatile unsigned char I2C_general_data = 0x00; // general call data (power, ---, ---, ---, ---, scroll_lock, caps_lock, num_lock)
volatile bool power_state = false; // led on/off
volatile unsigned char key_state = 0x00; // rsw_cw, rsw_ccw, rsw_sw, key5, key4, key3, key2, key1

ISR(TWI_vect) {

  switch(I2C_SC) {
    case I2C_SC_SR_SWA:
    break;
    case I2C_SC_SR_GNC:
    break;
    case I2C_SC_SR_DBA:
      I2C_data = TWDR;
      power_state = I2C_data & 0x80;
    break;
    case I2C_SC_SR_DBN:
      I2C_data = TWDR;
      power_state = I2C_data & 0x80;
    break;
    case I2C_SC_SR_GNA:
      I2C_general_data = TWDR;
      power_state = I2C_general_data & 0x80;
    break;
    case I2C_SC_SR_GNN:
      I2C_general_data = TWDR;
      power_state = I2C_general_data & 0x80;
    break;
    case I2C_SC_SR_STO:
      TWCR = 0xC5; // clear TWINT, ACK
    break;
    case I2C_SC_ST_SRA:
      key_state += rotery_check() << 6;
      TWDR = key_state;
      //TWCR = 0x85; // clear TWINT, NOT ACK
      TWCR = 0xC5; // clear TWINT, ACK
    break;
    case I2C_SC_ST_DBA:
      key_state = 0x00;
    break;
    case I2C_SC_ST_DBN:
      key_state = 0x00;
      TWCR = 0xC5; // clear TWINT, ACK
    break;
    case I2C_SC_ST_ARA:
    break;
    case I2C_SC_SR_AWA:
    break;
    case I2C_SC_SR_AGA:
      // Arbitration unused
    break;
    case I2C_SC_ER_ERR:
    break;
    case I2C_SC_ER_NA:
    break;
  }
  
  TWCR |= 0x80; //clear TWINT
}

void I2C_init_slave(unsigned char address) { // set I2C to slave

  TWAR = (address << 1) | 1; // address set + general call enable
  TWCR = 0xC5; // 0b11000101, I2C interrupt set, ACK
}