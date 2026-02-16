// Embedded C program to blink first four LEDs and last four LEDs alternately on PORTB

#include <xc.h>

#pragma config WDTE = OFF 
#define _XTAL_FREQ  4000000

static void init_config(void)
{
     TRISB = 0x00;  // all pins output
     PORTB = 0x00;  // initialize LOW
}

int main(void)
{
     init_config();

     while(1)
     {
         PORTB = 0x0F;      // first 4 LEDs ON, last 4 OFF
         __delay_ms(1000);

         PORTB = 0xF0;
         __delay_ms(1000);  // last 4 LEDs ON, first 4 OFF
     }
}
