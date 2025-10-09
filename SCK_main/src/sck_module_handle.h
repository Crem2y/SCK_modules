#pragma once

#include <HID-Project.h>

#include "i2c_master_interrupt.h"
#include "sck_key_handle.h"
#include "sck_key_define.h"
#include "user_datas/key_data.h"
#include "macro_timer.h"

unsigned char SCK_KM_address = 0x10; // 0x10 ~ 0x17
unsigned char SCK_PM_address = 0x18; // 0x18 ~ 0x1B
unsigned char SCK_FM_address = 0x1C; // 0x1C ~ 0x1F
unsigned char SCK_MM_addresses[MM_H] = {0x20,0x21,0x22,0x23,0x28}; // 0x20 ~ 0x2F

unsigned char SCK_KM_count = 0;
unsigned char SCK_PM_count = 0;
unsigned char SCK_FM_count = 0;
unsigned char SCK_MM_count = 0;

bool SCK_led_power = true;
byte SCK_lock_key;

volatile unsigned short msCount = 0; // timer count

void SCK_init(void);
void SCK_loop(void);

void keyCheck_KM(byte key_state, byte keyposV, byte keyposH, byte key_layer);
void keyCheck_PM(byte key_state, byte keyposV, byte keyposH, byte key_layer);
void keyCheck_FM(byte key_state, byte keyposV, byte keyposH, byte key_layer);
void keyCheck_MM(byte key_state, byte keyposV, byte module_num, byte key_layer);
void keyRepeat_MM(byte key_state, byte keyposV, byte module_num, byte key_layer);
void keyToggle_MM(byte key_state, byte keyposV, byte module_num, byte key_layer);
void toggleRepeat_MM(byte key_state, byte keyposV, byte module_num, byte key_layer);

//////////////////////////////// functions ////////////////////////////////

/**
 * @brief AVR Interrupt Service Routine (TIMER3_COMPA)
 * timer int (1kHz = 1ms)
 */
ISR(TIMER3_COMPA_vect) {
  msCount++;
  if (msCount < repeatSpeed) {
    return;
  }

  byte i,j;
  unsigned short mode_data;

  for(i=0; i<MM_H; i++) { // key checking
    mode_data = (SCK_MM_keyset[SCK_key_layer][0][i] << 8) + SCK_MM_keyset[SCK_key_layer][1][i];

    for(j=0; j<MM_V; j++) {
      if (mode_data & (0x4000 >> (j*2))) { // if repeat mode
        if ((mode_data & (0x8000 >> (j*2))) && SCK_MM_toggled[j][i]) { // if toggle mode & toggle flag on
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][2+j][i], true); // click a key
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][2+j][i], false);
        } else if (SCK_MM_pressed[j][i]) {
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][2+j][i], true); // click a key
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][2+j][i], false);
        }
      }
    }
  }

  msCount = 0;
}

/**
 * @brief SCK initalize
 * 
 */
void SCK_init(void) {
  byte i;
  byte temp;

  // module check
  if(!I2C_init()) {
    Serial.println(F("[SCK] i2c init error!"));
    while(1);
  }

  sei();

  Serial.println(F("[SCK] i2c slave scanning..."));
  for(i=0x10; i<0x30; i++) {
    if(I2C_check(i, temp, 10)) {
      Serial.print("[SCK] 0x");
      Serial.print(i, HEX);
      Serial.print(F(" ACK! data : 0x"));
      if(temp < 0x10) Serial.print('0');
      Serial.println(temp, HEX);

      if(i > 0x0F && i < 0x18) {
        SCK_KM_address = i;
        SCK_KM_count++;
      } else if(i > 0x17 && i < 0x1C) {
        SCK_PM_address = i; 
        SCK_PM_count++;
      } else if(i > 0x1B && i < 0x20) {
        SCK_FM_address = i;
        SCK_FM_count++;
      } else if(i > 0x1F && i < 0x30) {
        //SCK_MM_addresses[SCK_MM_count] = i;
        SCK_MM_count++;
      }
    }
    delay(1);
  }
  Serial.println(F("[SCK] all slaves checked!\n"));
  // module check end

  Serial.print(F("[SCK] keyboard modules : "));
  Serial.println(SCK_KM_count);
  Serial.print(F("[SCK] keypad modules : "));
  Serial.println(SCK_PM_count);
  Serial.print(F("[SCK] fnkey modules : "));
  Serial.println(SCK_FM_count);
  Serial.print(F("[SCK] macro modules : "));
  Serial.println(SCK_MM_count);

  if(SCK_KM_count == 0) {
    Serial.println(F("[SCK] keyboard module error!"));
  }

  Serial.print(F("\n[SCK] default mouseSpeed  : "));
  Serial.println(mouseSpeed);
  Serial.print(F("[SCK] default wheelSpeed  : "));
  Serial.println(wheelSpeed);
  Serial.print(F("[SCK] default repeatSpeed : "));
  Serial.println(repeatSpeed);

  SCK_led_func_init();
  SCK_user_func_init();

  Serial.println(F("[SCK] keyboard start!"));
  timer_init();
}

