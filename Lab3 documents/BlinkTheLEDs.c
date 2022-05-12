/*******************************************************************************
 * File: BlinkTheLED.c
 * Description: This program toggles the red and green LEDs on MSP-EXP430F5529LP
 *
 * Platform: MSP-EXP430F5529LP
 * Initial conditions: the red LED is off, the green LED is on
 *                          MSP430F5529
 *                      -------------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |             P1.0|---> Red LED
 *                      |                 |
 *                      |             P4.7|---> Green LED
 *
 * Author: Aleksandar Milenkovic, milenkovic@computer.org
 * Date:   August 2, 2020
 *
 ********************************************************************************/
#include <msp430.h>

# define REDLED 0x01                // mask for BIT0=00000001b
# define GREENLED 0x80              // mask for BIT7=10000000b

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1DIR |= REDLED;                // configure P1.0 as output
    P4DIR |= GREENLED;              // configure P4.7 as output

    P1OUT = P1OUT & ~REDLED;        // P1.0 is cleared
    P4OUT = P4OUT | GREENLED;       // P4.7 is on
    while(1) {                      // main application loop
        __delay_cycles(500000);     // delay of ~250 ms
        P1OUT ^= REDLED;            // toggle P1.0
        P4OUT ^= GREENLED;          // toggle P4.7
    }
}
