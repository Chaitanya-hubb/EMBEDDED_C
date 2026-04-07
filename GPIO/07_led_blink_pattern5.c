//Embedded C program to implement LED fill and clear pattern on PORTB

#include <xc.h>

#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
                    
#define _XTAL_FREQ  4000000

static void init_config(void)
{
    //write initialization code here 
    TRISB = 0x00;   // Set PORTB as output
    PORTB = 0x00;   // Turn OFF all LEDs
}

void main()
{
    unsigned int wait = 0;
    int i = 0;

    init_config();

    while(1)
    {
        if(wait++ == 15000)
        {
            wait = 0;
            
            if(i >= 0 && i < 8)               //LEFT ON
            {
                PORTB = (PORTB << 1) | 1; 
            }
            else if(i >= 8 && i < 16)        //LEFT OFF
            {
                PORTB = (PORTB << 1);
            }
            else if(i >= 16 && i < 24)       // Right ON
            {
                PORTB = (PORTB >> 1) | 0x80;
            }
            else if(i >= 24 && i < 32)       // Right OFF
            {
                PORTB = (PORTB >> 1);
            }
            else
            {
                i = 0;
            }

            i++;
        }
    }
}