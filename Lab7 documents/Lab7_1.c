/*------------------------------------------------------------------------------
 * File:        Lab7_D6.c (CPE 325 Lab7 Demo code)
 *
 * Function:    Changing righness of LED1
 *
 * Description: The brightness of led 1 goes from 0 to 100 within a period of 6 seconds
 * and when sw1 is pressed it stops and when sw2 is pressed it keeps going.
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
 *
 *
 * Author:     Juan Tarrat
 *------------------------------------------------------------------------------*/
#include <msp430F5529.h>

const int CEIL = 375;

void main(void) {
    WDTCTL = WDT_MDLY_8;          // Stop WDT
    // WDTCTL = WDTPW +WDTHOLD;    // Stop WDT
    _EINT();                      // Enable interrupts

    P1DIR |= BIT0;                //LED1 as output
    P1OUT &= ~BIT0;               // ensure LED1 is off

    //sw1
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;
    P2IE |= BIT1;
    P2IES |= BIT1;
    //sw2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IE |= BIT1;
    P1IES |= BIT1;


    TA0CCTL0 = CCIE;              // TA0 count triggers interrupt
    TA0CCR0 = CEIL;              // Set TA0 (and maximum) count value

    TA0CCTL1 = CCIE;            // TA0.1 count triggers interrupt
    TA0CCR1 = 0;               // Set TA0.1 count value

    TA0CTL = TASSEL_1 | MC_1;  // ACLK is clock source, UP mode
    SFRIE1 |= WDTIE;

    _BIS_SR(LPM3);                // Enter Low Power Mode 3

}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerISR(void) {

    if(TA0CCR1 == 0){
        P1OUT &= ~BIT0;
        TA0CCTL1 &= ~CCIFG;            // Clear interrupt flag
        return;
    }
    P1OUT |= BIT0;                // Toggle LED1
    TA0CCTL1 &= ~CCIFG;            // Clear interrupt flag
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void timerISR2(void) {

    P1OUT &= ~BIT0; // Toggle LED1
 }


#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void) {
    static int a = 1;
    static int count = 0;
    TA0CCR1 += a;
    count++;
    if(count == 375){
        a = a*(-1);
        count = 0;
    }
}

#pragma vector = PORT2_VECTOR
 __interrupt void PORT2_ISR(void)
 {
     // let us clear the flag
     P2IFG &= ~BIT1;
     //debouncing section
     __delay_cycles(2500);
     WDTCTL = WDTPW + WDTHOLD; // stop watchdg timer
     SFRIE1 &= ~WDTIFG; // clear interrupt flag
 }

#pragma vector = PORT1_VECTOR
 __interrupt void PORT1_ISR(void)
 {
     // let us clear the flag
     P1IFG &= ~BIT1;
     //debouncing section
     __delay_cycles(2500);
     WDTCTL = WDT_MDLY_8;  // set watchdog timer again
     SFRIE1 |= WDTIE;   // set flag bit again

 }

