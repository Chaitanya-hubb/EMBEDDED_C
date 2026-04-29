/* Implemented point brightness control using Timer0 ISR based software PWM */

#include <xc.h>

#pragma config WDTE = OFF        // Watchdog Timer Disabled

/* ---------------------- MAIN ---------------------- */

#define LED1                RD0

#define LED_ARRAY           PORTD
#define LED_ARRAY_DDR       TRISD

#define ON                  1
#define OFF                 0

#define LEVEL_DETECTION     0
#define LEVEL               0

#define STATE_DETECTION     1
#define STATE               1

#define KEYPAD_PORT         PORTB
#define KEYPAD_PORT_DDR     TRISB

#define INPUT_LINES         0x3F

#define SW1                 0x3E
#define SW2                 0x3D
#define SW3                 0x3B
#define SW4                 0x37
#define SW5                 0x2F
#define SW6                 0x1F

#define ALL_RELEASED        INPUT_LINES

/* ---------------------- TIMER ---------------------- */

void init_timer0(void)
{
    T0CS = 0;        // Internal clock source
    PSA  = 1;        // Prescaler assigned to WDT
    TMR0 = 6;        // Preload value

    TMR0IE = 1;      // Enable Timer0 interrupt
    TMR0IF = 0;      // Clear interrupt flag
}

/* ---------------------- KEYPAD ---------------------- */

void init_digital_keypad(void)
{
    KEYPAD_PORT_DDR |= INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;

    if (mode == LEVEL_DETECTION)
    {
        return (KEYPAD_PORT & INPUT_LINES);
    }
    else if (mode == STATE_DETECTION)
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;
            return (KEYPAD_PORT & INPUT_LINES);
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
        {
            once = 1;
        }
    }

    return ALL_RELEASED;
}

/* ---------------------- GLOBAL VARIABLES ---------------------- */

static unsigned long int timer_count = 0;
static unsigned long int five_second_counter = 0;

/* ---------------------- ISR ---------------------- */

void __interrupt() isr(void)
{
    if (TMR0IF == 1)
    {
        TMR0 = 6;      // Reload timer
        TMR0IF = 0;    // Clear interrupt flag

        if (++timer_count == 15000)
        {
            timer_count = 0;

            if (++five_second_counter == 5)
            {
                five_second_counter = 0;
                TMR0IE = 0;     // Disable interrupt after 5 sec
            }
        }
    }
}

/* ---------------------- CONFIG ---------------------- */

void init_config(void)
{
    LED_ARRAY = 0x00;
    LED_ARRAY_DDR = 0x00;

    init_digital_keypad();
    init_timer0();

    PEIE = 1;
    GIE  = 1;
}

/* ---------------------- PWM FUNCTION ---------------------- */

void software_pwm(void)
{
    static unsigned long int period = 100;
    static unsigned long int loop_counter = 0;
    static unsigned long int duty_cycle = 50;

    unsigned char key;

    /* Default dim brightness */
    if (TMR0IE == 0)
    {
        duty_cycle = 10;
    }

    key = read_digital_keypad(STATE);

    /* Full brightness on SW1 press */
    if (key == SW1)
    {
        duty_cycle = 100;

        TMR0IE = 1;
        timer_count = 0;
        five_second_counter = 0;
    }

    /* PWM generation */
    if (loop_counter < duty_cycle)
    {
        LED1 = ON;
    }
    else
    {
        LED1 = OFF;
    }

    /* Period counter */
    if (loop_counter++ >= period)
    {
        loop_counter = 0;
    }
}

/* ---------------------- MAIN ---------------------- */

void main(void)
{
    init_config();

    while (1)
    {
        software_pwm();
    }
}