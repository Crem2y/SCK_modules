// for arduino Leonardo board
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include "sck_module_handle.h"
#include "sck_command.h"
#include "neopixel_handle.h"
#include "user_datas/user_functions.h"

#define MODULE_MASTER 0
#define MODULE_SUB    1
#define MODULE_CONFIG MODULE_MASTER

#if MODULE_CONFIG == MODULE_MASTER
  #define KM_RS 18 // keyboard module reset pin

  #define P_NL 19 // num lock led pin
  #define P_CL 20 // caps lock led pin
  #define P_SL 21 // scroll lock led pin
#elif MODULE_CONFIG == MODULE_SUB
  #define SM_RS 11 // sub module reset pin
  #define SM_PM 10 // sub module program mode pin
#endif

String uart_string = "";
unsigned short sleep_count = 0;
bool sleep_mode_prev = false;
bool is_sleep_mode = false;

#define YEAR ((__DATE__ [9] - '0') * 10 + (__DATE__ [10] - '0'))

#define MONTH (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
: __DATE__ [2] == 'b' ? 2 \
: __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
: __DATE__ [2] == 'y' ? 5 \
: __DATE__ [2] == 'l' ? 7 \
: __DATE__ [2] == 'g' ? 8 \
: __DATE__ [2] == 'p' ? 9 \
: __DATE__ [2] == 't' ? 10 \
: __DATE__ [2] == 'v' ? 11 : 12)

#define DAY ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
+ (__DATE__ [5] - '0'))

#define INT2BCD(x) (((x / 10) << 4) | ((x % 10)))

#define VERSION_SUB 0x0A

unsigned char led_buf[19] = {0,};

struct version_t firm_version = {0x01, 0x06, {INT2BCD(YEAR), INT2BCD(MONTH), INT2BCD(DAY)}, VERSION_SUB};

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
#if MODULE_CONFIG == MODULE_MASTER
  pinMode(KM_RS, OUTPUT); // keyboard module reset
  digitalWrite(KM_RS, HIGH);

  pinMode(P_NL, OUTPUT);
  pinMode(P_CL, OUTPUT);
  pinMode(P_SL, OUTPUT);

  digitalWrite(P_NL, HIGH);
  digitalWrite(P_CL, HIGH);
  digitalWrite(P_SL, HIGH);
#elif MODULE_CONFIG == MODULE_SUB
  pinMode(SM_RS, INPUT_PULLUP);
  pinMode(SM_PM, INPUT_PULLUP);
#endif

  Serial.begin(115200);

  digitalWrite(LED_BUILTIN, HIGH);
#if MODULE_CONFIG == MODULE_MASTER
  delay(200); // power stabilization time & led check
  digitalWrite(P_NL, LOW);
  delay(200);
  digitalWrite(P_CL, LOW);
  delay(200);
  digitalWrite(P_SL, LOW);
  delay(200);
#endif

  Neo_init();
  Neo_boot();
  
  Serial.println(F("[sys] - Super Custom Keyboard -"));
  Serial.print(F("[sys] firmware ver. "));
  firm_ver = &firm_version;
  print_firm_ver();

  BootKeyboard.begin();
  Mouse.begin();
  SurfaceDial.begin();

  SCK_init();

#if MODULE_CONFIG == MODULE_MASTER
  if(SCK_KM_count == 0) {
    delay(3000);
    debug_reset();
  }
#endif

  led_func_set();
  user_func_set();
  debug_func_set();

  Neo.key.mode = EEPROM.read(1);
  Neo.key.bright = EEPROM.read(2);
  Neo.side.mode = EEPROM.read(3);
  Neo.side.bright = EEPROM.read(4);

  wdt_enable(WDTO_120MS);
  wdt_reset();
}

void module_led_fixed(void) {
  uint32_t col_temp[6] = { // 0x00RRGGBB
    0x00FFFFFF,
    0x00FFFFFF,
    0x00FFFFFF,
    0x00FFFFFF,
    0x00FFFFFF,
    0x00FFFFFF,
  };

  for(byte i=0; i<6; i++) {
    led_buf[i*3 +1]  = (col_temp[i] & 0x00FF0000) >> 16;
    led_buf[i*3 +2]  = (col_temp[i] & 0x0000FF00) >> 8;
    led_buf[i*3 +3]  = (col_temp[i] & 0x000000FF);
  }
}

void module_led_auto(void) {
  uint32_t col_temp[6];

  col_temp[0] = neopixel.getPixelColor(0);
  col_temp[1] = neopixel.getPixelColor(26);
  col_temp[2] = neopixel.getPixelColor(27);
  col_temp[3] = neopixel.getPixelColor(53);
  col_temp[4] = neopixel.getPixelColor(54);
  col_temp[5] = neopixel.getPixelColor(73);

  for(byte i=0; i<6; i++) {
    led_buf[i*3 +1]  = (col_temp[i] & 0x00FF0000) >> 16;
    led_buf[i*3 +2]  = (col_temp[i] & 0x0000FF00) >> 8;
    led_buf[i*3 +3]  = (col_temp[i] & 0x000000FF);
  }
}

//////////////////////////////// main loop //////////////////////////////// 
void loop(void) {
  if (I2C_is_initalized && !is_sleep_mode) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  serial_loop();
  main_loop();
  sleep_check_loop();
  module_led_control_loop();
  
  wdt_reset();
  delay(1);
}

