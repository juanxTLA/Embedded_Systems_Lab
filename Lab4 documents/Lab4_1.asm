;-------------------------------------------------------------------------------
; File       : Lab4_1.asm
; Function   : Counts the number of sentences and words
; Description: Program traverses an input array of characters
;              to detect characters that end sentences like '.' or '!' and
;			   and ' ' character
; Input      : The input string is specified in myStr
; Output     : Registers R5 and R7
; Author     : Juan Tarrat
; Date       : Sept 17, 2020
;-------------------------------------------------------------------------------
        .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.

myStr:  .cstring "HELLO WORLD. I AM THE MSP430!"
        ; .string does add NULL at the end of the string;
        ;    '' ensures that a NULL follows the string.
        ; You can alternatively use .cstring "HELLO WORLD, I AM THE MSP430!"
        ;    that adds a NULL character at the end of the string automatically.
;-------------------------------------------------------------------------------
		.data
sent_count:		.int	0
word_count:		.int	0
;-------------------------------------------------------------------------------
        .text                           ; Assemble into program memory.
        .retain                         ; Override ELF conditional linking
                                        ; and retain current section.
        .retainrefs                     ; And retain any sections that have
                                        ; references to current section.

;-------------------------------------------------------------------------------
RESET:  		mov.w   #__STACK_END,SP         ; Initialize stack pointer
		        mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main loop here sentence: "HELLO WORLD. I AM THE MSP430!"
;-------------------------------------------------------------------------------
main:
        mov.w   #myStr, R4              ; load the starting address of the string into R4
        clr.b	R7						; register R7 will store the sentence count
        mov.b   #1, R5                  ; R5 = 1 because word count = number of ' '  + 1
gnext:  mov.b   @R4+, R6                ; get a new character
        cmp     #0,R6                   ; is it a null character
        jeq     lend                    ; if yes, go to the end
        cmp.b   #' ', R6                ; is it an ' ' character
        jne     dot                   	; if not, go to dot
        inc.w   R5                      ; if yes, increment word counter
        jmp     gnext					; next character

dot:	cmp		#'.', R6				; is it a '.'
		jne		exc						; if not go to exc
		inc.w	R7						; if yes increment sentence counter
		jmp 	gnext					; next character

exc:	cmp		#'!', R6				; is it a '!'
		jne		gnext					; if not next character
		inc.w	R7						; if yes increment setence counter
		jmp 	gnext					; next character

lend:   mov.w   R5, &word_count		   	; write result in R7 and r5
		mov.w 	R7, &sent_count
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
         .sect   ".reset"               ; MSP430 RESET Vector
         .short  RESET
         .end
