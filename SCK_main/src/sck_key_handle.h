#pragma once
#include <HID-Project.h>
#include <avr/wdt.h>

#include "sck_key_code.h"
#include "sck_functiuon_set.h"

// limit values
#define MS_MAX 120 // mouseSpeed max value (~ 127)
#define MS_MIN 1   // mouseSpeed min value (0 ~)
#define MS_CNG 0.2   // mouseSpeed change value (1 ~ MS_MIN)
#define MS_DEF 1   // mouseSpeed default value (MS_MAX ~ MS_MIN)

#define WS_MAX 20  // wheelSpeed max value (~ 127)
#define WS_MIN 1   // wheelSpeed min value (0 ~)
#define WS_CNG 1   // wheelSpeed change value (1 ~ WS_MIN)
#define WS_DEF 2   // wheelSpeed default value (WS_MAX ~ WS_MIN)

#define RS_MAX 500 // repeatSpeed max value (~ 65535)
#define RS_MIN 10  // repeatSpeed min value (1 ~)
#define RS_CNG 0.2   // repeatSpeed change value (1 ~ RS_MIN)
#define RS_DEF 10  // repeatSpeed default value (RS_MAX ~ RS_MIN)

signed   char  mouseSpeed  = MS_DEF; // mouse move speed  (-128 ~ 127)
signed   char  wheelSpeed  = WS_DEF; // mouse wheel speed (-128 ~ 127)
unsigned short repeatSpeed = RS_DEF; // delay value in repeat mode (1 ~ 65535ms)

float          mouseSpeed_f  = MS_DEF; // mouse move speed  (-128 ~ 127)
float          wheelSpeed_f  = WS_DEF; // mouse wheel speed (-128 ~ 127)
float          repeatSpeed_f = RS_DEF; // delay value in repeat mode (1 ~ 65535ms)

unsigned char SCK_key_layer = 0;

void SCK_keyHandle(unsigned char keycode, bool pressed);

void SCK_code_Normal(byte keycode, bool pressed);
void SCK_code_Function(byte keycode, bool pressed);
void SCK_code_Consumer(byte keycode, bool pressed);
void SCK_code_Surface(byte keycode, bool pressed);
void SCK_code_Debug(byte keycode, bool pressed);

//////////////////////////////// functions ////////////////////////////////

/**
 * @brief check keycode and execute correct function
 * 
 * @param keycode unsigned char, 0~255
 * @param pressed bool, if true, key is pressed
 */
void SCK_keyHandle(unsigned char keycode, bool pressed) {
  if (keycode < 0x20) {
    SCK_code_Normal(keycode, pressed);
  } else if (keycode > 0x87 && keycode < 0xB0) {
    SCK_code_Function(keycode, pressed);
  } else if (keycode > 0xB3 && keycode < 0xC1) {
    SCK_code_Consumer(keycode, pressed);
  } else if (keycode > 0xEB && keycode < 0xF0) {
    SCK_code_Surface(keycode, pressed);
  } else if (keycode > 0xFB) {
    SCK_code_Debug(keycode, pressed);
  } else {
    if (pressed) Keyboard.press(keycode);
    else Keyboard.release(keycode);
  }
}

/**
 * @brief special keycode for mouse, repeat speed, and user function
 * 
 * @param keycode unsigned char, 0x01 ~ 0x1F (31)
 * @param pressed bool, if true, key is pressed
 */
