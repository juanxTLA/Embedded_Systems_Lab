/*------------------------------------------------------------------------------
 * File:        Lab7_D6.c (CPE 325 Lab7 Demo code)
 *
 * Function:    Blinking LED1 & LED2 using Timer_A0 with interrupts (MPS430F5529)
 *
 * Description: In this C program, Timer_A0 is configured for up/down mode with
 *              ACLK source and interrupts for channel 0 and channel 1 are
 *              enabled. In up/down mode timer TA0 counts the value from 0 up to
 *              value stored in TA0CCR0 and then counts back to 0. The interrupt
 *              for TA0 is generated when the counter reaches value in TA0CCR0.
 *              The interrupt TA0.1 is generated whenever the counter reaches value
 *              in TA0CCR1. Thus, TA0.1 gets two interrupts while counting upwards
 *              and counting downwards. This simulates a PWM control - adjusting
 *              the TA0.1 and TA0.0 CCR register values adjusts the duty cycle of the
 *              PWM signal.
 *
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (2^20 Hz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 *
 *                          MSP430xG461x
 *                       -----------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |             P1.0|--> LED1(RED)
 *                      |             P4.7|--> LED2(GREEN)
 * Input:       None
 * Output:      LED1 blinks at 1.64Hz with 20-80% duty cycle and LED2 blinks at
 *              0.82Hz with 50-50% duty cycle.
 *
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *              Prawar Poudel
 *------------------------------------------------------------------------------*/
#include <msp430F5529.h>

void main(void) {
    WDTCTL = WDT_MDLY_0_5;          //0.5 ms
    //WDTCTL = WDTPW +WDTHOLD;        // Stop WDT
    _EINT();                      // Enable interrupts

    P1DIR |= BIT0;                //LED1 as output
    P1OUT &= ~BIT0;               // ensure LED1 is off

    P2IE |= BIT1;
    P2IES |= BIT1;

    SFRIE1 |= WDTIE; // watchdog interrupt
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void) {
    static int count = 0;
    count++;
    if(count == 1000){
      P2IFG |= BIT1;
      count = 0;
    }
}

#pragma vector = PORT2_VECTOR
 __interrupt void PORT2_ISR(void)
 {
     // let us clear the flag
     P2IFG &= ~BIT1;
     P1OUT ^= BIT0;
 }

