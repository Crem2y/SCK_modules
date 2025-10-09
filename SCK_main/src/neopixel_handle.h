#pragma once

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "i2c_master_interrupt.h"
#include "sck_functiuon_set.h"
#include "sck_key_define.h"
#include "neopixel_define.h"
#include "user_datas/led_data.h"

Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(NEO_NUM, NEO_PIN, NEO_GRB + NEO_KHZ800);

unsigned short Neo_colors[NEO_KEY+NEO_SIDE] = {0,}; // 0xRGBW

byte neo_key_temp;
byte neo_side_temp;

void led_func_set(void);

void Neo_init(void);
void Neo_loop(void);
void Neo_all_off(void);
void Neo_boot(void);

void Neo_key_change(void);
void Neo_key_off(void);
void Neo_key_lighter(void);
void Neo_key_darker(void);
void Neo_key_rainbow_1(void);
void Neo_key_rainbow_2(void);
void Neo_key_fixed_color(void);
void Neo_key_fixed_custom(void);
void Neo_key_white(void);
void Neo_key_reaction(void);
void Neo_key_key(void);
void Neo_key_breath(void);

void Neo_side_change(void);
void Neo_side_off(void);
void Neo_side_lighter(void);
void Neo_side_darker(void);
void Neo_side_rainbow_1(void);
void Neo_side_rainbow_2(void);
void Neo_side_fixed_color(void);
void Neo_side_fixed_custom(void);
void Neo_side_white(void);
void Neo_side_reaction(void);
void Neo_side_breath(void);

//////////////////////////////// functions ////////////////////////////////

/**
 * @brief load led functions to array
 * 
 */
void led_func_set(void) {
  led_func[0] = Neo_key_change;
  led_func[1] = Neo_side_change;
  
  led_func[2] = Neo_key_lighter;
  led_func[3] = Neo_key_darker;

  led_func[4] = Neo_side_lighter;
  led_func[5] = Neo_side_darker;
}

/**
 * @brief SCK LED initalize
 * 
 */
void Neo_init(void) {
  neopixel.begin();
  Neo.key.bright = 1;
  Neo.side.bright = NEO_BMAX;
}

/**
 * @brief SCK LED loop function
 * 
 */
void Neo_loop(void) {
  Neo.module = NEO_MODULE_AUTO;
  switch(Neo.key.mode) {
    case NEO_MODE_NONE:
      Neo_key_off();
      Neo.module = NEO_MODULE_OFF;
    break;
    case NEO_MODE_RAINBOW_1:
      Neo_key_rainbow_1();
    break;
    case NEO_MODE_RAINBOW_2:
      Neo_key_rainbow_2();
    break;
    case NEO_MODE_FIXED_COL:
      Neo_key_fixed_color();
    break;
    case NEO_MODE_CUSTOM:
      Neo_key_fixed_custom();
    break;
    case NEO_MODE_WHITE:
      Neo_key_white();
    break;
    case NEO_MODE_REACTION:
      Neo_key_reaction();
    break;
    case NEO_MODE_KEY:
      Neo_key_key();
    break;
    case NEO_MODE_BREATH:
      Neo_key_breath();
    break;
    default:
      Neo_key_off();
    break;
  }

  switch(Neo.side.mode) {
    case NEO_MODE_NONE:
      Neo_side_off();
    break;
    case NEO_MODE_RAINBOW_1:
      Neo_side_rainbow_1();
    break;
    case NEO_MODE_RAINBOW_2:
      Neo_side_rainbow_2();
    break;
    case NEO_MODE_FIXED_COL:
      Neo_side_fixed_color();
    break;
    case NEO_MODE_CUSTOM:
      Neo_side_fixed_custom();
    break;
    case NEO_MODE_WHITE:
      Neo_side_white();
    break;
    case NEO_MODE_REACTION:
      Neo_side_reaction();
    break;
    case NEO_MODE_KEY:
      Neo.side.mode = NEO_MODE_BREATH;
    break;
    case NEO_MODE_BREATH:
      Neo_side_breath();
    break;
    default:
      Neo_side_off();
    break;
  }

  Neo.timer++;
  neopixel.show();
}

