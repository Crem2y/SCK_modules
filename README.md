# SCK_modules

## Description
Firmware for **modular submodules** of the SCK (Super Custom Keyboard) system.  
Each module runs on its own MCU and communicates with the main controller via I2C.
- main firmware : [SCK_main](https://github.com/Crem2y/SCK_main)
---

## Development Environment

- **IDE**: Microchip Studio (or AVR Studio)  
- **MCU**:  
  - ATmega8A

---

## Modules Overview

- Each module has its own firmware and schematic.
- It can be equip multiple of the same module, but only the macro module is working.

### SCK Main
- **MCU**: Arduino Pro Micro (ATmega32U4, 5V, 16MHz)
- I²C Address : none. just master 
- main firmware : [SCK_main](https://github.com/Crem2y/SCK_main)

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

---

## Communication (I2C)

- Each module acts as an **I2C slave** with a fixed address (it can be set by jumpers in each modules)
- Reports key or encoder input when polled by the main controller  

---

## Used Libraries

- [**pololu-led-strip-avr**](https://github.com/pololu/pololu-led-strip-avr):  
  Used for WS2812/APA102 LED strip control.