#pragma once
/**
 * @brief led functions (LED_key_change, LED_side_change, mLED_key_lighter, LED_key_darker, LED_side_lighter, LED_side_darker)
 * 
 */
void (*led_func[6])(void);

/**
 * @brief user functions. use freely!
 *
 */
void (*user_func[40])(void);

/**
 * @brief debug functions
 * 
 */
void (*debug_func[4])(void);

void SCK_func_none(void);
void SCK_led_func_init(void);
void SCK_user_func_init(void);
void SCK_debug_func_init(void);

//////////////////////////////// functions ////////////////////////////////

/**
 * @brief a function that does nothing
 * 
 */
void SCK_func_none(void) {
}

/**
 * @brief initalize led functions
 * 
 */
void SCK_led_func_init(void) {
  for(byte i=0; i<6; i++) {
    led_func[i] = SCK_func_none;
  }
}

/**
 * @brief initalize user functions
 * 
 */
void SCK_user_func_init(void) {
  for(byte i=0; i<40; i++) {
    user_func[i] = SCK_func_none;
  }
}

/**
 * @brief initalize debug functions
 * 
 */
void SCK_debug_func_init(void) {
  for(byte i=0; i<4; i++) {
    debug_func[i] = SCK_func_none;
  }
}