/*------------------------------------------------------------------------------
 * File:        Lab3_D3.c (CPE 325 Lab3 Demo code)
 * Function:    Turning on LED1 when S1 is pressed (MSP-EXP430F5529LP)
 * Description: This C program turns on LED1 connected to P1.0 when the S1 is
 *              pressed. S1 is connected to P2.1 and when it is pressed,
 *              P2IN, bit 1 is read as a logic 0 (check the schematic).
 *              To avoid faulty detection of button press,
 *              debouncing delay of 20ms is added before turning on the LED1.
 * Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO (~1 MHz)
 *
 *                           MSP430F552x
 *                       -----------------
 *                   /|\|                 |
 *                    | |                 |
 *                    --|RST              |
 *                      |             P1.0|-->LED1(RED)
 *                      |             P2.1|<--S1
 *                      |                 |
 * Input:       Press S1
 * Output:      LED1 is turned on when S1 is pressed
 * Authors:     Aleksandar Milenkovic, milenkovic@computer.org
 *              Prawar Poudel
 *------------------------------------------------------------------------------*/
#include <msp430.h>

#define S1 P2IN&BIT1

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
    P1DIR |= BIT0;                        // Set P1.0 to output direction (0000_0001)
    P1OUT &= ~BIT0;                       // LED1 is OFF

    P2DIR &= ~BIT1;                       // Set P2.1 as input for S1 input
    P2REN |= BIT1;                        // Enable the Pull-up resistor at P2.1
    P2OUT |= BIT1;                        // Required for proper IO

    unsigned int i = 0;
    for (;;) {                            // Infinite loop
        if ((S1) == 0) {                  // If S1 is pressed
            for (i = 2000; i > 0; i--);   // Debounce ~20 ms
            if ((S1) == 0)
            {
                P1OUT |= BIT0;            // S1 pressed, turn LED1 on
            }
            while ((S1) == 0);            // Hang-on as long as S1 pressed
        } else
            P1OUT &= ~BIT0;
    }
}
