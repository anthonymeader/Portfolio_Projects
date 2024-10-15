;-------------------------------------------------------------------------------
;MSP430 Assembler Code Template for use with TI Code Composer Studio
;Anthony Meader, EELE465, Project 01
;January 23, 2024
;Heart Beat LED
;	R4: Outer Delay Loop Counter
;	R5: Inner Delay Loop Counter
;	Timer B0 ACLK - 32768 Hz
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
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
init:
;-- Setup LED1 and Two
		bis.b	#BIT0, &P1DIR
		bic.b   #BIT0, &P1OUT
		bis.b   #BIT6, &P6DIR
		bic.b   #BIT6, &P6OUT
		bic.b	#LOCKLPM5, &PM5CTL0

;-- Setup Interupts
		bis.w	#TBCLR, &TB0CTL
		bis.w   #TBSSEL__ACLK, &TB0CTL
		bis.w   #MC__UP, &TB0CTL

;-- Setup Compare Registers
		mov.w #32768, &TB0CCR0
		bis.w #CCIE, &TB0CCTL0
		bic.w #CCIFG, &TB0CCTL0
		NOP
		bis.w #GIE, SR
		NOP

;-- Initialize Loop Registers
		mov.w 	#35008, R4 ;initialize R4
		mov.b   #00Ah, R5 ;initialize R5

main:
		call #toggle_red_led
		jmp main

;-------------------------
;Start RED Led Delay Subroutine
;-------------------------
toggle_red_led:

delay: ; Delay for one second
		dec.w	R4
		jnz		delay
		mov.w   #35009, R4 ;Resets R4
		dec.b	R5
		jnz     delay
		mov.b   #00Ah, R5 ;Delayed for one second, reset loop
		xor.b 	#BIT0, &P1OUT
		jmp     main ; Go back to ISR to toggle green led
;-------------------------
;END of Red LED Delay
;-------------------------

;---------------------------
;Start ISR Green LED
;---------------------------
ISR_TB0_CCR0:
			xor.b #BIT6, &P6OUT ;Toggle Green LED right after the Red LED
			bic.w #CCIFG, &TB0CCTL0 ;Clear Interupt Flag
			reti
;----------------------------
;END of ISR Green LED
;----------------------------

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

            .sect   ".int43"
            .short	ISR_TB0_CCR0
