# SCK (Super Custom Keyboard)

![sck](images/picture/sck.jpg)

## Description
- Modular keyboard system
- Each module runs on its own MCU and communicates with the main controller via I2C.
---

## Development Environment

### Main board
- **IDE**: PlatformIO (or arduino IDE)
- **Board**:  
  - Arduino Pro Micro (ATmega32U4, 5V, 16MHz)
### Modules
- **IDE**: Microchip Studio (or AVR Studio)  
- **MCU**:  
  - ATmega8A

---

## Modules Overview

![modules](images/picture/modules.jpg)

- Each module has its own firmware and schematic.

### SCK Main
- **MCU**: ATmega32U4
- I²C Address : none. just master
- Attached to the keyboard module
- [SCK_main](SCK_main/README.md)

### SCK Keyboard Module
- **MCU**: ATmega8A
- I²C Address : 0x10 - 0x17 (0x10 default)
- [SCK_Keyboard](SCK_Keyboard/README.md)

### SCK Keypad Module
- **MCU**: ATmega8A
- I²C Address : 0x18 - 0x1B (0x18 default)
- [SCK_keyPad](SCK_keyPad/README.md)

### SCK Fnkey Module
- **MCU**: ATmega8A
- I²C Address : 0x1C - 0x1F (0x1C default)
- [SCK_Fnkey](SCK_Fnkey/README.md)

### SCK Macro Module
- **MCU**: ATmega8A
- I²C Address : 0x20 - 0x2F (0x20 default)
- [SCK_Macro](SCK_Macro/README.md)

## How to upload firmware

### Main board
1. Press `D_PG` key (Default : FN1 + `\``)
2. The LED will blink slowly.
3. You can upload firmware to main board.
4. Press ESC to cancel programming mode.
### Modules
1. You will need an AVR ISP that can supply power to target board.
2. Connect AVR ISP to ISP port. (2x3 header)
3. Program to module. (DO NOT press any key while programming)

---

## Communication (I²C)

- Each module acts as an **I²C slave** with a fixed address (it can be set by jumpers in each modules)
- Reports key or encoder input when polled by the main controller  

---

## License

This project is licensed under the MIT License.  
See [LICENSE](./LICENSE) for details.

---

### Third-party libraries

This project is licensed primarily under the **MIT License**.  
It also includes third-party components with the following licenses:

- **NicoHood/HID** (by NicoHood), licensed under the **MIT License**.  
  See [NicoHood-HID-MIT.txt](third_party_licenses/NicoHood-HID-MIT.txt).
- **Adafruit_NeoPixel** (by Adafruit), licensed under the **LGPL-3.0 License**.  
  See [Adafruit_NeoPixel-LGPL-3.0.txt](third_party_licenses/Adafruit_NeoPixel-LGPL-3.0.txt).
- **pololu-led-strip-avr** (by pololu), licensed under the **MIT License**.  
See [pololu-led-strip-avr-MIT.txt](third_party_licenses/pololu-led-strip-avr-MIT.txt).