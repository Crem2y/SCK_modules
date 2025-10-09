#pragma once

#include "sck_key_define.h"

#define NEO_PIN  9  // neopixel data pin
#define NEO_KEY  74 // number of neopixels (key)
#define NEO_SIDE 16 // number of neopixels (side)
#define NEO_NUM  NEO_KEY + NEO_SIDE // number of neopixels
#define NEO_BMAX 15 // LED max bright (default)

enum neo_mode {
  NEO_MODE_NONE = 0,  // neopixel off
  NEO_MODE_RAINBOW_1, // neopixel rainbow mode (1)
  NEO_MODE_RAINBOW_2, // neopixel rainbow mode (2)
  NEO_MODE_FIXED_COL, // neopixel fixed color mode (one color)
  NEO_MODE_CUSTOM,    // neopixel custom color mode
  NEO_MODE_WHITE,     // neopixel white color mode (fixed)
  NEO_MODE_REACTION,  // neopixel reaction mode (all)
  NEO_MODE_KEY,       // neopixel reaction mode (key)
  NEO_MODE_BREATH,    // neopixel breath mode
  NEO_MODE_MAX,       //
};

enum neo_module_mode {
  NEO_MODULE_OFF = 0, // module led off
  NEO_MODULE_AUTO,    // module led auto mode
  NEO_MODULE_FIXED,   // module led fixed mode
  NEO_MODULE_MAX,     //
};

struct _neo_value {
  byte count;
  byte count2;
  byte mode;
  byte bright;
};

struct _neo {
  byte timer;
  byte module;
  struct _neo_value key;
  struct _neo_value side;
} Neo;

const byte Neo_num_table[KM_V][KM_H] = { // led number table
  {0   ,0   ,1   ,2   ,3   ,4   ,5   ,6   ,7   ,8   ,9   ,10  ,11  ,12  },
  {26  ,25  ,24  ,23  ,22  ,21  ,20  ,19  ,18  ,17  ,16  ,15  ,14  ,13  },
  {27  ,28  ,29  ,30  ,31  ,32  ,33  ,34  ,35  ,36  ,37  ,38  ,39  ,40  },
  {53  ,52  ,51  ,50  ,49  ,48  ,47  ,46  ,45  ,44  ,43  ,42  ,0   ,41  },
  {54  ,55  ,56  ,57  ,58  ,59  ,60  ,61  ,62  ,63  ,64  ,0   ,0   ,65  },
  {73  ,72  ,71  ,0   ,0   ,70  ,0   ,0   ,0   ,69  ,68  ,67  ,0   ,66  }
};

byte Neo_col_table[NEO_KEY] = {0,}; // led color table