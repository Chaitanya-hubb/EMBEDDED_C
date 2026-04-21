// LED ON/OFF blinking using Timer0 polling with preload for 1-second delay

#include <xc.h>

#pragma config WDTE = OFF

#define LED    PORTB
#define LED_DDR TRISB

void init_config(void)
{
    LED_DDR = 0x00;    // PORTB as output
    LED = 0x00;        // LEDs OFF initially
 
    OPTION_REG = 0x07; // Prescaler 1:256
    TMR0 = 6;          // Preload value (~64ms per overflow)

    T0IF = 0;         // Clear Timer0 interrupt flag
    T0IE = 0;         // No interrupt (polling method)
}

void delay_1sec(void)
{
    unsigned int count = 0;

    while(count < 15)    // ~15 x 64ms ≈ 1 second
    {
        if(T0IF)
        {
            T0IF = 0;   //clear flag
            TMR0 = 6;   //Reload preload
            count++;
        }
    }
}

void main(void)
{
   init_config();

   while(1)
   {
      LED = 0xFF;     //LED ON
      delay_1sec();

      LED = 0x00;    //LED OFF
      delay_1sec();
   }
}
