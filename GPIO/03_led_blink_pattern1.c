// Embedded C program to implement LED fill and clear train pattern on PORTB

#include <xc.h>

#pragma config WDTE = OFF   // Watchdog Timer disabled

#define _XTAL_FREQ 8000000


static void init_config(void)
{
    TRISB = 0x00;   // Set PORTB as output
    PORTB = 0x00;   // Turn OFF all LEDs
}

void main(void) {

    init_config();

    while(1)
    {
        PORTB = 0x00;   // Start with all OFF

        //TURN ON one by one
        for(int i = 0; i < 8; i++)
        {
            PORTB |= (1 << i);   // Turn ON one LED
            __delay_ms(200);
        }

        // Small delay after full ON
        __delay_ms(300);

        //TURN OFF one by one
        for(int i = 0; i < 8; i++)
        {
            PORTB &= ~(1 << i);   // Turn OFF one LED
            __delay_ms(200);
        }

        __delay_ms(300);
    }
}