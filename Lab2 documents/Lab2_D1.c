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
    int i1 = 11, i2 = -6;           // 16-bit integers
    unsigned int u1 = 65535;        // unsigned 16-bit integer
    long int l1 = 100000;           // 32-bit signed integers
    long int l2 = -2;
    char c1 = 'A';                  // 8-bit character
    float f1 = 1.25;                // single-precision floating-point
    unsigned long int * p1 = &f1;   // p1 points to memory where f1 is stored
    unsigned long int l3 = *p1;     // interpret floating-point number as long integer

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    printf("Each variable below is printed in decimal, hex, and octal representations.\n");
    printf(" i1 = %6d, %#04.4x, %#06o\n", i1, i1, i1);
    printf(" i2 = %6d, %#04.4x, %#06o\n", i2, i2, i2);
    printf(" u1 = %6u, %#04.4x, %#06o\n", u1, u1, u1);
    printf(" l1 = %ld, %#08.8lx, %#011lo\n", l1, l1, l1);
    printf(" l2 = %6ld, %#08.8lx, %#011lo\n", l2, l2, l2);
    printf(" f1 = %6.2f, %#08.8lx\n", f1, l3);
    printf(" c1 = %c, %#02.2x, %#03o\n", c1, c1, c1);
    printf("That's all folks\n");

    return 0;
}
