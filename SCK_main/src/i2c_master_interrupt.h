#pragma once

#include "i2c_status_code.h"

volatile bool I2C_is_initalized = false;           // I2C is initalized
volatile bool I2C_is_communicating = false;        // I2C is communicating

volatile unsigned char* I2C_read_buf;  // I2C reading buffer pointer
volatile unsigned char* I2C_write_buf;  // I2C writing buffer pointer

volatile unsigned char I2C_target_address = 0x00;  // I2C target slave address
volatile unsigned char I2C_mode = 0;               // I2C mode (0 : write , 1 : read)
volatile unsigned char I2C_data_size = 0;          // I2C data size
volatile unsigned char I2C_data_pos = 0;           // I2C data pos
volatile unsigned char I2C_err_count = 0;          // I2C error count

bool I2C_init(void);
bool I2C_deinit(void);
bool I2C_force_deinit(void);
bool I2C_data_clear(void);
bool I2C_wait(void);
bool I2C_check(unsigned char address, void* buf);
bool I2C_read_byte(unsigned char address, void* buf);
bool I2C_read_data(unsigned char address, void* buf, unsigned char length);
bool I2C_write_byte(unsigned char address, void* buf);
bool I2C_write_data(unsigned char address, void* buf, unsigned char length);

//////////////////////////////// functions ////////////////////////////////

/**
 * @brief AVR Interrupt Service Routine (TWI)
 * 
 */
ISR(TWI_vect) {
  //Serial.println(I2C_SC, HEX);
  if(!I2C_is_communicating) {
    TWCR = 0x85; // clear TWINT
    return;
  }

  switch(I2C_SC) {
    case I2C_SC_MT_SC:
      TWDR = (I2C_target_address << 1) | I2C_mode;
      TWCR = 0x85; // clear TWINT, clear TWSTA
    break;
    case I2C_SC_MT_RSC:
      TWCR = 0x85; // clear TWINT
      I2C_is_communicating = false;
      // repeated start unused
    break;
    case I2C_SC_MT_SWA:
      TWDR = *I2C_write_buf;
      TWCR = 0x85; // clear TWINT
    break;
    case I2C_SC_MT_SWN:
      I2C_err_count++;
      I2C_is_communicating = false;
      TWCR = 0x95; // clear TWINT, STOP Condition
    break;
    case I2C_SC_MT_DBA:
      I2C_data_pos++;
      I2C_write_buf++;
      if(I2C_data_pos == I2C_data_size) { // if it was last byte
        I2C_is_communicating = false;
        TWCR = 0x95; // clear TWINT, STOP Condition
      } else {
        TWDR = *I2C_write_buf;
        TWCR = 0x85; // clear TWINT
      }
    break;
    case I2C_SC_MT_DBN:
      I2C_is_communicating = false;
      TWCR = 0x95; // clear TWINT, STOP Condition
    break;
    case I2C_SC_MT_AL:
      TWCR = 0x85; // clear TWINT
#if MODULE_CONFIG == MODULE_SUB
      I2C_force_deinit();
#endif
      I2C_err_count++;
      I2C_is_communicating = false;
    break;
    case I2C_SC_MR_SRA:
      if(I2C_data_size == 1) { // if next is last byte
        TWCR = 0x85; // clear TWINT, NOT ACK
      } else {
        TWCR = 0xC5; // clear TWINT, ACK
      }
    break;
    case I2C_SC_MR_SRN:
      I2C_err_count++;
      I2C_is_communicating = false;
      TWCR = 0x95; // clear TWINT, STOP Condition
    break;
    case I2C_SC_MR_DBA:
      *I2C_read_buf = TWDR;
      I2C_data_pos++;
      I2C_read_buf++;
      if(I2C_data_pos == I2C_data_size) { // if it was last byte
        I2C_is_communicating = false;
        TWCR = 0x95; // clear TWINT, STOP Condition
      } else if (I2C_data_pos + 1 == I2C_data_size) { // if next is last byte
        TWCR = 0x85; // clear TWINT, NOT ACK
      } else {
        TWCR = 0xC5; // clear TWINT, ACK
      }
    break;
    case I2C_SC_MR_DBN:
      I2C_is_communicating = false;
      *I2C_read_buf = TWDR;
      TWCR = 0x95; // clear TWINT, STOP Condition
      
    break;
    case I2C_SC_ER_ERR:
      I2C_err_count++;
      I2C_is_communicating = false;
      TWCR = 0x85; // clear TWINT
    break;
    case I2C_SC_ER_NA:
    break;
  }

  TWCR |= 0x85; // clear TWINT
}

