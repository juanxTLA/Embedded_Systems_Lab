/*------------------------------------------------------------------------------
 * File:        Lab10_D2.c (CPE 325 Lab10 Demo code)
 * Function:    Interfacing thumbstick (MPS430F5529)
 * Description: This C program interfaces with a thumbstick sensor that has
 *              x (HORZ) and y (VERT) axis and outputs from 0 to 3V.
 *              The value of x and y axis
 *              is sent as the percentage of power to the UAH Serial App.
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
 * Output:      Displays % of power in UAH serial app
 * Author:      Micah Harvey
 *              Prawar Poudel
 *------------------------------------------------------------------------------*/

#include <msp430.h>

volatile long int ADCXval, ADCYval;
volatile float Xper, Yper;

void TimerA_setup(void) {
    TA0CCR0 = 3277;                      // 3277 / 32768 Hz = 0.1s
    TA0CTL = TASSEL_1 + MC_1;            // ACLK, up mode
    TA0CCTL0 = CCIE;                     // Enabled interrupt
}

void ADC_setup(void) {
    int i =0;

    P6DIR &= ~BIT3 + ~BIT4;             // Configure P6.4 and P6.4 as input pins
    P6SEL |= BIT3 + BIT4;               // Configure P6.3 and P6.4 as analog pins
    // configure ADC converter
    ADC12CTL0 = ADC12ON + ADC12SHT0_6 + ADC12MSC_L;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ1;         // Use sample timer, single sequence
    ADC12MCTL0 = ADC12INCH_3;                // ADC A3 pin - Stick X-axis
    ADC12MCTL1 = ADC12INCH_4 + ADC12EOS;          // ADC A4 pin - Stick Y-axis
                                        // EOS - End of Sequence for Conversions
    ADC12IE |= 0x02;                    // Enable ADC12IFG.1
    for (i = 0; i < 0x3600; i++);       // Delay for reference start-up
    ADC12CTL0 |= ADC12ENC;                   // Enable conversions
}

void UART_putCharacter(char c) {
    while (!(UCA0IFG&UCTXIFG));    // Wait for previous character to transmit
    UCA0TXBUF = c;                  // Put character into tx buffer
}

void UART_setup(void) {
    P3SEL |= BIT3 + BIT4;               // Set up Rx and Tx bits
    UCA0CTL0 = 0;                       // Set up default RS-232 protocol
    UCA0CTL1 |= BIT0 + UCSSEL_2;        // Disable device, set clock
    UCA0BR0 = 27;                       // 1048576 Hz / 38400
    UCA0BR1 = 0;
    UCA0MCTL = 0x94;
    UCA0CTL1 &= ~BIT0;                  // Start UART device
}

void sendData(void) {
    int i;
    Xper = (ADCXval*3.0/4095*100/3);    // Calculate percentage outputs
    Yper = (ADCYval*3.0/4095*100/3);
    // Use character pointers to send one byte at a time
    char *xpointer=(char *)&Xper;
    char *ypointer=(char *)&Yper;

    UART_putCharacter(0x55);            // Send header
    for(i = 0; i < 4; i++) {            // Send x percentage - one byte at a time
        UART_putCharacter(xpointer[i]);
    }
    for(i = 0; i < 4; i++) {            // Send y percentage - one byte at a time
        UART_putCharacter(ypointer[i]);
    }
}

void main(void) {
    WDTCTL = WDTPW +WDTHOLD;            // Stop WDT
    TimerA_setup();                     // Setup timer to send ADC data
    ADC_setup();                        // Setup ADC
    UART_setup();                       // Setup UART for RS-232
    _EINT();

    while (1){
        ADC12CTL0 |= ADC12SC;               // Start conversions
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0
    }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    ADCXval = ADC12MEM0;                  // Move results, IFG is cleared
    ADCYval = ADC12MEM1;
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerA_isr() {
    sendData();                           // Send data to serial app
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}
