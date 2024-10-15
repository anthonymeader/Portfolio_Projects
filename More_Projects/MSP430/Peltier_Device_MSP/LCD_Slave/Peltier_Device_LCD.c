//-------------------------------------------------------------------------------
//Riley Holmes Anthony Meader, EELE465, Project 06
//April 11, 2024
//Slave to run LCD
//P1.2 is SDA
//P1.3 is SCL
//P1.4-7 is DB4-7
//P2.6 is RS
//P2.0 is E
//
//DataIn : empty array, received I2C Data
//data_cnt : keeps track of receiving data
//
//Run the LCD, including initialization, writing, and commands
//-------------------------------------------------------
#include "msp430fr2310.h"
#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

volatile char received[9];
volatile char DataIn[13] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
volatile int data_cnt =0;

volatile char Plant_Message[] = {'R','e','s','=','A',':','.',0xDF,'C',':','s','P',':','.',0xDF,'C','\0'};
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//set up I2C slave
    UCB0CTLW0 |= UCSWRST;

    UCB0CTLW0 |= UCMODE_3;
    UCB0CTLW0 &= ~UCMST;
    UCB0I2COA0 = 0x020;
    UCB0I2COA0 |= UCOAEN;
    UCB0CTLW0 &= ~UCTR;
    UCB0CTLW1 &= ~UCASTP0;
    UCB0CTLW1 &= ~UCASTP1;

    P1SEL1 &= ~BIT3; //SCL
    P1SEL0 |= BIT3;

    P1SEL1 &= ~BIT2; //SDA
    P1SEL0 |= BIT2;

    PM5CTL0 &= ~LOCKLPM5;

    UCB0CTLW0 &= ~UCSWRST;



 //set up lcd ports
    P1DIR |=BIT4; //DB4
    P1DIR |=BIT5; //DB5
    P1DIR |=BIT6; //DB6
    P1DIR |=BIT7; //DB7

    P2DIR |=BIT6; //RS
    P2DIR |=BIT0; //E

    P1OUT &= ~BIT4; //clear DB4-7, RS, E to
    P1OUT &= ~BIT5;
    P1OUT &= ~BIT6;
    P1OUT &= ~BIT7;

    P2OUT &= ~BIT6;
    P2OUT &= ~BIT0;
//------------------

    UCB0IE |= UCRXIE0;
    __enable_interrupt();

    InitLCD();
    int i =0;

//------write to screen---------
    while(1){
        for(i = 0;i<80;i++){ //80 places on LCD, index properly
            switch(i){
            case 0:
                write(Plant_Message[0]);
                break;
            case 1:
                write(Plant_Message[1]);
                break;
            case 2:
                write(Plant_Message[2]);
                break;
            case 3:
                write(Plant_Message[3]);
                break;
            case 4: //Resolution
                write(DataIn[0]);
                break;
            case 8:
                write(Plant_Message[4]); //A
                break;
            case 9:
                write(Plant_Message[5]); //:
                break;
            case 10:
                write(DataIn[1]); //Ambient Temp 1
                break;
            case 11:
                write(DataIn[2]); //Ambient Temp 2
                break;
            case 12:
                write(Plant_Message[6]); //.
                break;
            case 13:
                write(DataIn[3]); //Ambient Temp 3
                break;
            case 14:
                write(Plant_Message[7]);
                break;
            case 15:
                write(Plant_Message[8]);
                break;
            case 40: //Mode
                if (DataIn[5] == '0'){
                    write('A');
                }
                else if (DataIn[5] == '1'){
                    write('B');
                }
                else if (DataIn[5] == '2'){
                    write('C');
                }
                else if (DataIn[5] == '3'){
                    write('D');
                }
                else{
                 write(DataIn[5]); //M
                }
                break;
            case 41:
                write(Plant_Message[9]);
                break;
            case 42: //Time 1
                write(DataIn[7]);
                break;
            case 43: //Time 2
                write(DataIn[8]);
                break;
            case 44: //Time 3
                write(DataIn[9]);
                break;
            case 45:
                write(Plant_Message[10]);
                break;
            case 48://P
                write(Plant_Message[11]);
                break;
            case 49: //:
                write(Plant_Message[12]);
                break;
            case 50: //Plant Temp 1
                write(DataIn[10]);
                break;
            case 51: //Plant Temp 2
                write(DataIn[11]);
                break;
            case 52://.
                write(Plant_Message[13]);
                break;
            case 53: //Plant Temp 3
                write(DataIn[12]);
                break;
            case 54:
                write(Plant_Message[14]);
                break;
            case 55:
                write(Plant_Message[15]);
                break;
            default:
                write(' ');
                break;
            }
        }

    }
    return 0;
}

void InitLCD(void){

    //set to 4 bit mode
    __delay_cycles(50000);
    P2OUT &= ~BIT6;     //clear RS
    P1OUT = 0x30;
    latch();
    __delay_cycles(4100);
    latch();
    __delay_cycles(100);
    latch();


    //set to 4 bit mode
    P1OUT = 0x20;
    latch();
    __delay_cycles(40);

    //4 bit with font
    command(0x28);
    __delay_cycles(40);


    //display on/no cursor
    command(0x0C);
    __delay_cycles(40);


    //clear
    command(0x01);
    __delay_cycles(1640);


    //entry mode
    command(0x06);
    __delay_cycles(1640);

}


void latch(void){
    P2OUT |= BIT0;             //start latch
    __delay_cycles(1);
    P2OUT &= ~BIT0;            //end latch
    return;
}

void command(char i){   //writes command
    P1OUT = i & 0xF0;
    P2OUT &= ~BIT6;            // clear RS bit
    latch();                   // latch
    P1OUT = ((i << 4) & 0xF0); // rotate bits
    latch();
    return;
}

void write(char i){     //writes char
    P1OUT = i & 0xF0;
    P2OUT |= BIT6;             //set RS bit
    latch();                   //latch
    P1OUT = ((i << 4) & 0xF0); //rotate bits
    P2OUT |= BIT6;
    latch();                   //latch
    return;
}

#pragma vector = EUSCI_B0_VECTOR

__interrupt void EUSCI_B0_I2C_ISR(void){ //receive data from master (I2C)
    if(data_cnt == 12){
        DataIn[data_cnt] = UCB0RXBUF;
        data_cnt = 0;
    }
    else{
        DataIn[data_cnt] = UCB0RXBUF;
        data_cnt++;
    }
}
