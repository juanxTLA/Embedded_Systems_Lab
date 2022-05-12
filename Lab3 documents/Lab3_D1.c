/*------------------------------------------------------------------------------
 * File:        Lab3_D1.c (CPE 325 Lab3 Demo code)
 * Function:    Turning on LED1 (EXP-MSP430F5529)
 * Description: This C program turns on LED1 connected to P1.0 by writing 1
 *              (P1.0 = 1).
 * Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO (~1 MHz)
 *
 *                           MSP430F552x
 *                       -----------------
 *                   /|\|                 |
 *                    | |                 |
 *                    --|RST              |
 *                      |             P1.0|-->LED1(RED)
 *                      |                 |
 * Input:       None
 * Output:      LED1 is turned on
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *              Prawar Poudel
 *------------------------------------------------------------------------------*/
#include  <msp430.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
    P1DIR |= 0x01;              // Set P1.0 to output direction (0000_0001)
    P1OUT |= 0x01;              // Set P1OUT to 0000_0001b (LED1 is ON)
    for (;;);                   // Infinite loop
}
