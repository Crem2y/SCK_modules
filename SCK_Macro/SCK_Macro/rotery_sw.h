//using INT0, INT1

#ifndef ROTERY_SW_H_
#define ROTERY_SW_H_

#define RSW_NONE 0
#define RSW_CCW  1
#define RSW_CW   2

volatile int previous = 0;
volatile int counter = 0;

void rotery_init(void);
void rotery_changed(void);
unsigned char rotery_chack(void);

ISR(INT0_vect) {
  rotery_changed();
}

ISR(INT1_vect) {
  rotery_changed();
}

void rotery_init(void) {
  MCUCR |= 0x05; // Detect INT0, INT1 any logical change
  GICR = 0xC0; // INT0, INT1 enable
}

void rotery_changed(void) {
  unsigned char A = (PIND & 0x04) ? 1 : 0;
  unsigned char B = (PIND & 0x08) ? 1 : 0;

  unsigned char current = (A << 1) | B;
  unsigned char combined  = (previous << 2) | current;
  
  if(combined == 0b0010 ||
  combined == 0b1011 ||
  combined == 0b1101 ||
  combined == 0b0100) {
    counter++;
  }
  
  if(combined == 0b0001 ||
  combined == 0b0111 ||
  combined == 0b1110 ||
  combined == 0b1000) {
    counter--;
  }

  previous = current;
}

unsigned char rotery_check(void) {
  if(counter >= 4) {
    counter -= 4;
    return RSW_CW;
  } else if(counter <= -4) {
    counter += 4;
    return RSW_CCW;
  }
  return RSW_NONE;
}  

#endif /* ROTERY_SW_H_ */