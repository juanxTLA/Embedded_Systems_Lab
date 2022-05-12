/*******************************************************************************
 * File: Lab2_D1.c
 * Description: This program demonstrates common C data types and P
 *              printf formatting options.
 *
 * Platform: EXP-MSP430F5529lp
 *
 * Author: Aleksandar Milenkovic, milenkovic@computer.org
 * Date:   August 3, 2020
 *
 ********************************************************************************/
#include <msp430.h> 
#include <stdio.h>

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    int a1[6] = {-1, 2, 5, 3, -5, 6};
    int a2[6] = {-7, 8, 23, 13, 23, 28};
    int i;
    int result = 0;

    for(i = 0; i < 6; i++){
        result += a1[i] * a2[i];
    }
    //print a1
    printf("Input array a1: [");
    for(i = 0; i < 6; i++){
        printf("%d ", a1[i]);
        }
    printf("]\n");

    //print a2
    printf("Input array a2: [");
    for(i = 0; i < 6; i++){
        printf("%d ", a2[i]);
        }
    printf("]\n");

    //print result
    printf("Dot Product is: %d", result);

    return 0;
}
