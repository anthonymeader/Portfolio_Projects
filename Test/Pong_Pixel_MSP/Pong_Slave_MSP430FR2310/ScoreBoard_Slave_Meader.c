//-----------------------------------------------
//Anthony Meader
//April,30 2024
//Receive Data from Master and Update LED (Scoreboard)
//------------------------------------------------

#include "msp430fr2310.h"
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>


unsigned int DataIn;


unsigned int p1_count = 0; //win count
unsigned int p2_count = 0; //win count


void player_one(void){
    switch(p1_count){
    case 0: //Turn on light, increment case and wait for a new recieved data
        P1OUT |= BIT7;
        p1_count++;
        break;
    case 1:
        P1OUT |= BIT6;
        p1_count++;
        break;
    case 2:
        P1OUT |= BIT5;
        p1_count++;
        break;
    case 3:
        P1OUT |= BIT4;
        break;
    default:
        break;
    }

}
void player_one_flash(void){ //LED P1 Win set
    P1OUT |= BIT5;
    P1OUT |= BIT6;
    P1OUT |= BIT7;
    P1OUT |= BIT4;
}


void player_two(void){
    switch(p2_count){ //Turn on light, increment case and wait for a new recieved data
    case 0:
        P1OUT |= BIT0;
        p2_count++;
        break;
    case 1:
        P1OUT |= BIT1;
        p2_count++;
        break;
    case 2:
        P2OUT |= BIT6;
        p2_count++;
        break;
    case 3:
        P2OUT |= BIT7;

        break;
    default:
        break;
    }

}
void player_two_flash(void){
    P1OUT |= BIT0;
    P1OUT |= BIT1;
    P2OUT |= BIT6;
    P2OUT |= BIT7;
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
    __enable_interrupt();

    while(1){
        switch(DataIn){
        case 0:
            DataIn = 0xFF; //Turn lights off to start game
            ledOff();
            p1_count = 0;
            p2_count = 0;
            break;
        case 1:
            DataIn = 0xFF;  //Recieved Data and clears it so only one light turns on at a time
            player_one();
            break;
        case 2:
            DataIn = 0xFF;  //Recieved Data and clears it so only one light turns on at a time
            player_two();
            break;
        case 3://P2 Wins Flash Pattern
            player_two_flash();
            __delay_cycles(300000);
            player_two_flash();
            __delay_cycles(300000);
            __delay_cycles(50000);
            DataIn = 0;
            break;
        case 4: //P1 Wins flash pattern
            player_one_flash();
            __delay_cycles(300000);
            player_one_flash();
            __delay_cycles(300000);
            player_one_flash();
            __delay_cycles(50000);
            DataIn = 0;
            break;
        default:
            break;
        }

        UCB0IE |= UCRXIE0; //Enable I2C Recieve RX
    }

    return 0;

}


#pragma vector = EUSCI_B0_VECTOR
__interrupt void EUSCI_B0_I2C_ISR(void){
    DataIn = UCB0RXBUF;
}



