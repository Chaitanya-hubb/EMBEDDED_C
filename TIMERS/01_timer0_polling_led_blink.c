// Embedded C program to blink an LED using Timer0 in polling method (PIC16F877A)

#include <xc.h>

#pragma config WDTE = OFF

#define LED PORTB
#define LED_DDR TRISB

void init_config(void)
{
    LED_DDR = 0x00;
    LED = 0x00;

    OPTION_REG = 0x07; // Prescaler 1:256
    TMR0 = 0;
    T0IF = 0;
}

void main(void)
{
    unsigned int count = 0;

    init_config();

    while (1)
    {
        if (T0IF)   //polling
        {
            T0IF = 0;
            TMR0 = 0;

            count++;

            if (count == 50)
            {
                LED = ~LED;
                count = 0;
            }
        }
    }
}