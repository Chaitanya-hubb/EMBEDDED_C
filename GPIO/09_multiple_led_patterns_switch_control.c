/*
 * Project: Multiple LED Patterns using Switch Control
 * Controller: PIC16F877A
 *
 * Description:
 * Displays different LED patterns based on switch input.
 * Each switch selects a unique LED pattern.
 *
 * Concepts Used:
 * - GPIO (input/output)
 * - Bit manipulation
 * - State detection (keypad)
 */

#include <xc.h>

#pragma config WDTE = OFF

#define LED_ARRAY1      PORTD
#define LED_ARRAY1_DDR  TRISD

#define KEYPAD_PORT     PORTB
#define KEYPAD_DDR      TRISB

#define INPUT_LINES     0x3F
#define ALL_RELEASED    INPUT_LINES

#define SW1 0x3E
#define SW2 0x3D
#define SW3 0x3B
#define SW4 0x37

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

void init_config(void)
{
    LED_ARRAY1 = 0x00;
    LED_ARRAY1_DDR = 0x00;
    init_keypad();
}

void glow_on_press(unsigned char key)
{
    static unsigned char flag = 0;
    static unsigned int wait = 0;
    static int i = 0;

    wait++;

    // --------- PATTERN 1 (L<->R Train) ----------
    if (key == SW1)
    {
        if (wait == 20000)
        {
            wait = 0;

            if (i < 8)
                LED_ARRAY1 = (LED_ARRAY1 << 1) | 1;

            else if (i < 16)
                LED_ARRAY1 = (LED_ARRAY1 << 1);

            else if (i < 24)
                LED_ARRAY1 = (LED_ARRAY1 >> 1) | 0x80;

            else if (i < 32)
                LED_ARRAY1 = (LED_ARRAY1 >> 1);

            else
            {
                i = 0;
                LED_ARRAY1 = 0x00;
            }

            i++;
        }
    }

    // -------- PATTERN 2 (L->R ON then OFF) --------
    else if (key == SW2)
    {
        if (wait == 20000)
        {
            wait = 0;

            if (i < 8)
                LED_ARRAY1 = (LED_ARRAY1 << 1) | 1;

            else if (i < 16)
                LED_ARRAY1 = (LED_ARRAY1 << 1);

            else
            {
                i = 0;
                LED_ARRAY1 = 0x00;
            }

            i++;
        }
    }

    // -------- PATTERN 3 (Alternate Blink) --------
    else if (key == SW3)
    {
        if (wait == 20000)
        {
            wait = 0;
            flag = !flag;

            if (flag)
                LED_ARRAY1 = 0xAA;
            else
                LED_ARRAY1 = 0x55;
        }
    }

    // -------- PATTERN 4 (Nibble Blink) --------
    else if (key == SW4)
    {
        if (wait == 20000)
        {
            wait = 0;
            flag = !flag;

            if (flag)
                LED_ARRAY1 = 0xF0;
            else
                LED_ARRAY1 = 0x0F;
        }
    }
}

void main(void)
{
    unsigned char key, key_copy = 0;

    init_config();

    while (1)
    {
        key = read_keypad();

        // Store last pressed key
        if (key != ALL_RELEASED)
        {
            key_copy = key;
        }

        glow_on_press(key_copy);
    }
}