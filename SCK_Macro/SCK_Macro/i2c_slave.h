#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "i2c_status_code.h"

#define I2C_GENERAL_BYTES_MAX 32          // I2C max reading bytes (1 ~ 255)
#define I2C_READING_BYTES_MAX 32          // I2C max reading bytes (1 ~ 255)
#define I2C_WRITING_BYTES_MAX 32          // I2C max writing bytes (1 ~ 255)

void I2C_init_slave(unsigned char address); // init I2C to slave

// general call data (power, ---, ---, ---, ---, scroll_lock, caps_lock, num_lock)
volatile unsigned char I2C_general_data[I2C_READING_BYTES_MAX] = {0,};  // I2C general call data
volatile unsigned char I2C_reading_data[I2C_READING_BYTES_MAX] = {0,};  // I2C reading data
volatile unsigned char I2C_writing_data[I2C_WRITING_BYTES_MAX] = {0,};  // I2C writing data


ISR(TWI_vect) {

  switch(I2C_SC) {
    case I2C_SC_SR_SWA:
    break;
    case I2C_SC_SR_GNC:
    break;
    case I2C_SC_SR_DBA:
      I2C_reading_data[0] = TWDR;
    break;
    case I2C_SC_SR_DBN:
      I2C_reading_data[0] = TWDR;
    break;
    case I2C_SC_SR_GNA:
      I2C_general_data[0] = TWDR;
    break;
    case I2C_SC_SR_GNN:
      I2C_general_data[0] = TWDR;
    break;
    case I2C_SC_SR_STO:
      TWCR = 0xC5; // clear TWINT, ACK
    break;
    case I2C_SC_ST_SRA:
      I2C_writing_data[0] += rotery_check() << 6;
      TWDR = I2C_writing_data[0];
      //TWCR = 0x85; // clear TWINT, NOT ACK
      TWCR = 0xC5; // clear TWINT, ACK
    break;
    case I2C_SC_ST_DBA:
      I2C_reading_data[0] = 0x00;
    break;
    case I2C_SC_ST_DBN:
      I2C_reading_data[0] = 0x00;
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