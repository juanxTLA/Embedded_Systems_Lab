/*------------------------------------------------------------------------------
* Instructor:   Aleksander Milenkovic
* Program:      LED BLINK
* Date:         Oct 09, 2020
* Input:        None
* Output:       Table
*-----------------------------------------------------------------------------*/
#include <msp430.h> 



void configure_clock_sources();

inline void change_clock_freq_8Mhz();
inline void change_clock_freq_4Mhz();
inline void change_clock_freq_2Mhz();
inline void change_clock_freq_1Mhz();

int counter = 0; // from lower frequency to higher

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //configure leds
    P1DIR |= BIT0;
    P4DIR |= BIT7;
    P1OUT |= BIT0;
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

    configure_clock_sources();

    while(1){
        for(;;){
            switch(counter){
                case 0:
                    change_clock_freq_1Mhz();
                    break;
                case 1:
                    change_clock_freq_2Mhz();
                    break;
                case 2:
                    change_clock_freq_4Mhz();
                    break;
                case 3:
                    change_clock_freq_8Mhz();
                    break;
            }
            P1OUT ^= BIT0;
            P4OUT ^= BIT7;
            __delay_cycles(1000000);
        }

    }
}

void change_clock_freq_8Mhz()
{
     __bis_SR_register(SCG0);   // Disable the FLL control loop
     UCSCTL1 = DCORSEL_5;       // Select DCO range 16MHz operation
     UCSCTL2 = 249;             // Set DCO Multiplier for 8MHz
                                // (N + 1) * FLLRef = Fdco
                                // (249 + 1) * 32768 = 8MHz
     __bic_SR_register(SCG0);   // Enable the FLL control loop

     // Worst-case settling time for the DCO when the DCO range bits have been
     // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
     // UG for optimization.
     // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
     __delay_cycles(250000);
 }

void change_clock_freq_4Mhz()
{
     __bis_SR_register(SCG0);   // Disable the FLL control loop
     UCSCTL1 = DCORSEL_5;       // Select DCO range 16MHz operation
     UCSCTL2 = 121;             // Set DCO Multiplier for 8MHz
                                // (N + 1) * FLLRef = Fdco
                                // (121 + 1) * 32768 = 4MHz
     __bic_SR_register(SCG0);   // Enable the FLL control loop

     // Worst-case settling time for the DCO when the DCO range bits have been
     // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
     // UG for optimization.
     // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
     __delay_cycles(125000);
     }

void change_clock_freq_2Mhz()
{
    __bis_SR_register(SCG0);   // Disable the FLL control loop
    UCSCTL1 = DCORSEL_4;       // Select DCO range 16MHz operation
    UCSCTL2 = 60;             // Set DCO Multiplier for 8MHz
                               // (N + 1) * FLLRef = Fdco
                               // (121 + 1) * 32768 = 2MHz
    __bic_SR_register(SCG0);   // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
    __delay_cycles(62500);
}

void change_clock_freq_1Mhz()
{
     __bis_SR_register(SCG0); // Disable the FLL control loop
     UCSCTL1 = DCORSEL_3; // Select DCO range 1MHz operation
     UCSCTL2 = 32; // Set DCO Multiplier for 1MHz
     // (N + 1) * FLLRef = Fdco
     // (32 + 1) * 32768 = 1MHz
     __bic_SR_register(SCG0); // Enable the FLL control loop

     // Worst-case settling time for the DCO when the DCO range bits have been
     // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
     // UG for optimization.
     // 32 x 32 x 1 MHz / 32,768 Hz = 33792 = MCLK cycles for DCO to settle
     __delay_cycles(33792);
}


void configure_clock_sources()
 {
    UCSCTL3 = SELREF_2; // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2; // Set ACLK = REFO
    UCSCTL0 = 0x0000; // Set lowest possible DCOx, MODx

    // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG); // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG; // Clear fault flags
    } while (SFRIFG1&OFIFG); // Test oscillator fault flag
 }


#pragma vector = PORT2_VECTOR
 __interrupt void PORT2_ISR(void)
 {
     // let us clear the flag
     P2IFG &= ~BIT1;

     //debouncing section
     __delay_cycles(25000);

     // if SW1 is not pressed, return
     if((P2IN&BIT1)!=0x00)
     return;

    counter = counter - 1;

    if(counter < 0) counter = 0; // prevent it from going lower than 0


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

    counter = counter + 1;

    if(counter > 3) counter = 3; // prevent it from going higher than 0


 }





