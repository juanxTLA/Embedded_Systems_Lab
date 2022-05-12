/*------------------------------------------------------------------------------
* Instructor:   Aleksander Milenkovic
* Program:      Print table
* Date:         Sept 03, 2020
* Input:        None
* Output:       Table
* Description:  This C program prints out the size of different data types and
* their maximum and minimum values
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
#include <limits.h>
#include <float.h>

const int LENGTH = 81;

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    long long int signed_data[5][3] = {
                    {sizeof(char), SCHAR_MIN, SCHAR_MAX},
                    {sizeof(short int), SHRT_MIN, SHRT_MAX},
                    {sizeof(int), INT_MIN, INT_MAX},
                    {sizeof(long int), LONG_MIN, LONG_MAX},
                    {sizeof(long long int), LLONG_MIN, LLONG_MAX},
    };


   unsigned long long int unsigned_data[5][3] = {
                            {sizeof(unsigned char), 0, UCHAR_MAX},
                            {sizeof(unsigned short int), 0, USHRT_MAX},
                            {sizeof(unsigned int), 0, UINT_MAX},
                            {sizeof(unsigned long int), 0, ULONG_MAX},
                            {sizeof(unsigned long long int), 0, ULLONG_MAX},
    };

   double decimal_data[2][3] = {
                            {sizeof(float), FLT_MIN, FLT_MAX},
                            {sizeof(double), DBL_MIN, DBL_MAX},
   };

    int i;
    for(i = 0; i < LENGTH; i++) putchar('-');
    printf("\n");
    printf("|%-22s | %-7s | %-20s | %-20s |\n", "Data Type", "Bytes", "Minimum", "Maximum");
    for(i = 0; i < LENGTH; i++) putchar('-');
    printf("\n");
    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "char", signed_data[0][0], signed_data[0][1], signed_data[0][2]);
    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "short int", signed_data[1][0], signed_data[1][1], signed_data[1][2]);
    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "int", signed_data[2][0], signed_data[2][1], signed_data[2][2]);
    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "long int", signed_data[3][0], signed_data[3][1], signed_data[3][2]);
    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "long long int", signed_data[4][0], signed_data[4][1], signed_data[4][2]);

    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "unsigned char", unsigned_data[0][0], unsigned_data[0][1], unsigned_data[0][2]);
    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "unsigned short int", unsigned_data[1][0], unsigned_data[1][1], unsigned_data[1][2]);
    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "unsigned int", unsigned_data[2][0], unsigned_data[2][1], unsigned_data[2][2]);
    printf("|%-22s | %-7lld | %20lld | %-20lld |\n", "unsigned long int", unsigned_data[3][0], unsigned_data[3][1], unsigned_data[3][2]);
    printf("|%-22s | %-7lld | %20lld | %-20llu |\n", "unsigned long long int", unsigned_data[4][0], unsigned_data[4][1], unsigned_data[4][2]);

    printf("|%-22s | %-7d | %20e | %-20e |\n", "float", (int)decimal_data[0][0], decimal_data[0][1], decimal_data[0][2]);
    printf("|%-22s | %-7d | %20e | %-20e |\n", "double", (int)decimal_data[1][0], decimal_data[1][1], decimal_data[1][2]);

    for(i = 0; i < LENGTH; i++) putchar('-');
    return 0;
}