/**
 * @brief SCK loop function
 * 
 */
void SCK_loop(void) {
  byte i, j;
  byte key_mask;
  byte key_state;

  I2C_wait();

  // getting key
  if(SCK_KM_count) { // if there is keyboard modules
    I2C_read_data(SCK_KM_address, SCK_KM_buf, KM_H);
    while(I2C_is_communicating);
    if(!I2C_err_count) {
      for(i=0; i<KM_H; i++) {
        key_mask = 0x20;
        for(j=0; j<KM_V; j++) {
          key_state = SCK_KM_buf[i] & key_mask;
          keyCheck_KM(key_state, j, i, SCK_key_layer);
          key_mask >>= 1;
        }
      }
    }
    
  }

  if(true) { //if(SCK_FM_count) { // if there is fnkey modules
    I2C_read_data(SCK_FM_address, SCK_FM_buf, FM_H);
    while(I2C_is_communicating);
    if(!I2C_err_count) {
      for(i=0; i<FM_H; i++) {
        key_mask = 0x10;
        for(j=0; j<FM_V; j++) {
          key_state = SCK_FM_buf[i] & key_mask;
          keyCheck_FM(key_state, j, i, SCK_key_layer);
          key_mask >>= 1;
        }
      }
    }
  }
  
  if(true) { //if(SCK_PM_count) { // if there is keypad modules
    I2C_read_data(SCK_PM_address, SCK_PM_buf, PM_H);
    while(I2C_is_communicating);
    if(!I2C_err_count) {
      for(i=0; i<PM_H; i++) {
        key_mask = 0x10;
        for(j=0; j<PM_V; j++) {
          key_state = SCK_PM_buf[i] & key_mask;
          keyCheck_PM(key_state, j, i, SCK_key_layer);
          key_mask >>= 1;
        }
      }
    }
  }

  if(true) { //if(SCK_MM_count) { // if there is macro modules
    for(i=0; i<MM_H; i++) {
      I2C_read_byte(SCK_MM_addresses[i], SCK_MM_buf);
      while(I2C_is_communicating);
      if(I2C_err_count) continue;

      key_mask = 0x80;
      unsigned short mode_data = (SCK_MM_keyset[SCK_key_layer][0][i] << 8) + SCK_MM_keyset[SCK_key_layer][1][i];
      for(j=0; j<MM_V; j++) {
        key_state = SCK_MM_buf[0] & key_mask;
        if (mode_data & (0x4000 >> (j*2))) { // if repeat mode
          if (mode_data & (0x8000 >> (j*2))) { // if toggle mode
            toggleRepeat_MM(key_state, j, i, SCK_key_layer);
          } else {
            keyRepeat_MM(key_state, j, i, SCK_key_layer);
          }
        } else {
          if (mode_data & (0x8000 >> (j*2))) { // if toggle mode
            keyToggle_MM(key_state, j, i, SCK_key_layer);
          } else {
            keyCheck_MM(key_state, j, i, SCK_key_layer);
          }
        }
        key_mask >>= 1;
      }
    }
  }
  // getting key end

  SCK_lock_key = BootKeyboard.getLeds(); // lock key checking

  // general call data (power, ---, ---, ---, ---, scroll_lock, caps_lock, num_lock)
  //I2C_writing_data[0] = (SCK_led_power << 7) | (SCK_lock_key & 0x07);
  //I2C_write_byte(I2C_GCA);
}

