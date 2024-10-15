;-------------------------------------------------------------------------------
;MSP430 Assembler Code Template for use with TI Code Composer Studio
;Anthony Meader, EELE465, Project 02
;January 25, 2024
;I2C Bit Bang
;P1.2 is SDA
;P1.6 is SCL
;R6 is the index of the bit being transmitted
;R8 is the transmitted value
;R4 decrement register for short delay
;R5 decrement register for long delay
;R12 decrment register for very long delay
;R11 is SCL pulse counter when reading data
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


       ;Setup I2C Lines
       bis.b #BIT2, &P1DIR ;Make P1 Bit 2 as an output high -SDA
       bis.b #BIT2, &P1OUT
       bis.b #BIT6, &P1DIR ;Make P1 Bit 6 as an output -SCL
       bis.b #BIT6, &P1OUT
       bic.b #LOCKLPM5, &PM5CTL0

       ;Delay Registers
       mov.b #2, R4 ;71us
       mov.b #3, R5 ;82us
       mov.b #10000, R12

	  ;Read Clock Subroutine Register
       mov.b #8, R11

      ;call #initrtc ; Intialize S/M/H for rollover testing

main:
;----------------------------------------
;S/M/H I2C Protocol
;----------------------------------------
       call #I2Cstart ;Start I2C Protocol
       mov #0D0h, TransmitByte ;Slave Address, write
       mov TransmitByte, R8 ;Store value into usable register
       mov #8, R6
       call #I2Csend ;Transmit and RTC ack


       mov #000h, TransmitByte ;Send Seconds address
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmit and RTC ack

       NOP
       NOP

       call #SDA_on ;Bring Data and Clock Lines High
       call #SCL_on

       call #I2Cstart
       mov #0D1h, TransmitByte ;Slave Address, read
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmit and RTC ack

;Read Seconds
       call #Read_Clock ;8 clock pulses
       call #SDA_off ;Manual ACK
       call #SCL_on
       call #delay_short
       call #SCL_off
       call #SDA_on ;Bring Data and Clock Lines High
       call #SCL_on

;Read Min
       call #Read_Clock ;8 clock pulses
       call #SDA_off ;Manual ACK
       call #SCL_on
       call #delay_short
       call #SCL_off
       call #SDA_on ;Bring Data and Clock Lines High
       call #SCL_on

;Read hours
       call #Read_Clock ;8 clock pulses
       call #SDA_on ;Manual NAK
       call #SCL_on
       call #delay_short
       call #SCL_off
       NOP
       NOP
       NOP
       NOP
       NOP
       NOP
       NOP
       call #stopcondition ;bring scl high then sda

;---------------------------------
;END S/M/H I2C Protocol
;---------------------------------

;---------------------------------
;Start Temperature I2C Protocol
;---------------------------------

       call #I2Cstart
       mov #0D0h, TransmitByte ;send slave address WR
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmit and RTC ack


       mov #011h, TransmitByte ;send register address WR
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmit and RTC ack

       NOP
       NOP
       call #SDA_on ;Bring Data and Clock Lines High
       call #SCL_on

       call #I2Cstart
       mov #0D1h, TransmitByte ;send slave address read
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmit and RTC ack

;Read first 8 bits for temp
       call #Read_Clock ;8 clock pulses
       call #SDA_off ;Manual ACK
       call #SCL_on
       call #delay_short
       call #SCL_off
       call #SDA_on ;Bring Data and Clock Lines High
       call #SCL_on
;Read remaining 2 bits for time
       call #Read_Clock ;8 clock pulses
       call #SDA_on ;Manual NAK
       call #SCL_on
       call #delay_short
       call #SCL_off

       nop
       nop
       nop

       call #stopcondition ;bring scl high then sda
       call #delay_verylong
       jmp main

initrtc: ;Initialize S/M/H

       call #I2Cstart
       mov #0D0h, TransmitByte ;Slave address, write
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmits and ACKs data.

       mov #01h, TransmitByte ;send min address
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmits and ACKs data.

       mov #059h, TransmitByte ;write min value
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmits and ACKs data.

       mov #02h, TransmitByte ;write hour value
       mov TransmitByte, R8
       mov #8, R6
       call #I2Csend ;Transmits and ACKs data.

       ;mov #02h, TransmitByte ;send hours
       ;mov TransmitByte, R8
       ;mov #8, R6
       ;call #I2Csend

       call #stopcondition ;bring scl high then sda
       ret ;start program


;--------------------------------------------------------------------------------
;START I2C start subroutine
;--------------------------------------------------------------------------------
I2Cstart: ;Bring SDA low, then shortly SCL low
       call #SDA_off
       call #delay_short
       call #SCL_off
       call #delay_short
       ret
