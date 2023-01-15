// fuse bit : 0xCF 0xEF
// high 0b11001111
// low  0b11101111

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "led_sk6812.h"
#include "rotery_sw.h"

#define LED_PRT PORTB
#define LED_DDR DDRB
#define LED_PIN 1

#define LED_COUNT 5
rgbw_color pixel[LED_COUNT];

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

