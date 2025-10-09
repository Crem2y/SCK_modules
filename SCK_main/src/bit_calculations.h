#pragma once

#ifndef sbi // set bit
#define sbi(reg, bit) reg |= (1<<bit)
#else
#warning sbi is already defined!
#endif

#ifndef cbi // clear bit
#define cbi(reg, bit) reg &= ~(1<<bit)
#else
#warning cbi is already defined!
#endif

#ifndef xbi // xor bit
#define xbi(reg, bit) reg ^= (1<<bit)
#else
#warning xbi is already defined!
#endif

#ifndef sbc // set bit check
#define sbc(reg, bit) reg & (1<<bit)
#else
#warning sbc is already defined!
#endif

#ifndef cbc // cleared bit check
#define cbc(reg, bit) !(reg & (1<<bit))
#else
#warning cbc is already defined!
#endif