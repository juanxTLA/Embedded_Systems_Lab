/*------------------------------------------------------------------------------
 * File:        Lab3_1.c (CPE 325 Lab3 code)
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
 * Output:      Leds blink at different frequencies depending on the switches pressed
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *              Prawar Poudel
 *------------------------------------------------------------------------------*/

#include <stdio.h>
#include <msp430.h>

#define S1 P2IN&BIT1
#define S2 P1IN&BIT1

#define REDLED 0x01    // mask for BIT0=00000001b
#define GREENLED 0x80  // mask for BIT7=10000000b



int main(void) {

    WDTCTL = WDTPW | WDTHOLD;     // stop watchdog timer
    //set up leds
    P1DIR |= REDLED;             // set LED1 to output
    P4DIR |= GREENLED;          // set LED 2 to output
    P1OUT &= ~REDLED;          // LED 1 off
    P4OUT |= GREENLED;        //LED 2 is on
    //set up switches
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    unsigned long long int i;

    while(1){
        if((S1) == 0){
            for (i = 2000; i > 0; i--); // Debounce
            while((S1) == 0){
            P1OUT ^= REDLED; //alternate red led
            P4OUT &= ~GREENLED; // turn off led 2
            for(i = 0; i <  25000; i++); //2Hz (it takes 40 clock cycles for 1 for loop)

            if((S1) == 0 && (S2)==0){
                 for (i = 2000; i > 0; i--); // Debounce
                 P1OUT &= ~REDLED;          // LED 1 off
                 P4OUT |= GREENLED;        //LED 2 is on
                 while((S1) == 0 && (S2) == 0){
                     for(i = 0; i <  100000; i++); //8Hz
                     //alternate both leds
                     P1OUT ^= REDLED;
                     P4OUT ^= GREENLED;
                 }
                }
          }
        }

        else if((S2) == 0){
            for (i = 2000; i > 0; i--); // Debounce
            P1OUT &= ~REDLED;          // LED 1 off
            P4OUT |= GREENLED;        //LED 2 is on
            while((S2) == 0){
                P1OUT |= REDLED;//red led on
                P4OUT ^= GREENLED;//alternate green led
                for(i = 0; i <  62500; i++); //5Hz
                if((S1) == 0 && (S2)==0){
                     for (i = 2000; i > 0; i--); // Debounce ~20 ms
                     while((S1) == 0 && (S2) == 0){
                         for(i = 0; i <  100000; i++); //8Hz
                         //alternate both leds
                         P1OUT ^= REDLED;
                         P4OUT ^= GREENLED;
                     }
                }
            }


        }
        //case both switches are pressed at the same time
        else if((S1) == 0 && (S2)==0){
            for (i = 2000; i > 0; i--); // Debounce ~20 ms
            while((S1) == 0 && (S2) == 0){
                for(i = 0; i <  100000; i++); //8Hz
                P1OUT ^= REDLED;
                P4OUT ^= GREENLED;
          }
        }
        //default case
        else{
            P1OUT &= ~REDLED;          // LED 1 off
            P4OUT |= GREENLED;        //LED 2 is on

        }

    }





    return 0;
}
