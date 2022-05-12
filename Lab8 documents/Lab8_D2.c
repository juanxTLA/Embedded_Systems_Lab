/*----------------------------------------------------------------------------------
 * File:          Lab8_D2.c
 *
 * Function:      Echo a received character, using receiver ISR.
 * Description:   This program echos the character received from UART back to UART.
 *                Toggle LED1 with every received character.
 *                Baud rate: low-frequency (UCOS16=0);
 *                1048576/115200 = ~9.1 (0x0009|0x01)
 * Clocks:        ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO
 *
 * Instructions: Set the following parameters in putty
 * Port: COMx
 * Baud rate: 115200
 * Data bits: 8
 * Parity: None
 * Stop bits: 1
 * Flow Control: None
 *
 *        MSP430f5529
 *     -----------------
 * /|\ |            XIN|-
 *  |  |               | 32kHz
 *  |--|RST        XOUT|-
 *     |               |
 *     |   P3.3/UCA0TXD|------------>
 *     |               | 115200 - 8N1
 *     |   P3.4/UCA0RXD|<------------
 *     |           P1.0|----> LED1
 *
 * Input:     None (Type characters in putty/MobaXterm/hyperterminal)
 * Output:    Character echoed at UART
 * Author:    A. Milenkovic, milenkovic@computer.org
 * Date:      October 2018
 *--------------------------------------------------------------------------------*/
#include <msp430.h>

// Initialize USCI_A0 module to UART mode
void UART_setup(void) {

    P3SEL |= BIT3 + BIT4;   // Set USCI_A0 RXD/TXD to receive/transmit data
    UCA0CTL1 |= UCSWRST;    // Set software reset during initialization
    UCA0CTL0 = 0;           // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;   // Clock source SMCLK

    UCA0BR0 = 0x09;         // 1048576 Hz  / 115200 lower byte
    UCA0BR1 = 0x00;         // upper byte
    UCA0MCTL |= UCBRS0;     // Modulation (UCBRS0=0x01, UCOS16=0)

    UCA0CTL1 &= ~UCSWRST;   // Clear software reset to initialize USCI state machine
    UCA0IE |= UCRXIE;       // Enable USCI_A0 RX interrupt
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;// Stop WDT
    P1DIR |= BIT0;           // Set P1.0 to be output
    UART_setup();            // InitiAlize USCI_A0 in UART mode

    _BIS_SR(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector = USCI_A0_VECTOR
__interrupt void USCIA0RX_ISR (void) {
    while(!(UCA0IFG&UCTXIFG));  // Wait until can transmit
    UCA0TXBUF = UCA0RXBUF;      // TXBUF <-- RXBUF
    P1OUT ^= BIT0;              // Toggle LED1
}
