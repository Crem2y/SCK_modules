# SCK_modules

## Description
Firmware for **modular submodules** of the SCK (Super Custom Keyboard) system.  
Each module runs on its own MCU and communicates with the main controller via I2C.
- [SCK_main](https://github.com/Crem2y/SCK_main)
---

## Development Environment

- **IDE**: Microchip Studio (or AVR Studio)  
- **MCU**:  
  - ATmega8A

---

## Modules Overview

Each module has its own firmware and schematic.

### SCK Macro Module
- **MCU**: ATmega8A  
- [SCK_Macro](SCK_Macro/README.md)
---

### SCK Keypad Module
- **MCU**: ATmega8A  
- [SCK_keyPad](SCK_keyPad/README.md)

---

### SCK Fnkey Module
- **MCU**: ATmega8A  
- [SCK_Fnkey](SCK_Fnkey/README.md)

---

### SCK Keyboard Module
- **MCUs**: ATmega8A + Arduino Pro Micro (ATmega32U4, 5V, 16MHz)  
- [SCK_Keyboard](SCK_Keyboard/README.md)

---

## Communication (I2C)

- Each module acts as an **I2C slave** with a fixed address  
- Reports key or encoder input when polled by the main controller  

---

## Used Libraries

- [**pololu-led-strip-avr**](https://github.com/pololu/pololu-led-strip-avr):  
  Used for WS2812/APA102 LED strip control.