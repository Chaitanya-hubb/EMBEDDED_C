// Embedded C program to blink an LED using Timer0 in polling method (PIC16F877A)

#include <xc.h>

void delay()
{
    int i;

    for(i = 0; i < 1000; i++)   // Increase delay
    {
        TMR0 = 0;

        while(TMR0 < 255);
    }
}

void main()
{
    TRISB = 0x00;        // PORTB output
    OPTION_REG = 0x87;   // Prescaler 1:256 (slower timer)

    while(1)
    {
        PORTB = 0xFF;    // LED ON
        delay();

        PORTB = 0x00;    // LED OFF
        delay();
    }
}