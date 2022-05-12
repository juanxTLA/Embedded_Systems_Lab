;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to

operation:	.cstring	"4-3+5"				;mathematical expression                                            ; make it known to linker.
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here doing: 4-3+5
;-------------------------------------------------------------------------------
main:	mov.w	#operation, R4

next:	mov.b	@R4+, R6	; next character
		cmp.b	#0, R6		; is it a null char?
		jeq		lend		; if yes go to end
		cmp.b	#'+', R6	; is it a +?
		jne		sym			; if not go to sym
		jmp		sum			; if yes go to sum

sym:	cmp.b	#'-', R6	; is it a '-'
		jne		num			; if not go to num
		jmp		subs		; go to subs

sum:	mov.b	@R4+, R6	; read next character
		sub		#48, R6		; substract 48 to get number and put it in R6
		add		R6,R7		; add R6 and R7
		jmp 	next		; go to next

num:	sub		#48, R6		; substract 48 to get number and put it in R6
		mov.b	R6, R7		; move contents of R6 into R7
		jmp		next		; go to next

subs: 	mov.b	@R4+, R6	; read next charcater
		sub		#48, R6		; substract 48 to get number and put it in R6
		sub		R6, R7		; do R7<-R7 - R6
		jmp 	next		; go to next

lend:	mov.b	R7, &P2OUT	; put R7 into P2OUT
		bis.w   #LPM4,SR                ; LPM4
        nop                             ; required only for Debugger

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
            
