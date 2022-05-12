
;------------------------------------------------------------------------------
; Instructor:   Aleksander Milenkovic
; Program:      LED  AND SWITCH interact
; Date:         Oct 09, 2020
; Input:        None
; Output:       Table
;-----------------------------------------------------------------------------



            .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
            .def    SW2_ISR
            .def	SW1_ISR
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

            bis.b   #080h, &P4DIR           ; Set P1.0 to output
                                            ; direction (0000_0001)
            bic.b   #080h, &P4OUT           ; Set P1OUT to 0x0000_0001

            bic.b   #002h, &P1DIR           ; SET P1.1 as input for SW2
            bis.b   #002h, &P1REN           ; Enable Pull-Up resister at P1.1
            bis.b   #002h, &P1OUT			; required for proper IO set up

            bic.b   #002h, &P2DIR           ; SET P2.1 as input for SW1
            bis.b   #002h, &P2REN           ; Enable Pull-Up resister at P2.1
            bis.b   #002h, &P2OUT			; required for proper IO set up

            bis.w   #GIE, SR                ; Enable Global Interrupts

            bis.b   #002h, &P1IE            ; Enable Port 1 interrupt from bit 1
            bis.b   #002h, &P1IES           ; Set interrupt to call from hi to low

            bis.b   #002h, &P2IE            ; Enable Port 2 interrupt from bit 1
            bis.b   #002h, &P2IES           ; Set interrupt to call from hi to low

main:       bic.b   #002h, &P1IFG           ; Clear interrupt flag
			bic.b   #002h, &P2IFG           ; Clear interrupt flag

       		cmp		#1, R5
            jne		led1
            clr		R5
            xor.b   #001h,&P1OUT


led1:
            cmp		#1, R6
            jne		InfLoop
            clr		R6
            mov		#6, R7			; counter for blinks
next:       mov		#0xFFFF, R15 	;1Hz
delay1:     nop
            nop
            nop
            nop
            nop
            dec 	R15
            jnz		delay1
            xor.b	#80h, &P4OUT	;from on to off or viceversa
            dec		R7
            jnz		next
            bic.b	#80h, &P4OUT	;turn it off
            bit.b	#001h, &P1OUT	;check status of other led
            jz		InfLoop			; if off skip
            bis.b	#80h, &P4OUT	; if not turn it on

InfLoop:
            jmp    	main                       ; Loop here until interrupt

;----------------------------------------------------------------------------
; P1_1 (SW2) interrupt service routine (ISR)
;----------------------------------------------------------------------------
SW2_ISR:
            clr 	R5
            bic.b   #02h, &P1IFG           ; Clear interrupt flag
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
			mov		#1, R5

LExit:      reti                            ; Return from interrupt
;----------------------------------------------------------------------------
; P2_1 (SW2) interrupt service routine (ISR)
;----------------------------------------------------------------------------
SW1_ISR:	clr		R6
			bic.b   #002h, &P2IFG           ; Clear interrupt flag
			bit.b	#002h, &P2IN			; check switch 1

			jnz		exit
			mov.b	#2000, R15
delay:		dec		R15
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz		delay
			bit.b	#002h, &P2IN
			jnz		exit
			mov		#1, R6

exit:		reti
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
            .short	SW2_ISR
            .sect	".int42"
            .short  SW1_ISR
            .end
