// Embedded C program to blink LED using Timer0 interrupt method (PIC16F877A)

#include <xc.h>

void __interrupt() ISR()
{
    static int count = 0;

    if (T0IF)              // Check Timer0 overflow interrupt
    {
        count++;

        if(count == 50)    // Adjust this value to control speed
        {
            PORTB = ~PORTB;   // Toggle LEDs on PORTB
            count = 0;
        }

        TMR0 = 0;          // Reload timer
        T0IF = 0;          // Clear interrupt flag
    }
}

void main()
{
    TRISB = 0x00;          // Configure PORTB as output
    PORTB = 0x00;          // Initial LEDs OFF

    OPTION_REG = 0x87;     // Prescaler 1:256

    TMR0 = 0;              // Initialize Timer0

    T0IE = 1;              // Enable Timer0 interrupt
    GIE = 1;               // Enable global interrupt

    while(1)
    {
        // CPU is free (no polling)
    }
}