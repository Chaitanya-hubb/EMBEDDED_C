// Embedded C program to implement LED fill and clear train pattern on PORTB


#include <xc.h>
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)

#define _XTAL_FREQ  4000000

static void init_config(void) 
{
    //write initialization code here 
    TRISB = 0x00;   // Set PORTB as output
    PORTB = 0x00;   // Turn OFF all LEDs

}

void main(void) {

    init_config();

    while (1) {

        PORTB = 0x00;   // Start with all OFF
        
        //write application code here

        //TURN ON one by one
        for(int i = 7; i >= 0; i--)
        {
            PORTB |= (1 << i);
            __delay_ms(300);
        }

        // Small delay after full ON
        __delay_ms(300);
        
        //TURN OFF one by one
        for(int i = 7; i >= 0; i--)
        {
            PORTB &= ~(1 << i);
            __delay_ms(300);
        }
        __delay_ms(300);
    }
}
