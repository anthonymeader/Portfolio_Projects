//-----------------------------------------------
//Anthony Meader, Riley Holmes
//April,11 2024
//Heating()          : HEAT UP PATTERN
//Cooling()          : COOL DOWN PATTERN
//P1.3             : SCL
//P1.2             : SDA
//------------------------------------------------
#include "msp430fr2310.h"
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>


unsigned int DataIn;
int Data = 0;
volatile uint32_t i;

unsigned int Dselect = 1;
unsigned int Cselect = 1;

unsigned int b_count = 8;
unsigned int Bpat = 1;
unsigned int Bpat1 = 0;
unsigned int Bpat2 = 0;

unsigned int c_count = 0;
unsigned int Cpat = 0;
unsigned int Cpat1 = 0;
unsigned int Cpat2 = 0;




unsigned int lock = 0;
unsigned int unlock = 0;


unsigned int a_count = 0;
unsigned int Apat = 0;
unsigned int Apat1 = 0;
unsigned int Apat2 = 0;

void Cooling(void){ // A
    switch(a_count){
    case 0:
        Apat = 1;
        break;
    case 1:
        Apat = 3;
        break;
    case 2:
        Apat = 7;
        break;
    case 3:
        Apat = 15;
        break;
    case 4:
        Apat = 31;
        break;
    case 5:
        Apat = 63;
        break;
    case 6:
        Apat = 127;
        break;
    case 7:
        Apat = 255;
        break;
    case 8:
        Apat = 0;
        break;
    default:
        Apat = 0;
    }

    Apat1 = Apat; //For Port 1 Outputs
    Apat2 = Apat; //For Port 2 Outputs
    Apat1 &= (0xF3); //Don't set Port 1 I2C Outputs
    Apat2 &= (0xC); //Set remaining bits to Port 2
    Apat2 = Apat2 << 4; //Shift the two remaining bits to P7 and P6
    P1OUT = Apat1;
    P2OUT = Apat2;

    a_count++;
    if(a_count == 9){
        a_count = 0;
    }
    int i = 0;

}

void Heating(void){
    switch(b_count){
    case 0:
        Bpat = 128;
        break;
    case 1:
        Bpat = 192;
        break;
    case 2:
        Bpat = 224;
        break;
    case 3:
        Bpat = 240;
        break;
    case 4:
        Bpat = 248;
        break;
    case 5:
        Bpat = 252;
        break;
    case 6:
        Bpat = 254;
        break;
    case 7:
        Bpat = 255;
        break;
    case 8:
        Bpat = 0;
        break;
    default:
        Bpat = 0;
        break;
    }

    Bpat1 = Bpat; //For Port 1 Outputs
    Bpat2 = Bpat; //For Port 2 Outputs
    Bpat1 &= (0xF3); //Don't set Port 1 I2C Outputs
    Bpat2 &= (0xC); //Set remaining bits to Port 2
    Bpat2 = Bpat2 << 4; //Shift the two remaining bits to P7 and P6
    P1OUT = Bpat1;
    P2OUT = Bpat2;


    b_count++;
    if(b_count == 9){
        b_count = 0;
    }



}

void off(void){
    switch(c_count){
    case 0:
        Cpat = 170;
        break;
    case 1:
        Cpat = 85;
        break;
    default:
        Cpat = 0;
        break;
    }

    Cpat1 = Cpat; //For Port 1 Outputs
    Cpat2 = Cpat; //For Port 2 Outputs
    Cpat1 &= (0xF3); //Don't set Port 1 I2C Outputs
    Cpat2 &= (0xC); //Set remaining bits to Port 2
    Cpat2 = Cpat2 << 4; //Shift the two remaining bits to P7 and P6
    P1OUT = Cpat1;
    P2OUT = Cpat2;

    c_count++;
    if(c_count == 2){
        c_count = 0;
    }


}



void ledOff(void){
    P1OUT &= ~BIT0;
    P1OUT &= ~BIT1;
    P1OUT &= ~BIT4;
    P1OUT &= ~BIT5;
    P1OUT &= ~BIT6;
    P1OUT &= ~BIT7;
    P2OUT &= ~BIT6;
    P2OUT &= ~BIT7;
}

void ledOn(void){
    P1OUT |= BIT0;
    P1OUT |= BIT1;
    P1OUT |= BIT4;
    P1OUT |= BIT5;
    P1OUT |= BIT6;
    P1OUT |= BIT7;
    P2OUT |= BIT6;
    P2OUT |= BIT7;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    UCB0CTLW0 |= UCSWRST; //Put UCB1CTLW0 into software reset

    UCB0CTLW0 |= UCMODE_3; //Put into I2c Mode
    UCB0CTLW0 &= ~UCMST; //Put into slave mode
    UCB0I2COA0 = 0x010; //set slave address
    UCB0I2COA0 |= UCOAEN; //enable slave address
    UCB0CTLW0 &= ~UCTR; // RX Mode
    UCB0CTLW1 &= ~UCASTP0; //disable auto stop
    UCB0CTLW1 &= ~UCASTP1; //disable auto stop



    P1SEL1 &= ~BIT3; //SCL
    P1SEL0 |= BIT3;

    P1SEL1 &= ~BIT2; //SDA
    P1SEL0 |= BIT2;


    // P1OUT |= BIT1;



    PM5CTL0 &= ~LOCKLPM5;

    UCB0CTLW0 &= ~UCSWRST; //Take out of Software Reset

    P1DIR |= BIT0;
    P1DIR |= BIT1;
    P1DIR |= BIT4;
    P1DIR |= BIT5;
    P1DIR |= BIT6;
    P1DIR |= BIT7;
    P2DIR |= BIT6;
    P2DIR |= BIT7;

    P1OUT &= ~BIT0;
    P1OUT &= ~BIT1;
    P1OUT &= ~BIT4;
    P1OUT &= ~BIT5;
    P1OUT &= ~BIT6;
    P1OUT &= ~BIT7;
    P2OUT &= ~BIT6;
    P2OUT &= ~BIT7;

    UCB0IE |= UCRXIE0; //Enable I2C Recieve RX

    //---setup timer
    TB1CTL |= TBCLR;
    TB1CTL |= TBSSEL__ACLK;
    TB1CTL |= MC__UP;
    TB1CCR0 = 10923;

    TB1CCTL0 &= ~CCIFG;
    TB1CCTL0 |= CCIE;



    __enable_interrupt();

    while(1){
        if (Data == 1){
            Data = 0;
            switch(DataIn){
            case 0:
                a_count=0;
                c_count=0;
                Heating();
                break;
            case 1:
                c_count = 0;
                b_count = 0;
                Cooling();
                break;
            case 2:
                a_count=0;
                b_count=0;
                off();
                break;
            default:
                a_count=0;
                b_count=0;
                c_count=0;
                P1OUT = 0;
                P2OUT = 0;
                break;
            }
        }
        UCB0IE |= UCRXIE0; //Enable I2C Recieve RX
    }

    return 0;

}


#pragma vector = EUSCI_B0_VECTOR
__interrupt void EUSCI_B0_I2C_ISR(void){
    DataIn = UCB0RXBUF;
}
#pragma vector = TIMER1_B0_VECTOR
__interrupt void ISR_TB3_CCR0(void){
    Data = 1;
    TB1CCTL0 &= ~CCIFG;
}


