/*------------------------------------------------------------------------------
 * File:        Lab9.c (CPE 325 Lab10 Demo code)
 * Function:    Interfacing accelerometer (MPS430F5529)
 * Description: This C program interfaces with an accelerometer and plots the data
 *
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 *
 *                         MSP430F5529
 *                      -------------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |     P3.3/UCA0TXD|------------>
 *                      |                 | 38400 - 8N1
 *                      |     P3.4/UCA0RXD|<------------
 *                      |                 |
 * Input:       Connect thumbstick to the board
 * Output:      Displays gs
 * Author:      Juan Tarrat
 *------------------------------------------------------------------------------*/

#include <msp430.h>
#include <math.h>
//define constants
volatile int ADCXval, ADCYval, ADCZval;
int on = 1;
volatile float Xper, Yper, Zper, gnet;
const float g = 9.8;
int net = 0;
//setup
void TimerA_setup(void) {
    TA0CCR0 = 3277;                      // 3277 / 32768 Hz = 0.1s
    TA0CTL = TASSEL_1 + MC_1;            // ACLK, up mode
    TA0CCTL0 = CCIE;                     // Enabled interrupt
}

void ADC_setup(void) {
    int i =0;

    P6DIR &= ~BIT0 + ~BIT1 + ~BIT2;             // Configure P6.4 and P6.4 as input pins
    P6SEL |= BIT0 + BIT1 + BIT2;               // Configure P6.3 and P6.4 as analog pins
    // configure ADC converter
    ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_8;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1;         // Use sample timer, single sequence

    ADC12MCTL0 = ADC12INCH_0;                       // ADC A0 pin -  X-axis
    ADC12MCTL1 = ADC12INCH_1;                       // ADC A2 pin -  Y-axis
    ADC12MCTL2 = ADC12INCH_2 + ADC12EOS;            //ADC A1 pin - Z axis
                                        // EOS - End of Sequence for Conversions
    ADC12IE |= 0x02;                    // Enable ADC12IFG.1
    for (i = 0; i < 0x3600; i++);       // Delay for reference start-up
    ADC12CTL0 |= ADC12ENC;              // Enable conversions
}

void UART_putCharacter(char c) {
    while (!(UCA0IFG&UCTXIFG));    // Wait for previous character to transmit
    UCA0TXBUF = c;                  // Put character into tx buffer
}

void UART_setup(void) {
    P3SEL |= BIT3 + BIT4;               // Set up Rx and Tx bits
    UCA0CTL1 |= UCSWRST;
    UCA0CTL0 = 0;                       // Set up default RS-232 protocol
    UCA0CTL1 |= UCSSEL_2;        // Disable device, set clock
    UCA0BR0 = 0x09;                       // 1048576 Hz / 38400
    UCA0BR1 = 0;
    UCA0MCTL |= UCBRS0;
    UCA0CTL1 &= ~UCSWRST;                  // Start UART device
}

void sendData(void) {
    int i;
    Xper = ((((float)ADCXval/4095*3.0)-1.5)/0.3);    // Calculate percentage outputs
    Yper = ((((float)ADCYval/4095*3.0)-1.5)/0.3);;
    Zper = ((((float)ADCZval/4095*3.0)-1.5)/0.3);;

    gnet = sqrt(Xper*Xper + Yper*Yper + Zper*Zper);



    // Use character pointers to send one byte at a time
    char *xpointer = (char *)&Xper;
    char *ypointer = (char *)&Yper;
    char *zpointer = (char *)&Zper;

    UART_putCharacter(0x55);            // Send header
    for(i = 0; i < 4; i++) {            // Send x percentage - one byte at a time
        UART_putCharacter(xpointer[i]);
    }
    for(i = 0; i < 4; i++) {            // Send y percentage - one byte at a time
        UART_putCharacter(ypointer[i]);
    }
    for(i = 0; i < 4; i++) {            // Send y percentage - one byte at a time
       UART_putCharacter(zpointer[i]);
    }
}
//turn leds on and enable watchdog timer
void turn_LED_on(int a){
    if(a == 1){
        P1OUT |= BIT0;
        P4OUT |= BIT7;
        WDTCTL = WDT_ADLY_250; // 250 ms
        SFRIE1 |= WDTIE;
    }
}

void main(void) {
    WDTCTL = WDTPW +WDTHOLD;            // Stop WDT
    __enable_interrupt();
    TimerA_setup();                     // Setup timer to send ADC data
    ADC_setup();                        // Setup ADC
    UART_setup();                       // Setup UART for RS-232

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
    //switches
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

    while (1){
                                   // Send data to serial app
        if(gnet >= 7){
           turn_LED_on(on);
           on = 0;
           net = 1;
        }
        __bis_SR_register(LPM0_bits + GIE);   // Enter LPM0
        sendData();

    }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    ADCXval = ADC12MEM0;                  // Move results, IFG is cleared
    ADCYval = ADC12MEM1;
    ADCZval = ADC12MEM2;
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerA_isr() {
    ADC12CTL0 |= ADC12SC;
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void) {
    static int a = 1;
    a++;
    if(a == 2){
        P1OUT ^= BIT0;
        P4OUT ^= BIT7;
        a = 0;
    }
}

#pragma vector = PORT2_VECTOR
 __interrupt void PORT2_ISR(void)
 {
     // let us clear the flag

     P2IFG &= ~BIT1;
     P1OUT &= ~BIT0;
     P4OUT &= ~BIT7;
     on = 1;
     net = 0;
     WDTCTL = WDTPW + WDTHOLD;

 }

#pragma vector = PORT1_VECTOR
 __interrupt void PORT1_ISR(void)
 {
     // let us clear the flag
     P1IFG &= ~BIT1;
     //debouncing section
     //__delay_cycles(250);

     P1OUT &= ~BIT0;
     P4OUT &= ~BIT7;
     on = 1;
     net = 0;
     WDTCTL = WDTPW + WDTHOLD;

 }
