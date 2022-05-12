/*------------------------------------------------------------------
* Instructor:  Aleksandar Milenkovic
* Program:     This program calculates the power of a given
*              base using recursive function call.
* Date:        Aug 16, 2017
* Input:       None
* Output:      Base raised to the given power.
* Description: This C code will calculate the power of a given base.
*-------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include "msp430.h"

//function prototypes
long int exponential(int, int);

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;     //stop the watchdog timer

    int base = 12;        //the base
    int power = 4;        //the exponent, must be positive and 1 or greater
    long int answer = exponential(base, power);  //find the power

    printf("%d raised to the %d power is %ld", base, power, answer);

    return 0;
}

//This function recursively calculates a power given a base and a power
long int exponential(int base, int power)
{
    long int answer;      //the solution
    if (power > 1)        //if the current call has power > 1 than call again
    {
        power--;
        answer = base*exponential(base, power);
    }

    else                  //else the power = 1 and the answer is the base
    {
        answer = base;
    }

    return answer;        //return the answer
}