/**
 * @brief turn off all LEDs
 * 
 */
void Neo_all_off(void) {
  neopixel.clear();
  neopixel.show();
}

/**
 * @brief SCK LED boot animation
 * 
 */
void Neo_boot(void) {
  for(byte j=0; j<NEO_KEY+NEO_SIDE; j++) {
    neopixel.setPixelColor(j, 15, 15, 15);
    neopixel.show();
    delay(1);
  }

  Neo.key.mode = neo_key_temp;
  Neo.side.mode = neo_side_temp;
}

/////////////// neopixel (key) ///////////////

/**
 * @brief change LED mode (key)
 * 
 */
void Neo_key_change(void) {
  Neo.key.mode++;
  if(Neo.key.mode == NEO_MODE_MAX) Neo.key.mode = 0;
  Neo.key.count = 0;
  Neo.key.count2 = 0;
}

/**
 * @brief turn off LEDs (key)
 * 
 */
void Neo_key_off(void) {
  neopixel.fill(neopixel.Color(0,0,0,0),0,NEO_KEY);
}

/**
 * @brief change LED bright lighter (key)
 * 
 */
void Neo_key_lighter(void) {
  if (Neo.key.bright < NEO_BMAX) {
    Neo.key.bright++;
  }
}

/**
 * @brief change LED bright darker (key)
 * 
 */
void Neo_key_darker(void) {
  if (Neo.key.bright > 0) {
    Neo.key.bright--;
  }
}

/**
 * @brief LED rainbow mode 1 (key)
 * 
 */
void Neo_key_rainbow_1(void) {
  if(Neo.timer % 2 == 0) {
    Neo.key.count -= 1;
  }
  uint16_t first_hue = Neo.key.count * 256;
  uint16_t hue;
  uint32_t color;

  hue = first_hue + (0 * 1 * 65536) / 6;
  color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
  color = neopixel.gamma32(color);

  for (uint16_t i=0; i<13; i++) {
    neopixel.setPixelColor(i, color);
  }

  hue = first_hue + (1 * 1 * 65536) / 6;
  color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
  color = neopixel.gamma32(color);

  for (uint16_t i=0; i<14; i++) {
    neopixel.setPixelColor(i+13, color);
  }

  hue = first_hue + (2 * 1 * 65536) / 6;
  color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
  color = neopixel.gamma32(color);

  for (uint16_t i=0; i<14; i++) {
    neopixel.setPixelColor(i+27, color);
  }

  hue = first_hue + (3 * 1 * 65536) / 6;
  color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
  color = neopixel.gamma32(color);

  for (uint16_t i=0; i<13; i++) {
    neopixel.setPixelColor(i+41, color);
  }

  hue = first_hue + (4 * 1 * 65536) / 6;
  color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
  color = neopixel.gamma32(color);

  for (uint16_t i=0; i<12; i++) {
    neopixel.setPixelColor(i+54, color);
  }

  hue = first_hue + (5 * 1 * 65536) / 6;
  color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
  color = neopixel.gamma32(color);

  for (uint16_t i=0; i<8; i++) {
    neopixel.setPixelColor(i+66, color);
  }
}

/**
 * @brief LED rainbow mode 2 (key)
 * 
 */
void Neo_key_rainbow_2(void) {
  Neo.key.count -= 1;
  uint16_t first_hue = Neo.key.count * 256;
  uint16_t hue;
  uint32_t color;

  for (uint16_t i=0; i<13; i++) {
    hue = first_hue + (i * 1 * 65536) / 13;
    color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(i, color);
    neopixel.setPixelColor(53-i, color);
  }

  for (uint16_t i=0; i<14; i++) {
    hue = first_hue + (i * 1 * 65536) / 14;
    color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(26-i, color);
    neopixel.setPixelColor(i+27, color);
  }

  for (uint16_t i=0; i<12; i++) {
    hue = first_hue + (i * 1 * 65536) / 12;
    color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(i+54, color);
  }

  for (uint16_t i=0; i<8; i++) {
    hue = first_hue + (i * 1 * 65536) / 8;
    color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(73-i, color);
  }
}