/**
 * @brief check and press a key once
 * 
 * @param key_state byte, if 0, key is pressed
 * @param keyposV byte, 0 ~ FM_V
 * @param keyposH byte, 0 ~ FM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyCheck_KM(byte key_state, byte keyposV, byte keyposH, byte key_layer) {
  if (key_state) { // if pressed
    if (!SCK_KM_pressed[keyposV][keyposH]) { // if first detacted
      SCK_keyHandle(SCK_KM_keyset[key_layer][keyposV][keyposH], true); // press a key
      SCK_KM_pressed[keyposV][keyposH] = true;
    }
  } else if (SCK_KM_pressed[keyposV][keyposH]) { // if key released
    SCK_keyHandle(SCK_KM_keyset[key_layer][keyposV][keyposH], false); // release a key
    SCK_KM_pressed[keyposV][keyposH] = false;
  }
}

/**
 * @brief check and press a key once
 * 
 * @param key_state byte, if 0, key is pressed
 * @param keyposV byte, 0 ~ FM_V
 * @param keyposH byte, 0 ~ FM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyCheck_PM(byte key_state, byte keyposV, byte keyposH, byte key_layer) {
  if (key_state) { // if pressed
    if (!SCK_PM_pressed[keyposV][keyposH]) { // if first detacted
      SCK_keyHandle(SCK_PM_keyset[key_layer][keyposV][keyposH], true); // press a key
      SCK_PM_pressed[keyposV][keyposH] = true;
    }
  } else if (SCK_PM_pressed[keyposV][keyposH]) { // if key released
    SCK_keyHandle(SCK_PM_keyset[key_layer][keyposV][keyposH], false); // release a key
    SCK_PM_pressed[keyposV][keyposH] = false;
  }
}

/**
 * @brief check and press a key once
 * 
 * @param key_state byte, if 0, key is pressed
 * @param keyposV byte, 0 ~ FM_V
 * @param keyposH byte, 0 ~ FM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyCheck_FM(byte key_state, byte keyposV, byte keyposH, byte key_layer) {
  if (key_state) { // if pressed
    if (!SCK_FM_pressed[keyposV][keyposH]) { // if first detacted
      SCK_keyHandle(SCK_FM_keyset[key_layer][keyposV][keyposH], true); // press a key
      SCK_FM_pressed[keyposV][keyposH] = true;
    }
  } else if (SCK_FM_pressed[keyposV][keyposH]) { // if key released
    SCK_keyHandle(SCK_FM_keyset[key_layer][keyposV][keyposH], false); // release a key
    SCK_FM_pressed[keyposV][keyposH] = false;
  }
}

/**
 * @brief check and press a key once
 * 
 * @param key_state byte, if 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param module_num byte, 0 ~ MM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyCheck_MM(byte key_state, byte keyposV, byte module_num, byte key_layer) {
  if (key_state) { // if pressed
    if (!SCK_MM_pressed[keyposV][module_num]) { // if first detacted
      SCK_keyHandle(SCK_MM_keyset[key_layer][2 + keyposV][module_num], true); // press a key
      SCK_MM_pressed[keyposV][module_num] = true;
    }
  } else if (SCK_MM_pressed[keyposV][module_num]) { // if key released
    SCK_keyHandle(SCK_MM_keyset[key_layer][2 + keyposV][module_num], false); // release a key
    SCK_MM_pressed[keyposV][module_num] = false;
  }
}

/**
 * @brief click a key
 * 
 * @param key_state byte, if not 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param module_num byte, 0 ~ MM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyRepeat_MM(byte key_state, byte keyposV, byte module_num, byte key_layer) {
  if (key_state) { // if pressed
    SCK_MM_pressed[keyposV][module_num] = true;
  } else {
    SCK_MM_pressed[keyposV][module_num] = false;
  }
}

/**
 * @brief check and toggle key
 * 
 * @param key_state byte, if not 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param module_num byte, 0 ~ MM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyToggle_MM(byte key_state, byte keyposV, byte module_num, byte key_layer) {
  bool TK = SCK_MM_toggled[keyposV][module_num]; // previous state

  if (key_state) { // if pressed
    if (!SCK_MM_pressed[keyposV][module_num]) { // if first detacted
      SCK_MM_toggled[keyposV][module_num] = !TK; // toggle state
      SCK_keyHandle(SCK_MM_keyset[key_layer][2 + keyposV][module_num], !TK); 
      SCK_MM_pressed[keyposV][module_num] = true;
    }
  } else { // if not pressed
    SCK_MM_pressed[keyposV][module_num] = false;
  }
}

/**
 * @brief toggle & click a key
 * 
 * @param key_state byte, if not 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param module_num byte, 0 ~ MM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void toggleRepeat_MM(byte key_state, byte keyposV, byte module_num, byte key_layer) {
  bool TK = SCK_MM_toggled[keyposV][module_num]; // previous state

  if (key_state) { // if pressed
    if (!SCK_MM_pressed[keyposV][module_num]) { // if first detacted
      SCK_MM_toggled[keyposV][module_num] = !TK; // toggle state
      SCK_MM_pressed[keyposV][module_num] = true;
    }
  } else { // if not pressed
    SCK_MM_pressed[keyposV][module_num] = false;
  }
}