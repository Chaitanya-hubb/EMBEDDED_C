/*Implemented Time Separator using Timer0, Timer1 and Timer2 with synchronized 0.5s LED blinking using interrupts */

#include <xc.h>

#pragma config WDTE = OFF

#define LED0 RD0
#define LED1 RD1
#define LED2 RD2

#define LED_DDR TRISD

/* ---------------- Timer0 ---------------- */
void init_timer0(void)
{
    OPTION_REG = 0x03;   // prescaler 1:256
    TMR0 = 6;
    T0IF = 0;
    T0IE = 1;
}

/* ---------------- Timer1 ---------------- */
void init_timer1(void)
{
    T1CON = 0x31;   // prescaler 1:8, timer ON
    TMR1H = 0xFE;
    TMR1L = 0x0C;

    TMR1IF = 0;
    TMR1IE = 1;
}

/* ---------------- Timer2 ---------------- */
void init_timer2(void)
{
    T2CON = 0x7C;   // prescaler + postscaler
    PR2 = 249;

    TMR2IF = 0;
    TMR2IE = 1;
}

/* ---------------- ISR ---------------- */
void __interrupt() isr(void)
{
    static unsigned int c0 = 0;
    static unsigned int c1 = 0;
    static unsigned int c2 = 0;

    /* Timer0 */
    if (T0IF)
    {
        T0IF = 0;
        TMR0 = 6;

        c0++;
        if (c0 == 500)
        {
            LED0 = !LED0;
            c0 = 0;
        }
    }

    /* Timer1 */
    if (TMR1IF)
    {
        TMR1IF = 0;
        TMR1H = 0xFE;
        TMR1L = 0x0C;

        c1++;
        if (c1 == 500)
        {
            LED1 = !LED1;
            c1 = 0;
        }
    }

    /* Timer2 */
    if (TMR2IF)
    {
        TMR2IF = 0;

        c2++;
        if (c2 == 500)
        {
            LED2 = !LED2;
            c2 = 0;
        }
    }
}

/* ---------------- Init ---------------- */
void init_config(void)
{
    LED_DDR = 0x00;

    LED0 = LED1 = LED2 = 0;

    init_timer0();
    init_timer1();
    init_timer2();

    PEIE = 1;
    GIE  = 1;
}

/* ---------------- Main ---------------- */
void main(void)
{
    init_config();

    while (1)
    {
        // CPU free
    }
}