/**
 * @brief LED fixed color mode (key)
 * 
 */
void Neo_key_fixed_color(void) {
  unsigned short colordata = Neo_colors_fixed[0]; // 0xRGBW
  for(unsigned char i=0; i<NEO_KEY; i++) {
    neopixel.setPixelColor(i, ((colordata & 0xF000) >> 12), ((colordata & 0x0F00) >> 8), ((colordata & 0x00F0) >> 4));
  }
}

/**
 * @brief LED fixed color mode (key)
 * 
 */
void Neo_key_fixed_custom(void) {
  for(unsigned char i=0; i<NEO_KEY; i++) {
    unsigned short colordata = Neo_colors_custom[i]; // 0xRGBW
    neopixel.setPixelColor(i, ((colordata & 0xF000) >> 12), ((colordata & 0x0F00) >> 8), ((colordata & 0x00F0) >> 4));
  }
}

/**
 * @brief LED white color mode (key)
 * 
 */
void Neo_key_white(void) {
  for(unsigned char i=0; i<NEO_KEY; i++) {
    neopixel.setPixelColor(i, Neo.key.bright, Neo.key.bright, Neo.key.bright);
  }
}

/**
 * @brief LED reaction mode (all)
 * 
 */
void Neo_key_reaction(void) {
  bool any_key_pressed = false;
  
  if(Neo.timer % 2 == 0) {
    if(Neo.key.count > 1) {
      Neo.key.count -= 1;
    }
  }

  for(unsigned char i=0; i<KM_V; i++) {
    for(unsigned char j=0; j<KM_H; j++) {
      if(SCK_KM_pressed[i][j]) {
        any_key_pressed = true;
        break;
      }
    }
    if(any_key_pressed) break;
  }

  if(any_key_pressed) {
    Neo.key.count = 15;
  }

  for(unsigned char i=0; i<NEO_KEY; i++) {
    neopixel.setPixelColor(i, Neo.key.count, Neo.key.count, Neo.key.count);
  }
}


/**
 * @brief LED reaction mode (key)
 * 
 */
void Neo_key_key(void) {
  bool timer_check = false;

  if(Neo.timer % 8 == 0) {
    timer_check = true;
  }

  for(unsigned char i=0; i<KM_V; i++) {
    for(unsigned char j=0; j<KM_H; j++) {
      if(SCK_KM_pressed[i][j]) {
        Neo_col_table[Neo_num_table[i][j]] = 15;
      }
    }
  }

  for(unsigned char i=0; i<NEO_KEY; i++) {
    byte bright_temp = Neo_col_table[i];
    if(timer_check && Neo_col_table[i] > 0) Neo_col_table[i] -= 1;
    neopixel.setPixelColor(i, bright_temp, bright_temp, bright_temp);
  }
}

/**
 * @brief LED breath mode (key)
 * 
 */
void Neo_key_breath(void) {
  if(Neo.timer % 16 == 0) {
    if(Neo.key.count2 == 1) {
      Neo.key.count -= 1;
      if(Neo.key.count == 1) Neo.key.count2 = 0;
    } else {
      Neo.key.count += 1;
      if(Neo.key.count == 15) Neo.key.count2 = 1;
    }
  }

  for(unsigned char i=0; i<NEO_KEY; i++) {
    neopixel.setPixelColor(i, Neo.key.count, Neo.key.count, Neo.key.count);
  }
}

/////////////// neopixel (side) ///////////////

/**
 * @brief change LED mode (side)
 * 
 */
void Neo_side_change(void) {
  Neo.side.mode++;
  if(Neo.side.mode == NEO_MODE_MAX) Neo.side.mode = 0;
  Neo.side.count = 0;
  Neo.side.count2 = 0;
}

/**
 * @brief turn off LEDs (side)
 * 
 */
void Neo_side_off(void) {
  neopixel.fill(neopixel.Color(0,0,0,0),NEO_KEY,0);
}

/**
 * @brief change LED bright lighter (side)
 * 
 */
