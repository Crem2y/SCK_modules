#pragma once

#define TIM_DISABLE TIMSK3 &= (0 << OCIE3A)
#define TIM_ENABLE  TIMSK3 |= (1 << OCIE3A)

/**
 * @brief timer set (default : 1kHz)
 * 
 */
void timer_init(void) {
  TCCR3A = 0x00;
  TCCR3B = 0x0A;
  TCNT3  = 0;
  OCR3A  = 1000 - 1;

  TIFR3  = 0x02;
  TIMSK3 = 0x02;
}

/**
 * @brief set timer period
 * 
 */
void timer_set_period(unsigned short ms) {
  if(ms < 2) {
    ms = 2;
  } 
  OCR3A  = ms - 1;
}