//Embedded C program to blink all leds ON for 1 sec and OFF for 1 sec on PORTB

#include <xc.h>

#pragma config WDTE = 0FF      // Watchdog Timer disabled
#define __XTAL_FREQ  4000000   // 4 MHz crystal frequency

static void init_congit(void)
{
   TRISB = 0x00;  //set all pins as output 
}

void main()
{
   init_config();
   
   while(1)
   {
       PORTB = 0xFF;     //TURN ON all LEds on PORTB
       __delay_ms(100);
      PORTB = 0x00;     // TURN OFF all LEDs on PORTB
      __delay_ms(100);
   }
   return;
}