;-------------------------------------------------------------------------------
;   File:        Lab6_D3.asm
;   Description: The program demonstrates Press/Release using SW2 and LED1.
;                LED1 is initialized off. The main program enables interrupts
;                from P1.BIT1 (SW2) and remains in an infinite loop doing nothing.
;                P1_ISR implements debouncing and waits for a SW2 to be released.
;
;   Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO = 2^20=1,048,576 Hz
;   Platform:    TI EXP430F5529LP Experimenter's Board
;
;                 MSP430F5529
;             -----------------
;         /|\|                 |
;          | |                 |
;          --|RST              |
;            |             P1.0|-->LED1(RED)
;            |             P1.1|<--SW2
;
;   Author:  Aleksandar Milenkovic, milenkovic@computer.org
;   Date:    September 14, 2018
;	Modified: Prawar Poudel, Auguest 8, 2019
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
            .def    SW2_ISR
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:      mov.w   #__STACK_END, SP        ; Initialize stack pointer
StopWDT:    mov.w   #WDTPW|WDTHOLD, &WDTCTL ; Stop watchdog timer
;-------------------------------------------------------------------------------
Setup:
            bis.b   #001h, &P1DIR           ; Set P1.0 to output
                                            ; direction (0000_0001)
            bic.b   #001h, &P1OUT           ; Set P1OUT to 0x0000_0001

            bic.b   #002h, &P1DIR           ; SET P1.1 as input for SW2
            bis.b   #002h, &P1REN           ; Enable Pull-Up resister at P1.1
            bis.b   #002h, &P1OUT			; required for proper IO set up


            bis.w   #GIE, SR                ; Enable Global Interrupts
            bis.b   #002h, &P1IE            ; Enable Port 1 interrupt from bit 1
            bis.b   #002h, &P1IES           ; Set interrupt to call from hi to low
            bic.b   #002h, &P1IFG           ; Clear interrupt flag
InfLoop:
            jmp     $                       ; Loop here until interrupt
                                            
;----------------------------------------------------------------------------
; P1_0 (SW2) interrupt service routine (ISR)
;----------------------------------------------------------------------------
SW2_ISR:
            bic.b   #002h, &P1IFG           ; Clear interrupt flag
ChkSW2:     bit.b   #02h, &P1IN             ; Check if SW2 is pressed
                                            ; (0000_0010 on P1IN)
            jnz     LExit                   ; If not zero, SW is not pressed
                                            ; loop and check again
Debounce:   mov.w   #2000, R15              ; Set to (2000 * 10 cc )
SWD20ms:    dec.w   R15                     ; Decrement R15
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            jnz     SWD20ms                 ; Delay over?
            bit.b   #00000010b,&P1IN        ; Verify SW2 is still pressed
            jnz     LExit                   ; If not, wait for SW2 press
LEDon:      bis.b   #001h,&P1OUT            ; Turn on LED1
SW2wait:    bit.b   #002h,&P1IN             ; Test SW2
            jz      SW2wait                 ; Wait until SW2 is released
            bic.b   #001,&P1OUT             ; Turn off LED1
LExit:      reti                            ; Return from interrupt
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   ".int47"                ;PORT1_VECTOR, please check the MSP430F5529.h header file
            .short  SW2_ISR
            .end
