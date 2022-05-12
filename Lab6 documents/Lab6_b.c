/*------------------------------------------------------------------------------
* Instructor:   Aleksander Milenkovic
* Program:      Dot product
* Date:         Sept 03, 2020
* Input:        None
* Output:       Table
* Description:  This C program performs and prints the dot product of two arrays
*-----------------------------------------------------------------------------*/
#include <msp430.h> 

unsigned int led1 = 0;
unsigned int led2 = 0;
unsigned int on = 1; // flag for led1

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //configure leds
    P1DIR |= BIT0;
    P4DIR |= BIT7;
    P1OUT &= ~BIT0;
    P4OUT &= ~BIT7;
    //interrupt
    _EINT();
    //configure switches
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;
    P2IE |= BIT1;
    P2IES |= BIT1;
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IE |= BIT1;
    P1IES |= BIT1;
    // program

    while(1){

        if(led1 == 1 & led2 == 1){
            P1OUT |= BIT0;
            P4OUT |= BIT7;
            P2IES &= ~BIT1;
            P1IES &= ~BIT1;
            on = 0;//set it to false
            }



        else if (led1 == 1 & led2 == 0){
            P1OUT ^= BIT0;
            led1 = 0;
            on = 1;

            }

        else if (led2 == 1 & led1 == 0){
            int i;
            for(i = 5; i > 0; i--){
                P4OUT ^= BIT7;
                __delay_cycles(500000);

            }
            led2 = 0;
            if(P1OUT&BIT0 == 1){
                P4OUT |= BIT7;
            }
            else P4OUT &= ~BIT7;
        }

       if(led1 == 0 & led2 == 0 & on == 0){
               P1OUT &= ~BIT0;
               P4OUT &= ~BIT7;
         }




    }
}

#pragma vector = PORT2_VECTOR
 __interrupt void PORT2_ISR(void)
 {
     // let us clear the flag
     P2IFG &= ~BIT1;
     led1 = 0;
     led2 = 0;
     P1IES |= BIT1;
     P2IES |= BIT1;

     //debouncing section
     __delay_cycles(25000);

     // if SW1 is not pressed, return
     //if(P2IES&BIT1 == 1){
         if((P2IN&BIT1)!=0x00)
          return;
         led2 = 1;
 }

#pragma vector = PORT1_VECTOR
 __interrupt void PORT1_ISR(void)
 {
     // let us clear the flag
     P1IFG &= ~BIT1;

     //debouncing section
     __delay_cycles(25000);

     // if SW1 is not pressed, return
     if((P1IN&BIT1)!=0x00)
          return;
     led1 = 1;
     on ^= 1;

 }