void SCK_code_Normal(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case M_LB: // left_mouse
        Mouse.press(MOUSE_LEFT);
      break;
      case M_RB: // right_mouse
        Mouse.press(MOUSE_RIGHT);
      break;
      case M_MB: // middle_mouse
        Mouse.press(MOUSE_MIDDLE);
      break;
      case M_4B: // mouse_button_4
        Mouse.press(MOUSE_PREV);
      break;
      case M_5B: // mouse_button_5
        Mouse.press(MOUSE_NEXT);
      break;
      case M_U: // mouse_up
        Mouse.move(0, -mouseSpeed, 0);
      break;
      case M_D: // mouse_down
        Mouse.move(0, mouseSpeed, 0);
      break;
      case M_L: // mouse_left
        Mouse.move(-mouseSpeed, 0, 0);
      break;
      case M_R: // mouse_right
        Mouse.move(mouseSpeed, 0, 0);
      break;
      case M_UL: // mouse_ul
        Mouse.move(-mouseSpeed, -mouseSpeed, 0);
      break;
      case M_UR: // mouse_ur
        Mouse.move(mouseSpeed, -mouseSpeed, 0);
      break;
      case M_DL: // mouse_dl
        Mouse.move(-mouseSpeed, mouseSpeed, 0);
      break;
      case M_DR: // mouse_dr
        Mouse.move(mouseSpeed, mouseSpeed, 0);
      break;
      case MH_U: // mouse_wheel_up
        Mouse.move(0, 0, wheelSpeed);
      break;
      case MH_D: // mouse_wheel_down
        Mouse.move(0, 0, -wheelSpeed);
      break;
      case M_F: // mouse_faster
        mouseSpeed_f = mouseSpeed_f + (1 + (mouseSpeed_f * MS_CNG));
        if (mouseSpeed_f > MS_MAX) mouseSpeed_f = MS_MAX;
        mouseSpeed = (unsigned short)mouseSpeed_f;
      break;
      case M_S: // mouse_slower
        mouseSpeed_f = mouseSpeed_f - (1 + (mouseSpeed_f * MS_CNG));
        if (mouseSpeed_f < MS_MIN) mouseSpeed_f = MS_MIN;
        mouseSpeed = (unsigned short)mouseSpeed_f;
      break;
      case MH_F: // mouse_wheel_faster
        if (wheelSpeed < WS_MAX) wheelSpeed += WS_CNG;
      break;
      case MH_S: // mouse_wheel_slower
        if (wheelSpeed > WS_MIN) wheelSpeed -= WS_CNG;
      break;
      case R_F: // repeat_faster
        repeatSpeed_f = repeatSpeed_f - (1 + (repeatSpeed_f * RS_CNG));
        if (repeatSpeed_f < RS_MIN) repeatSpeed_f = RS_MIN;
        repeatSpeed = (unsigned short)repeatSpeed_f;
      break;
      case R_S: // repeat_slower
        repeatSpeed_f = repeatSpeed_f + (1 + (repeatSpeed_f * RS_CNG));
        if (repeatSpeed_f > RS_MAX) repeatSpeed_f = RS_MAX;
        repeatSpeed = (unsigned short)repeatSpeed_f;
      break;
      case FK_1: // function_key_01
        SCK_key_layer = 1;
      break;
      case FK_2: // function_key_02
        SCK_key_layer = 2;
      break;
      case L_KC: // LED_key_change
        led_func[0]();
      break;
      case L_SC: // LED_side_change
        led_func[1]();
      break;
      case L_KL: // LED_key_lighter
        led_func[2]();
      break;
      case L_KD: // LED_key_darker
        led_func[3]();
      break;
      case L_SL: // LED_side_lighter
        led_func[4]();
      break;
      case L_SD: // LED_side_darker
        led_func[5]();
      break;
    }
  } else {
    switch(keycode) {
      case M_LB: // left_mouse
        Mouse.release(MOUSE_LEFT);
      break;
      case M_RB: // right_mouse
        Mouse.release(MOUSE_RIGHT);
      break;
      case M_MB: // middle_mouse
        Mouse.release(MOUSE_MIDDLE);
      break;
      case M_4B: // mouse_button_4
        Mouse.release(MOUSE_PREV);
      break;
      case M_5B: // mouse_button_5
        Mouse.release(MOUSE_NEXT);
      break;
      case FK_1: // function_key_01
        SCK_key_layer = 0;
      break;
      case FK_2: // function_key_02
        SCK_key_layer = 0;
      break;
    }
  }
}

