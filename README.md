# Embedded C Projects

**Description**
This repository contains simple Embedded C programs for PIC microcontrollers.

**Projects**
1. 01_led_blink_portb.c - Blink all LEDs on PORTB
2. 02_led_alternating_pattern.c - Alternate blinking of first 4 and last 4 LEDs on PORTB
3. 03_led_blink_pattern.c - Turn ON LEDs one by one on PORTB, then turn OFF LEDs one by one from top to bottom

**Microcontroller**
- PIC series (XC8 compiler)

**Features**
- GPIO configuration using TRIS register
- Software delay using __delay_ms()
- Watchdog Timer disabled
- 
 **Notes**
- The program was written and built using **MPLAB X IDE**.  
- The program logic was tested using **PicSimLab** simulator.