void Neo_side_lighter(void) {
  if (Neo.side.bright < NEO_BMAX) {
    Neo.side.bright++;
  }
}

/**
 * @brief change LED bright darker (side)
 * 
 */
void Neo_side_darker(void) {
  if (Neo.side.bright > 0) {
    Neo.side.bright--;
  }
}

/**
 * @brief LED rainbow mode 1 (side)
 * 
 */
void Neo_side_rainbow_1(void) {
  Neo.side.count += 1;
  uint16_t first_hue = Neo.side.count << 8;

  for (uint16_t i=NEO_KEY; i<NEO_NUM; i++) {
    uint16_t hue = first_hue + (i * 1 * 65536) / NEO_SIDE;
    uint32_t color = neopixel.ColorHSV(hue, 127, Neo.side.bright * 8);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(i, color);
  }
}

/**
 * @brief LED rainbow mode 2 (side)
 * 
 */
void Neo_side_rainbow_2(void) {
  Neo.side.count -= 1;
  uint16_t first_hue = Neo.side.count << 8;

  for (uint16_t i=NEO_KEY; i<NEO_NUM; i++) {
    uint16_t hue = first_hue + (i * 1 * 65536) / NEO_SIDE;
    uint32_t color = neopixel.ColorHSV(hue, 127, Neo.side.bright * 8);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(i, color);
  }
}

/**
 * @brief LED fixed color mode (side)
 * 
 */
void Neo_side_fixed_color(void) {
  unsigned short colordata = Neo_colors_fixed[1]; // 0xRGBW
  for(unsigned char i=0; i<NEO_SIDE; i++) {
    neopixel.setPixelColor(i+NEO_KEY, ((colordata & 0xF000) >> 12), ((colordata & 0x0F00) >> 8), ((colordata & 0x00F0) >> 4));
  }
}

/**
 * @brief LED fixed color mode (side)
 * 
 */
void Neo_side_fixed_custom(void) {
  for(unsigned char i=0; i<NEO_SIDE; i++) {
    unsigned short colordata = Neo_colors_custom[i+NEO_KEY]; // 0xRGBW
    neopixel.setPixelColor(i+NEO_KEY, ((colordata & 0xF000) >> 12), ((colordata & 0x0F00) >> 8), ((colordata & 0x00F0) >> 4));
  }
}

/**
 * @brief LED white color mode (side)
 * 
 */
void Neo_side_white(void) {
  for(unsigned char i=0; i<NEO_SIDE; i++) {
    neopixel.setPixelColor(i+NEO_KEY, Neo.side.bright, Neo.side.bright, Neo.side.bright);
  }
}

/**
 * @brief LED reaction mode (side)
 * 
 */
void Neo_side_reaction(void) {
  bool any_key_pressed = false;

  if(Neo.timer % 2 == 0) {
    if(Neo.side.count > 0) {
      Neo.side.count -= 1;
    }
  }

  for(unsigned char i=0; i<KM_V; i++) {
    for(unsigned char j=0; j<KM_H; j++) {
      if(SCK_KM_pressed[i][j]) {
        any_key_pressed = true;
        break;
      }
    }
    if(any_key_pressed) break;
  }

  if(any_key_pressed) {
    Neo.side.count = 15;
  }

  for(unsigned char i=0; i<NEO_SIDE; i++) {
    neopixel.setPixelColor(i+NEO_KEY, Neo.side.count, Neo.side.count, Neo.side.count);
  }
}

/**
 * @brief LED breath mode (side)
 * 
 */
void Neo_side_breath(void) {
  if(Neo.timer % 16 == 0) {
    if(Neo.side.count2 == 1) {
      Neo.side.count -= 1;
      if(Neo.side.count == 0) Neo.side.count2 = 0;
    } else {
      Neo.side.count += 1;
      if(Neo.side.count == 15) Neo.side.count2 = 1;
    }
  }

  for(unsigned char i=0; i<NEO_SIDE; i++) {
    neopixel.setPixelColor(i+NEO_KEY, Neo.side.count, Neo.side.count, Neo.side.count);
  }
}