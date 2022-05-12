/*------------------------------------------------------------------------------
* Instructor:   Aleksander Milenkovic
* Program:      Multiplication table for an integer
* Date:         Aug 23, 2020
* Input:        None
* Output:       Multiplication table
* Description:  This C program prints the first ten multiples of an integer. 
*-----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

void print_multiplication_table(unsigned int);

int main(void){
    WDTCTL = WDTPW + WDTHOLD;     //stop the watchdog timer
    int a = 3; //number to multiply
    printf("Multiplication table for %d:\n", a);
    print_multiplication_table(a);
    
    return 0;
}

void print_multiplication_table(unsigned int num){
    int i; // loop counter
    int mult; //result
    for(i = 1; i <= 10; i++){
        mult = num * i;
        printf("%d X %d = %d\n", num, i, mult);
    }
}
