/*------------------------------------------------------------------------------
* Instructor:   Aleksander Milenkovic
* Program:      Counting characters
* Date:         Aug 27, 2020
* Input:        None
* Output:       Counting chharacters
* Description:  This C program prints out the number of upper case, lower case and
*               digits a string has.
*-----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

int main(void){
    WDTCTL = WDTPW + WDTHOLD;     //stop the watchdog timer
    
    char string[] = "Hello! Welcome to CPE325. It’s a great day, isn’t it?";

    int i; // counter
    int count = 0; //length of the string

    //determine the length of the string
    while(string[count] != '\0'){
        count ++;
    }

    int upper_case = 0;
    int lower_case = 0;
    int digit = 0;

    for(i = 0; i < count; i++){
        if(65 <= (int)string[i] && (int)string[i] <= 90 ){
            upper_case++;
        }

        else if(97 <= (int)string[i] && (int)string[i] <= 122 ){
            lower_case++;
        }

        else if(48 <= (int)string[i] && (int)string[i] <= 57 ){
            digit++;
        }

    }

    printf("String: Hello! Welcome to CPE325. It’s a great day, isn’t it?\n"
            "Contains: %d upper case characters, %d lower case characters and %d digits\n", upper_case, lower_case, digit);

    return 0;
}
