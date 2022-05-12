/*----------------------------------------------------------------------------------
 * File:          Lab8.c
 *
 * Function:      Small interface.
 * Description:   This program echos the character received from UART back to UART.
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
 * Output:    Character echoed at UART and menus too
 * Author:    Juan Tarrat
 * Date:      October 2020
 *--------------------------------------------------------------------------------*/
#include <msp430.h>
#include <string.h>
#include <stdbool.h>

const int LEN = 10; //global length of arrays
char NEWLINE[2] = "\r\n";
char RED [LEN] = "\033[0;31m";
char YELLOW [LEN] = "\033[0;33m";
char GREEN [LEN] = "\033[0;32m";


bool sym = false;
bool credentials = false;

char input[LEN];
struct Member{
    char username[LEN];
    char password[LEN];
};

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

void UART_putCharacter(char c[], int length){

    int i = 0;
    for(i = 0; i < length; i++){
        UCA0TXBUF = c[i];
        __delay_cycles(100);
    }
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;// Stop WDT
    UART_setup();            // InitiAlize USCI_A0 in UART mode
    bool loop = false;
    int i;
    int current_user_pass;

    struct Member memb1 = {"juan", "hardware"};
    struct Member memb2 = {"javier", "films"};
    struct Member current_user;

    struct Member members[LEN] = {memb1, memb2}; //list of current members
    int memLength = 2;
    struct Member new_member = {"d", "d"};

    UART_putCharacter(YELLOW, 10);

    //start interface
    while (!loop){
        UART_putCharacter(YELLOW, 10);
        char user[] = "\nPlease enter your user name or press 1 to create a new user: ";
        int l = sizeof (user)/sizeof (user[0]);
        UART_putCharacter(user, l);
        UART_putCharacter(GREEN, 10);
        _BIS_SR(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
        //create new user
        if(input[0] == '@'){
            bool exists = false;

            do{
                exists = false;
                UART_putCharacter(YELLOW, 10);
                char user_name[] = "\r\nEnter user name: ";
                int size = sizeof (user_name)/sizeof (user_name[0]);
                UART_putCharacter(user_name, size);
                UART_putCharacter(GREEN, 10);
                _BIS_SR(LPM0_bits + GIE); // Enter LPM0, interrupts enabled


                //first check if member already exists
                for(i = 0; i < memLength; i++){
                    if (strcmp(members[i].username, input) == 0) exists = true;
                }

                if(exists){
                    UART_putCharacter(YELLOW, 10);
                    char error[] = "\r\nName already exists, try again";
                    int s = sizeof (error)/sizeof (error[0]);
                    UART_putCharacter(error, s);

                }
            } while(exists);



            strcpy(new_member.username,input);

            UART_putCharacter(YELLOW, 10);
            char user[] = "\r\nEnter password: ";
            int l = sizeof (user)/sizeof (user[0]);
            UART_putCharacter(user, l);
            UART_putCharacter(GREEN, 10);
            _BIS_SR(LPM0_bits + GIE); // Enter LPM0, interrupts enabled

            strcpy(new_member.password,input);
            members[memLength] = new_member;
            memLength++;

            }
        //login
        else{
            //check usernames
            bool checkn = false;
            for(i = 0; i < memLength; i++){
                if (strcmp(members[i].username, input) == 0) checkn = true;
            }
            memset(input, 0, strlen(input));
            //input password
            UART_putCharacter(YELLOW, 10);
            char pass[] = "\nEnter your password: ";
            l = sizeof (pass)/sizeof (pass[0]);
            UART_putCharacter(pass, l);

            sym = true;
            WDTCTL = WDT_ADLY_1000;
            TA0CTL = TASSEL_1 | MC_1;  // ACLK is clock source, UP mode
            SFRIE1 |= WDTIE;
            UART_putCharacter(GREEN, 10);
            _BIS_SR(LPM0_bits + GIE); // Enter LPM0, interrupts enabled

            //check passwords
            bool checkp = false;
            for(i = 0; i < memLength; i++){
                if (strcmp(members[i].password, input) == 0) {
                    current_user_pass = i;
                    checkp = true;
                }
            }

            memset(input, 0, strlen(input));
            //output
            if(checkp == true && checkn == true) {
                UART_putCharacter(YELLOW, 10);
                char correct[] = "\nWelcome to CPE 325";
                l = sizeof (correct)/sizeof (correct[0]);
                UART_putCharacter(correct, l);
                loop = true;
                sym = false;
                current_user = members[current_user_pass];
            }

            else{
                UART_putCharacter(RED, 10);
                char incorrect[] = "\nIncorrect user name or password or timeout\r";
                l = sizeof (incorrect)/sizeof (incorrect[0]);
                UART_putCharacter(incorrect, l);
                sym = false;
                UART_putCharacter(YELLOW, 10);
            }
        }
    }

    char change[] = "\r\nIf you want to change your password press c: ";
    int l = sizeof (change)/sizeof (change[0]);
    UART_putCharacter(change, l);
    credentials = true;
    UART_putCharacter(GREEN, 10);
    _BIS_SR(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
    if(input[0] == '@'){
        UART_putCharacter(YELLOW, 10);
        char change[] = "\nEnter new password and press enter. It shouldn't be more than 10 characters long and no special characters: ";
        l = sizeof (change)/sizeof (change[0]);
        UART_putCharacter(change, l);
        memset(input, 0, strlen(input));
        UART_putCharacter(GREEN, 10);
        _BIS_SR(LPM0_bits + GIE); // Enter LPM0, interrupts enabled

        UART_putCharacter(YELLOW, 10);
        strcpy(current_user.password,input);
        UART_putCharacter(NEWLINE, 2);
        char final[] = "New password is: ";
        l = sizeof (final)/sizeof (final[0]);
        UART_putCharacter(final, l);
        UART_putCharacter(current_user.password, 10);
    }

}
// Echo back RXed character, confirm TX buffer is ready first
#pragma vector = USCI_A0_VECTOR
__interrupt void USCIA0RX_ISR (void) {
    static int count = 0;
    while(!(UCA0IFG&UCTXIFG)); // Wait until can transmit
    if (UCA0RXBUF == '\r'){
        _BIC_SR_IRQ(LPM0_bits);
        UCA0TXBUF = UCA0RXBUF;
        count = 0;
    }

    else if(credentials == true && UCA0RXBUF == 'c'){
        input[0] = '@';
        UCA0TXBUF = UCA0RXBUF;
    }

    else if(UCA0RXBUF == '1'){
        input[0] = '@';
        UCA0TXBUF = UCA0RXBUF;
    }
    else{
        input[count] = UCA0RXBUF;
        if(sym == true){
            UCA0TXBUF = '*';
            count++;
            WDTCTL = WDTPW + WDTHOLD;
            if(count == 9){
                UCA0TXBUF = '\r';
                _BIC_SR_IRQ(LPM0_bits);
                count = 0;
            }
        }
        else if(count == 9){
            UCA0TXBUF = '\r';
            _BIC_SR_IRQ(LPM0_bits);
            count = 0;
        }
        else{
            UCA0TXBUF = UCA0RXBUF;
            count++;
        }
    }

}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void) {
    static int count = 1;
    count++;
    if (count == 15){ 
        _BIC_SR_IRQ(LPM0_bits);
        UCA0TXBUF = '\r';
        WDTCTL = WDTPW + WDTHOLD;
    }
}
