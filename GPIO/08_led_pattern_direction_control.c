/*
 * Project: LED Pattern Generator with Direction Control
 * Controller: PIC16F877A
 *
 * Description:
 * Generates LED train pattern (entry & exit).
 * Direction can be changed using switch input (SW1).
 *
 * Concepts Used:
 * - GPIO (input/output)
 * - Bit manipulation
 * - State-based logic
 */

#include <xc.h>

#pragma config WDTE = OFF

#define LED_ARRAY_1      PORTD
#define LED_ARRAY_1_DDR  TRISD

#define KEYPAD_PORT      PORTB
#define KEYPAD_DDR       TRISB

#define SW1              0x3E
#define INPUT_LINES      0x3F
#define ALL_RELEASED     INPUT_LINES

void init_keypad(void)
{
    KEYPAD_DDR |= INPUT_LINES;
}

unsigned char read_keypad(void)
{
    static unsigned char once = 1;

    if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
    {
        once = 0;
        return KEYPAD_PORT & INPUT_LINES;
    }
    else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
    {
        once = 1;
    }

    return ALL_RELEASED;
}

void delay(void)
{
    for (unsigned long i = 0; i < 20000; i++);
}

void init_config(void)
{
    LED_ARRAY_1 = 0x00;
    LED_ARRAY_1_DDR = 0x00;
    init_keypad();
}

void main(void)
{
    unsigned char key;
    unsigned char flag = 1;   // direction
    unsigned char pattern = 0;
    unsigned char step = 0;  

    init_config();

    while (1)
    {
        key = read_keypad();

        // Toggle direction
        if (key == SW1)
        {
            delay();
            flag = !flag;
        }

        // LEFT → RIGHT
        if (flag)
        {
            if (step < 8)  // LEDs ON
            {
                pattern |= (1 << step);
                step++;
            }
            else if (step < 16) // LEDs OFF
            {
                pattern &= ~(1 << (step - 8));
                step++;
            }
            else
            {
                step = 0;
                pattern = 0;
            }
        }
        // RIGHT → LEFT
        else
        {
            if (step < 8)
            {
                pattern |= (1 << (7 - step));
                step++;
            }
            else if (step < 16)
            {
                pattern &= ~(1 << (15 - step));
                step++;
            }
            else
            {
                step = 0;
                pattern = 0;
            }
        }

        LED_ARRAY_1 = pattern;

        delay();
    }
}