inline void serial_loop(void) {
  while(Serial.available()) {
    wdt_disable();
    TIM_DISABLE;

    uart_string = Serial.readStringUntil('\n');
    check_command(uart_string);

    TIM_ENABLE;
    wdt_enable(WDTO_120MS);
  }
}

void main_loop(void) {
  SCK_loop();

#if MODULE_CONFIG == MODULE_MASTER
  // lock led
  digitalWrite(P_NL, !(SCK_lock_key & LED_NUM_LOCK));
  digitalWrite(P_CL, !(SCK_lock_key & LED_CAPS_LOCK));
  digitalWrite(P_SL, !(SCK_lock_key & LED_SCROLL_LOCK));
#elif MODULE_CONFIG == MODULE_SUB
  if(!digitalRead(SM_RS)) {
    debug_reset();
  }
  if(!digitalRead(SM_PM)) {
    debug_program_mode();
  }
#endif

}

void sleep_check_loop(void) {
  sleep_mode_prev = is_sleep_mode;
  // if all lock leds are off, check time for sleep mode
  if(!(SCK_lock_key & 0x07)) {
    byte i, j;
    byte k = 0;

    for(i=0; i<KM_V; i++) {
      for(j=0; j<KM_H; j++) {
        if(SCK_KM_pressed[i][j]) {
          k++;
          break;
        }
      }
    }
    if(k == 0) {
      if(sleep_count > 6000) { // 200 = 1s, 6000 = 30s
        is_sleep_mode = true;
      } else {
        sleep_count++;
      }
    } else {
      sleep_count = 0;
      is_sleep_mode = false;
    }
  } else {
    sleep_count = 0;
    is_sleep_mode = false;
  }
}

void module_led_control_loop(void) {

  if(!is_sleep_mode)
  {
    if(is_sleep_mode != sleep_mode_prev)
    {
      Neo.key.mode = neo_key_temp;
      Neo.side.mode = neo_side_temp;
    }
  } else {
    if(is_sleep_mode != sleep_mode_prev)
    {
      neo_key_temp = Neo.key.mode;
      Neo.key.mode = NEO_MODE_NONE;
      neo_side_temp = Neo.side.mode;
      Neo.side.mode = NEO_MODE_NONE;
    }
  }

  TIM_DISABLE;
  Neo_loop();
  TIM_ENABLE;

  if(Neo.module == NEO_MODULE_OFF) 
  {
    SCK_led_power = false;
  } else {
    SCK_led_power = true;
  }

  // send data to module
  // general call data (power, ---, ---, ---, ---, scroll_lock, caps_lock, num_lock)
  led_buf[0] = (SCK_led_power << 7) | (SCK_lock_key & 0x07);
  // led color data (0xRR 0xGG 0xBB 0xRR 0xGG 0xBB ...)
  module_led_auto();
  I2C_write_data(I2C_GCA, led_buf, 1+18);
}

//////////////////////////////// debug functions ////////////////////////////////

/**
 * @brief load debug functions to array
 * 
 */
void debug_func_set(void) {
  debug_func[0] = debug_reset;
  debug_func[1] = debug_program_mode;
  debug_func[2] = debug_led_on;
  debug_func[3] = debug_led_off;
}

void debug_reset(void) {
  wdt_enable(WDTO_15MS);
  while(1);
}

void debug_program_mode(void) {
  cli();
  wdt_disable();
  led_buf[0] = 0x00;
  I2C_write_byte(I2C_GCA, led_buf);
  Neo_all_off();

  byte i, j;
  while(true) {

#if MODULE_CONFIG == MODULE_MASTER
    SCK_loop();
    if(SCK_KM_pressed[0][0]) { // if ESC key is pressing
      debug_reset();
    }

    for(i=0; i<10; i++) {
      for(j=0; j<5; j++) {
        digitalWrite(P_NL, 1);
        digitalWrite(P_CL, 1);
        digitalWrite(P_SL, 1);
        delay(i);
        digitalWrite(P_NL, 0);
        digitalWrite(P_CL, 0);
        digitalWrite(P_SL, 0);
        delay(10-i);
      }
    }
    for(i=0; i<10; i++) {
      for(j=0; j<5; j++) {
        digitalWrite(P_NL, 1);
        digitalWrite(P_CL, 1);
        digitalWrite(P_SL, 1);
        delay(10-i);
        digitalWrite(P_NL, 0);
        digitalWrite(P_CL, 0);
        digitalWrite(P_SL, 0);
        delay(i);
      }
    }
#elif MODULE_CONFIG == MODULE_SUB
    if(!digitalRead(SM_RS)) {
      debug_reset();
    }

    for(i=0; i<10; i++) {
      for(j=0; j<5; j++) {
        digitalWrite(LED_BUILTIN, 1);
        delay(i);
        digitalWrite(LED_BUILTIN, 0);
        delay(10-i);
      }
    }
    for(i=0; i<10; i++) {
      for(j=0; j<5; j++) {
        digitalWrite(LED_BUILTIN, 1);
        delay(10-i);
        digitalWrite(LED_BUILTIN, 0);
        delay(i);
      }
    }
#endif

  }
}

void debug_led_on(void) {
  Neo.key.mode = neo_key_temp;
  Neo.side.mode = neo_side_temp;
}

void debug_led_off(void) {
  neo_key_temp = Neo.key.mode;
  Neo.key.mode = NEO_MODE_NONE;
  neo_side_temp = Neo.side.mode;
  Neo.side.mode = NEO_MODE_NONE;
}