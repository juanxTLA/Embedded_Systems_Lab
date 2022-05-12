/*------------------------------------------------------------------------------
 * File:        Lab3_D2.c (CPE 325 Lab3 Demo code)
 * Function:    Blinking LED1 and LED2 (MSP-EXP430F5529LP)
 * Description: This C program toggle LED1 and LED2 at 1Hz by xoring P1.0 and
 *              P4.7 inside a loop. The LEDs are on when P1.0=1 and P4.7=1.
 *              The LED1 is initialized to be off and LED2 to be on.
 * Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO (~1 MHz)
 *
 *                           MSP430F552x
 *                       -----------------
 *                   /|\|                 |
 *                    | |                 |
 *                    --|RST              |
 *                      |             P1.0|-->LED1(RED)
 *                      |             P4.7|-->LED2(GREEN)
 *                      |                 |
 * Input:       None
 * Output:      LED1 and LED2 blink alternately at 1 Hz frequency
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *              Prawar Poudel
 *------------------------------------------------------------------------------*/
#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
    P1DIR |= BIT0;              // Set P1.0 to output direction
    P4DIR |= BIT7;              // Set P4.7 to output direction
    P1OUT &= ~BIT0;             // LED2 is OFF
    P4OUT |= BIT7;              // LED1 is ON
    unsigned int i = 0;
    while(1) {                       // Infinite loop
        for (i = 0; i < 50000; i++); // Delay 0.5s
                                     // 0.5s on, 0.5s off => 1/(1s) = 1Hz
        P1OUT ^= BIT0;               // Toggle LED1
        P4OUT ^= BIT7;               // Toggle LED1
    }
}
