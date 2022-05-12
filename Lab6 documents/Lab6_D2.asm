;-------------------------------------------------------------------------------
;   File:        Lab6_D2.asm
;   Description: The program demonstrates Press/Release using SW1 and LED1.
;                LED1 is initialized off.
;                When SW1 press is detected, a software delay of 20 ms
;                is used to implement debouncing. The switch is checked
;                again, and if on, LED1 is turned on until SW1 is released.
;
;   Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO = 2^20=1,048,576 Hz
;   Platform:    TI EXP430F5529LP Experimenter's Board
;
;                 MSP430F5529
;             -----------------
;         /|\|                 |
;          | |                 |
;          --|RST              |
;            |             P1.0|-->LED1(REd)
;            |             P2.1|<--SW1
;
;   Author:  Aleksandar Milenkovic, milenkovic@computer.org
;   Date:    September 14, 2018
;	Modified: Prawar Poudel, August 08, 2019
;-------------------------------------------------------------------------------

            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:      mov.w   #__STACK_END,SP         ; Initialize stack pointer
StopWDT:    mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
;-------------------------------------------------------------------------------
SetupP2:
            bis.b   #001h, &P1DIR           ; Set P1.0 to output
                                            ; direction (0000_0001)
            bic.b   #001h, &P1OUT           ; Set P1OUT to 0x0000_0001 (ensure
                                            ; LED1 is off)

            bic.b   #002h, &P2DIR           ; SET P2.1 as input for SW1
            bis.b   #002h, &P2REN           ; Enable Pull-Up resister at P2.1
            bis.b   #002h, &P2OUT			; required for proper IO set up

ChkSW1:     bic.b   #001h, &P1OUT
			bit.b   #002h, &P2IN             ; Check if SW1 is pressed
                                            ; (0000_0010 on P1IN)
            jnz     ChkSW1                  ; If not zero, SW1 is not pressed
                                            ; loop and check again
Debounce:
            mov.w   #2000, R15              ; Set to (2000 * 10 cc = 20,000 cc)
SWD20ms:    dec.w   R15                     ; Decrement R15
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            jnz     SWD20ms                 ; Delay over?
            bit.b   #00000010b, &P2IN       ; Verify SW1 is still pressed
            jnz     ChkSW1                  ; If not, wait for SW1 press

LEDon:      bis.b   #001h, &P1OUT           ; Turn on LED1
SW1wait:    bit.b   #002h, &P2IN            ; Test SW1
            jz      SW1wait                 ; Wait until SW1 is released
            bic.b   #001h, &P1OUT            ; Turn off LED1
            jmp     ChkSW1                  ; Loop to beginning
            nop
                                            
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
            .end
