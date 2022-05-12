;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
			.data
sent:		.cstring 	"I enjoy learning msp430" ;sentence
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
; Main loop here sent = I enjoy learning msp430
;-------------------------------------------------------------------------------
main:	mov.w	#sent, R4		; load the starting address of the string into R4

next:	mov.b	@R4+, R5		; next char
		cmp.b	#0, R5			; is it null?
		jeq		end				; if yes go to end
		cmp.b	#97, R5			; is it a lower case?
		jge		upper			; if it is (>=97) go to upper
		jmp 	next			; if not read next

upper:	sub.b	#32, R5			; get upper case
		mov.b	R5, -1(R4)		; put it in the location where the lower case was (before autoincrement)
		jmp		next			; go to next
                                            
end:	bis.w   #LPM4,SR                ; LPM4
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
            
