// Timer0 interrupt-based LED running pattern using left shift on PIC16F877A

#include <xc.h>

#pragma config WDTE = OFF

#define LED     PORTB
#define LED_DDR TRISB

void init_config(void)
{
    LED_DDR = 0x00;   // PORTB as output
    LED = 0x01;       // Start with first LED ON

    OPTION_REG = 0x07; // Prescaler 1:256
    TMR0 = 6;          // Preload value (~64)

    T0IF = 0;     // Clear flag
    T0IE = 1;     // Enable Timer0 interrupt
    GIE = 1;      // Enable global interrupt
}

// Timer0 ISR: generates periodic delay and updates LED running pattern
void __interrupt() ISR(void)
{
    static unsigned int count = 0;

    if(T0IF)
    {
       T0IF = 0; //clear interrupt flag
       TMR0 = 6; //Reload timer

       count++;

       if(count == 15)   //~delay
       {
           LED = LED << 1;  // Shift LED left
           if(LED == 0X00)   // If all LEDs OFF
           {
              LED = 0x01;   // Restart from first LED
           }
           count = 0;
      }
    }
}
void main(void)
{
    init_config();

    while (1)
    {
        // CPU free, work done in ISR
    }
}