/**
 * @brief initalize I2C port to master, call sei() to use I2C.
 *        clock : 400kHz
 * @return true
 * @return false 
 */
bool I2C_init(void) {
  if (I2C_is_initalized) return false;

  TWBR = 12;
  TWSR = 0x00;
  TWCR = 0x85;  // I2C enable, interrupt enable

  I2C_target_address = 0x00;
  I2C_data_pos = 0;
  I2C_data_size = 0;
  
  I2C_is_communicating = false;
  I2C_is_initalized = true;

  return true;
}

/**
 * @brief deinitalize I2C port
 * 
 * @return true 
 * @return false 
 */
bool I2C_deinit(void) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;

  TWCR = 0x00;
  TWBR = 0x00;
  TWSR = 0x00;
  
  I2C_is_initalized = false;
  return true;
}

/**
 * @brief force deinitalize I2C port
 * 
 * @return true 
 * @return false 
 */
bool I2C_force_deinit(void) {

  TWCR = 0x00;
  TWBR = 0x00;
  TWSR = 0x00;
  
  I2C_is_initalized = false;
  return true;
}

/**
 * @brief wait until communication end.
 */
bool I2C_wait(void) {
  while(I2C_is_communicating);
  return true;
}

/**
 * @brief Check the slave of the entered address. Pause until a response is received or a certain number of times are reached.
 * 
 * @param address I2C slave address
 * @param timeout communication timeout (ms)
 * @return true 
 * @return false 
 */
bool I2C_check(unsigned char address, void* buf, unsigned short timeout) {
  if(!I2C_read_byte(address, &buf)) return false;
  bool timeouted = true;
  for(unsigned short i=0; i<timeout*100; i++) {
    if(!I2C_is_communicating) {
      timeouted = false;
      break;
    }
    delayMicroseconds(10);
  }
  
  if(timeouted) {
    return false;
  } else if(I2C_err_count == 0) {
    return true;
  }
  return false;
}

/**
 * @brief Reads the byte of the entered address.
 * 
 * @param address I2C slave address
 * @return true 
 * @return false 
 */
bool I2C_read_byte(unsigned char address, void* buf) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_mode = 1;
  I2C_read_buf = buf;
  I2C_data_pos = 0;
  I2C_data_size = 1;
  I2C_err_count = 0;
  I2C_is_communicating = true;

  TWCR = 0xA5; // clear TWINT & START Condition

  return true;
}

/**
 * @brief Reads the bytes of the entered address.
 * 
 * @param address I2C slave address
 * @param length data length
 * @return true 
 * @return false 
 */
bool I2C_read_data(unsigned char address, void* buf, unsigned char length) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_mode = 1;
  I2C_read_buf = buf;
  I2C_data_pos = 0;
  I2C_data_size = length;
  I2C_err_count = 0;
  I2C_is_communicating = true;

  TWCR = 0xA5; // clear TWINT & START Condition

  return true;
}

/**
 * @brief Writes the byte of the entered address.
 * 
 * @param address I2C slave address
 * @return true 
 * @return false 
 */
bool I2C_write_byte(unsigned char address, void* buf) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_mode = 0;
  I2C_write_buf = buf;
  I2C_data_pos = 0;
  I2C_data_size = 1;
  I2C_err_count = 0;
  I2C_is_communicating = true;
  
  TWCR = 0xA5; // clear TWINT & START Condition

  return true;
}

/**
 * @brief Writes the bytes of the entered address.
 * 
 * @param address I2C slave address
 * @param length data length
 * @return true 
 * @return false 
 */
bool I2C_write_data(unsigned char address, void* buf, unsigned char length) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_mode = 0;
  I2C_write_buf = buf;
  I2C_data_pos = 0;
  I2C_data_size = length;
  I2C_err_count = 0;
  I2C_is_communicating = true;
  
  TWCR = 0xA5; // clear TWINT & START Condition

  return true;
}