;--------------------------------------------------------------------------------
;END I2C Start
;--------------------------------------------------------------------------------


;--------------------------------------------------------------------------------
;START I2Csend
;--------------------------------------------------------------------------------
I2Csend:  ;Reads in Data

       call #I2CtxByte ;transmits 8 bits
       call #I2CackRequest ;RTC ACK when done with transmit
       ret
;--------------------------------------------------------------------------------
;ENDI2C Send
;--------------------------------------------------------------------------------


;--------------------------------------------------------------------------------
;START I2C Transmit Byte Subroutine
;--------------------------------------------------------------------------------
I2CtxByte:
       clrc ;clears carry register
       rla.b R8 ;rotates register value left for MSB
       jc TransmitOn ;If there is a carry, transmit data
       jnc TransmitOff ;If no carry, dont trasmit

TransmitOn: ;If msb=1
       call #SDA_on ;SDA high
       call #delay_short
       call #SCL_on ;SCL high for short pulse
       call #delay_long
       call #SCL_off ;SCL low
       jmp EndTransmit ;Jump to End Transmit

TransmitOff: ;if msb=0
       call #SDA_off ;SDA low
       call #delay_short
       call #SCL_on ;SCL high for short pulse
       call #delay_long
       call #SCL_off ;SCL low
       jmp EndTransmit

EndTransmit:
       dec R6 ;decrement data register index
       jnz I2CtxByte ;if more index, transmit the next msb
       ret
;--------------------------------------------------------------------------------
;END I2C Transmit Byte Subroutine
;--------------------------------------------------------------------------------


;--------------------------------------------------------------------------------
; START Stop Condition Subroutine
;--------------------------------------------------------------------------------
stopcondition: ;Stops the I2C routine
       call #SCL_on
       call #delay_short
       call #SDA_on
       ret
;--------------------------------------------------------------------------------
;END Stop Condition Subroutine
;--------------------------------------------------------------------------------

;--------------------------------------------------------------------------------
;START RTC ACK Request
;--------------------------------------------------------------------------------
I2CackRequest: ;ACK determined by RTC, SDA will be pulled low by RTC
       call #SCL_on
       call #delay_short
       call #SCL_off
       mov.b #8, R6 ;reset register index
       ret
;--------------------------------------------------------------------------------
;END RTC ACK Request
;--------------------------------------------------------------------------------

;--------------------------------------------------------------------------------
;START Read Clock Subroutine
;--------------------------------------------------------------------------------
Read_Clock: ;Generates 8 clock pulses
       bis.b #BIT6, &P1OUT ;Pulls SCL high
       call #delay_short
       bic.b #BIT6, &P1OUT ;Pulls SCL low
       call #delay_long
       dec R11 ;decrement 8 times
       jnz Read_Clock
       mov.b #8, R11 ;reset SCL pulse counter
       bic.b #BIT6, &P1OUT ;bring SCL low
       ret
;--------------------------------------------------------------------------------
;END Read Clock Subroutine
;--------------------------------------------------------------------------------


;--------------------------------------------------------------------------------
;START Delay Subroutines
;--------------------------------------------------------------------------------
delay_short: ;Short Delay 71us
       dec R4
       jnz delay_short
       mov #2, R4
       ret

delay_long: ;Long Delay 82us
       dec R5
       jnz delay_long
       mov #3, R5
       ret

delay_verylong: ;Test for legibility delay
       call #SDA_on ;extra
       dec R12
       jnz delay_verylong
       mov #10000, R12
       ret
;--------------------------------------------------------------------------------
;END Delay Subroutines
;--------------------------------------------------------------------------------

;--------------------------------------------------------------------------------
;START SDA on/off subroutines
;--------------------------------------------------------------------------------
SDA_on:
       bis.b #BIT2, &P1OUT ;Pulls SDA High
       ret
SDA_off:
       bic.b #BIT2, &P1OUT ;Pulls SDA Low
       ret
;--------------------------------------------------------------------------------
;END SDA on/off subroutines
;--------------------------------------------------------------------------------

;--------------------------------------------------------------------------------
;START SCL on/off subroutines
;--------------------------------------------------------------------------------
SCL_on:
       bis.b #BIT6, &P1OUT ;Pulls SCL High
       ret
SCL_off:
       bic.b #BIT6, &P1OUT ;Pulls SCL Low
       ret
;--------------------------------------------------------------------------------
;END SCL on/off subroutines
;--------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; Memory Allocation
;-------------------------------------------------------------------------------
       .data
       .retain

TransmitByte: .space 2

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
