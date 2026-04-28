// LED dimmer using Timer0 interrupt based PWM where SW1 increases brightness and SW2 decreases brightness

#include <xc.h>

#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)

/* ---------------------- MACROS ---------------------- */

#define LED1                RD0

#define LED_ARRAY1          PORTD
#define LED_ARRAY1_DDR      TRISD

#define ON                  1
#define OFF                 0

#define PERIOD              100

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

/* ---------------------- GLOBAL VARIABLES ---------------------- */

unsigned char duty_cycle = 50;
unsigned char loop_counter = 0;

/* ---------------------- FUNCTION PROTOTYPES ---------------------- */

void init_timer0(void);
void init_digital_keypad(void);
unsigned char read_digital_keypad(unsigned char mode);
static void init_config(void);

/* ---------------------- TIMER0 INITIALIZATION ---------------------- */

void init_timer0(void)
{
    /* Setting internal clock source */
    T0CS = 0;

    /* Assigning prescaler to Watchdog Timer */
    PSA = 1;

    /* Load timer value */
    TMR0 = 6;

    /* Enable Timer0 interrupt */
    TMR0IE = 1;
}

/* ---------------------- DIGITAL KEYPAD ---------------------- */

void init_digital_keypad(void)
{
    /* Configure keypad pins as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;

    if (mode == LEVEL_DETECTION)
    {
        return (KEYPAD_PORT & INPUT_LINES);
    }
    else
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

/* ---------------------- INTERRUPT SERVICE ROUTINE ---------------------- */

void __interrupt() isr(void)
{
    if (TMR0IF == 1)
    {
        /* Reload Timer0 value */
        TMR0 = 6;

        /* PWM generation */
        if (loop_counter < duty_cycle)
        {
            LED1 = ON;
        }
        else
        {
            LED1 = OFF;
        }

        /* Increment loop counter */
        if (loop_counter++ == PERIOD)
        {
            loop_counter = 0;
        }

        /* Clear Timer0 interrupt flag */
        TMR0IF = 0;
    }
}

/* ---------------------- INITIAL CONFIGURATION ---------------------- */

static void init_config(void)
{
    /* Configure LED pins as output */
    LED_ARRAY1_DDR = 0x00;

    /* Turn OFF all LEDs initially */
    LED_ARRAY1 = 0x00;

    /* Initialize digital keypad */
    init_digital_keypad();

    /* Initialize Timer0 */
    init_timer0();

    /* Enable Peripheral Interrupt */
    PEIE = 1;

    /* Enable Global Interrupt */
    GIE = 1;
}

/* ---------------------- MAIN FUNCTION ---------------------- */

void main(void)
{
    unsigned char key;
    unsigned int wait = 2500;

    init_config();

    while (1)
    {
        if (!wait--)
        {
            wait = 2500;

            /* Read switch input */
            key = read_digital_keypad(LEVEL);

            /* Increase brightness */
            if (key == SW1)
            {
                if (duty_cycle < PERIOD)
                {
                    duty_cycle++;
                }
            }

            /* Decrease brightness */
            else if (key == SW2)
            {
                if (duty_cycle > 0)
                {
                    duty_cycle--;
                }
            }
        }
    }
}