/**
 * @brief special keycode for special function
 * 
 * @param keycode unsigned char, 0x88 ~ 0xAF (40)
 * @param pressed bool, if true, key is pressed
 */
void SCK_code_Function(byte keycode, bool pressed) {
  wdt_disable();
  if (pressed) {
    user_func[keycode - 0x88](); // execute function
  }
  wdt_enable(WDTO_120MS);
  wdt_reset();
}

/**
 * @brief special keycode for consumer key
 * 
 * @param keycode unsigned char, 0xB4 ~ 0xC0 (13)
 * @param pressed bool, if true, key is pressed
 */
void SCK_code_Consumer(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case C_VM: // con_volume_mute
        Consumer.press(MEDIA_VOLUME_MUTE);
        Consumer.release(MEDIA_VOLUME_MUTE);
      break;
      case C_VU: // con_volume_up
        Consumer.press(MEDIA_VOLUME_UP);
      break;
      case C_VD: // con_volume_down
        Consumer.press(MEDIA_VOLUME_DOWN);
      break;
      case C_MP: // con_media_play_pause
        Consumer.press(MEDIA_PLAY_PAUSE);
        Consumer.release(MEDIA_PLAY_PAUSE);
      break;
      case C_MS: // con_media_stop
        Consumer.press(MEDIA_STOP);
        Consumer.release(MEDIA_STOP);
      break;
      case C_MN: // con_media_next
        Consumer.press(MEDIA_NEXT);
        Consumer.release(MEDIA_NEXT);
      break;
      case C_MR: // con_media_prev
        Consumer.press(MEDIA_PREV);
        Consumer.release(MEDIA_PREV);
      break;
      case C_CA: // con_calculator
        Consumer.press(CONSUMER_CALCULATOR);
        Consumer.release(CONSUMER_CALCULATOR);
      break;
      case C_EM: // con_email
        Consumer.press(CONSUMER_EMAIL_READER);
        Consumer.release(CONSUMER_EMAIL_READER);
      break;
      case C_BH: // con_browser_home
        Consumer.press(CONSUMER_BROWSER_HOME);
        Consumer.release(CONSUMER_BROWSER_HOME);
      break;
      case C_BB: // con_browser_back
        Consumer.press(CONSUMER_BROWSER_BACK);
        Consumer.release(CONSUMER_BROWSER_BACK);
      break;
      case C_BF: // con_browser_forward
        Consumer.press(CONSUMER_BROWSER_FORWARD);
        Consumer.release(CONSUMER_BROWSER_FORWARD);
      break;
      /*
      case C_FI: // con_find
        Consumer.press(HID_CONSUMER_AC_FIND);
        Consumer.release(HID_CONSUMER_AC_FIND);
      break;
      */
    }
  } else {
    switch(keycode) {
      case C_VU: // con_volume_up
        Consumer.release(MEDIA_VOLUME_UP);
      break;
      case C_VD: // con_volume_down
        Consumer.release(MEDIA_VOLUME_DOWN);
      break;
    }
  }
}

/**
 * @brief special keycode for surface dial
 * 
 * @param keycode unsigned char, 0xEC ~ 0xEF (4)
 * @param pressed bool, if true, key is pressed
 */
void SCK_code_Surface(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case S_B : // surface_button
        SurfaceDial.press();
      break;
      case S_CW: // surface_clockwise
        SurfaceDial.rotate(45);
      break;
      case S_CC: // surface_counter_clockwise
        SurfaceDial.rotate(-45);
      break;
    }
  } else {
    if(keycode == S_B) {
      SurfaceDial.release();
    }
  }
}

/**
 * @brief special keycode for debug
 * 
 * @param keycode unsigned char, 0xFC ~ 0xFF (4)
 * @param pressed bool, if true, key is pressed
 */
void SCK_code_Debug(byte keycode, bool pressed) {
  if (pressed) {
    debug_func[keycode - 0xFC](); // execute function
  }
}