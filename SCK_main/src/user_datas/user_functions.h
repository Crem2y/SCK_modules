#pragma once

#include "../sck_functiuon_set.h"
#include "../sck_key_handle.h"

/////////////// user function ///////////////

/**
 * @brief sample user function
 * 
 */
void uf_print_sample(void) {
  Keyboard.println("hello world!");
}

/**
 * @brief load user functions to array
 * 
 */
void user_func_set(void) {
  user_func[0] = uf_print_sample;
}

/**
 * @brief load an user function to array
 * 
 * @param func user function, ex) void func(void) {...}
 * @param func_num function number, 1~40
 */
void user_func_set_one(void func(void), unsigned char func_num) {
  if(func_num < 41) {
    user_func[func_num-1] = func;
  }
}