#pragma once

void WDT_enable(void);                     // enable watchdog timer to 0.11s
void WDT_disable(void);
void WDT_reset(void);

bool WDT_flag = false; // watchdog timer flag

void WDT_enable(void) {  // enable watchdog timer to 0.11s
  
  WDTCR = 0x18;
  WDTCR = 0x1B;
  WDT_flag = true;
}

void WDT_disable(void) {
  
  WDTCR = 0x18;
  WDTCR = 0x00;
  WDT_flag = false;
}

void WDT_reset(void) {
  
  if(WDT_flag) {
    asm volatile(" WDR ");
  }
}