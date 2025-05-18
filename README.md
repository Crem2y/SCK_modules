# SCK_modules

## 📌 Description
Firmware for **modular submodules** of the SCK (Super Custom Keyboard) system.  
Each module runs on its own MCU and communicates with the main controller via I2C.
- [SCK_main](https://github.com/Crem2y/SCK_main)
---

## 🔧 Development Environment

- **IDE**: Microchip Studio (or AVR Studio)  
- **MCU**:  
  - ATmega8A  

---

## 📚 Used Libraries

- [**pololu-led-strip-avr**](https://github.com/pololu/pololu-led-strip-avr):  
  Used for WS2812/APA102 LED strip control.

---

## 🔌 Modules Overview

Each module has its own firmware and schematic.

### 🔹 SCK Macro Module
- **MCU**: ATmega8A  
- **Schematic**:  
  ![Macro](images/SCK_MM_V1.2.png)

---

### 🔹 SCK Keypad Module
- **MCU**: ATmega8A  
- **Schematic**:  
  ![Keypad main](images/SCK_PM_V1.2_(main).png)  
  ![Keypad keys](images/SCK_PM_V1.2_(key).png)  
  ![Keypad LEDs](images/SCK_PM_V1.2_(led).png)

---

### 🔹 SCK Fnkey Module
- **MCU**: ATmega8A  
- **Schematic**:  
  ![Fnkey main](images/SCK_FM_V1.2_(main).png)  
  ![Fnkey key/LED](images/SCK_FM_V1.2_(key,led).png)

---

### 🔹 SCK Keyboard Module
- **MCUs**: ATmega8A + Arduino Pro Micro (ATmega32U4, 5V, 16MHz)  
- **Schematic**:  
  ![Keyboard main](images/SCK_KM_V1.2_(main).png)  
  ![Keyboard keys](images/SCK_KM_V1.2_(key).png)  
  ![Keyboard LEDs](images/SCK_KM_V1.2_(led).png)

---

## 📡 Communication (I2C)

- Each module acts as an **I2C slave** with a fixed address  
- Reports key or encoder input when polled by the main controller  
