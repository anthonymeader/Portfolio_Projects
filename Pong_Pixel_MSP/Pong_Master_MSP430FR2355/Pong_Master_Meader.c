//----EELE 465 FINAL PROJECT--------
//Written by Anthony Meader
//April 30, 2024
//Pong Pixel Game
//----------------------------------
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Font.h"

//-----------I2C-------------
#define OLED_Address 0x03C;
volatile char transmit[2]; //transmit command to OLED
unsigned int led; //Transmit to LEDS
volatile char xy[7]; //XY Intialize Coordinates
int n; //I2C count
int i2c_0; //I2C Flags
int i2c_1; 
int i2c_2;
//---------------------------

//------INPUT VARIABLES------
int start = 0; //push button input to start game
unsigned int input; //ball speed input
unsigned int input_p; //paddle distance input
//---------------------------
//------TIME VARIABLES-------
int get_time_ones=0; //time tens
int get_time_tens=0; //time hundreds
int get_time_hundred =0; //time thousands
int tens_flag = 0; //tens flag
int hundreds_flag = 0; //hundreds flag
int thousands_flag = 0; //thousands flag
int time_start = 0; //time flag


//------PADDLE VARIABLES--------
unsigned int paddle_1_col1 = 0x7C; //P2 Paddle Col2
unsigned int paddle_1_col2 = 0x7E; //P2 Paddle Col2

unsigned int paddle_2_col1 = 0x00; //P1 paddle Col1
unsigned int paddle_2_col2 = 0x02; //P1 paddle Col2

unsigned int paddle_1_row1 = 0x00; //P1 Paddle row 1
unsigned int paddle_1_row2 = 0x01; //P1 paddle row 2

unsigned int paddle_2_row1 = 0x00; //P2 paddle row1
unsigned int paddle_2_row2 = 0x01; //P2 paddle row2
unsigned int new_paddle_1_row1 = 0xFF; //From ADC Range
unsigned int new_paddle_1_row2 = 0xFF;
unsigned int new_paddle_2_row1 = 0xFF; //From ADC Range
unsigned int new_paddle_2_row2 = 0xFF;
//-----------------------------

//------BALL VARIABLES---------
unsigned int ball_1x = 0x30; //Ball Col1
unsigned int ball_2x = 0x34; //Ball Col2
unsigned int ball_y1 = 0x03; //Ball Height1
unsigned int ball_y2 = 0x04; //Ball Height2
int x_contact = 0; //x hit
int y_contact = 0; //y hit
int y_slope = 0; //Slope of Ball
//-----------------------------

int p2_wincount = 0; //Increments if Player two wins round
int p1_wincount = 0; //increments if player one wins round

int new_round = 0; //Resets every round

unsigned int ADC_Value; //Gets ADC Value
int count = 2; //ADC Read count for selecting channel, 2 = paddle update paddle 1, 1 = update paddle two, it decrements

//I2C Function to set coordinates of what is being displayed
void OLED_setXY(char col_start, char col_end, char page_start, char page_end){
    i2c_1 = 1; //assert flag
    i2c_0 = 0;
    i2c_2 = 0;
    n = 0;
    xy[0]=0x00;
    xy[1]=0x21;
    xy[2]=col_start;
    xy[3]=col_end;
    xy[4]=0x22;
    xy[5]=page_start;
    xy[6]=page_end;
    UCB1TBCNT = 7; //Transmit 1 Byte of Data;
    UCB1I2CSA = OLED_Address;
    UCB1CTLW0 |= UCTR; // Put in transmit mode
    UCB1CTLW0 |= UCTXSTT; //generate the start condition*/
    //__delay_cycles(3000);

    while((UCB1IFG & UCSTPIFG)==0){}
    UCB1IFG &= ~UCSTPIFG;
}
//I2C function to sending data to OLED
void OLED_Data(char *DATA){
    int g = 0;
    int index = 0;
    int len = strlen(DATA);
    for(g=0; g<len; g++)
    {
        for (index=0; index<5; index++)
        {
            OLED_init(0x40,ASCII[DATA[g]-0x20][index]); //Gets value from "Font.h"
        }
    }
}
//I2C Function to send data to OLED or intialize
void OLED_init(unsigned int first, unsigned int second){
    i2c_0 = 1; //assert flag
    i2c_1 = 0;
    i2c_2 = 0;
    n = 0;
    transmit[0]=first;
    transmit[1]=second;
    UCB1TBCNT = 2; //Transmit 1 Byte of Data;
    UCB1I2CSA = OLED_Address;
    UCB1CTLW0 |= UCTR; // Put in transmit mode
    UCB1CTLW0 |= UCTXSTT; //generate the start condition*/
    while((UCB1IFG & UCSTPIFG)==0){}
    UCB1IFG &= ~UCSTPIFG;
}
//Clears entire display
void OLED_Off(void){
    int k =0;
    OLED_setXY(0x00,0x7F,0x00,0x07);
    for (k=0; k<1023;k++){ //clears each pixel on display
        OLED_init(0x40,0x00);
    }
}
//Clears selected area (used for paddle and ball)
void ball_off(unsigned int one, unsigned int two, unsigned int three, unsigned int four){
    int k =0;
    OLED_setXY(one,two,three,four); //clears specified section of display
    for (k=0; k<10;k++){
        OLED_init(0x40,0x00);
    }
}
//Transmits data to LEDS for scoreboard
void LED_Transmit(unsigned int first){
    i2c_0 = 0;
    i2c_1 = 0;
    i2c_2 = 1;
    n = 0;
    led=first;
    UCB1TBCNT = 1; //Transmit 1 Byte of Data;
    UCB1I2CSA = 0x010; //MSP430FR2310 Slave Address
    UCB1CTLW0 |= UCTR; // Put in transmit mode
    UCB1CTLW0 |= UCTXSTT; //generate the start condition*/
    while((UCB1IFG & UCSTPIFG)==0){}
    UCB1IFG &= ~UCSTPIFG;

}
int main (void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    UCB1CTLW0 |= UCSWRST; //Put UCB1CTLW0 into software reset
    UCB1CTLW0 |= UCSSEL_3; //Select Mode 3 //BRCLK - SMCLK = 1Mhz
    UCB1BRW = 10;
    UCB1CTLW0 |= UCMODE_3; //Put into I2c Mode
    UCB1CTLW0 |= UCMST; //Put into master mode
    UCB1CTLW0 |= UCTR; // Transmit Mode
    UCB1CTLW1 |= UCASTP_2; //Autostop enable

    P4SEL1 &= ~BIT7; //SCL
    P4SEL0 |= BIT7;
    P4SEL1 &= ~BIT6; //SDA
    P4SEL0 |= BIT6;

    PM5CTL0 &= ~LOCKLPM5;

    UCB1CTLW0 &= ~UCSWRST; //Take out of Software Reset

    UCB1IE |= UCTXIE0; //Enable I2C Interrupt TX
    //UCB1IE |= UCRXIE0; //Enable I2C Interrupt TX


    __enable_interrupt();
    ADCCTL0 &= ~ADCSHT;
    ADCCTL0 |= ADCSHT_2; //16 conversion cycles
    ADCCTL0 |= ADCON;
    ADCMCTL0 = 1; //ADC Input Channels (4 downto 0)

    // ADCCTL0 |= ADCSREF_5; //external positive reference
    ADCCTL1 |= ADCSSEL_2;
    ADCCTL1 |= ADCSHP;
    ADCCTL1 |= ADCCONSEQ_3; //repeated sequential sampling
    ADCCTL2 &= ~ADCRES;
    ADCCTL2 |= ADCRES_2; //12 bit res
    //ADCMCTL0 |= ADCINCH_2; //Channel A2
    ADCIE |= ADCIE0;

    __enable_interrupt();

    P3DIR &= ~BIT0; //INIT col low input
    P3REN |= BIT0;
    P3OUT |= BIT0;

    P6DIR &= ~BIT2; //INIT col low input
    P6REN |= BIT2;
    P6OUT |= BIT2;

    P2DIR |= BIT7; //INIT col low input
    P2REN |= BIT7;
    P2OUT |= BIT7;

    P2IES |= BIT7;
    P2IFG &= ~BIT7;
    P2IE |= BIT7;

    __enable_interrupt();

    TB0CTL |= TBCLR; //Set up timer for every second
    TB0CTL |= TBSSEL__ACLK;
    TB0CTL |= MC__UP;
    TB0CCR0 = 32768;

    TB0CCTL0 |= CCIE;
    TB0CCTL0 &= ~CCIFG;

    __enable_interrupt();

    //CONFIG DISPLAY
    OLED_init(0x00,0xAE); // Display OFF
    OLED_init(0x00,0xD5); // Set Clock Divide Ratio and Oscillator Frequency
    OLED_init(0x00,0x80);
    OLED_init(0x00,0xA8);
    OLED_init(0x00,0x3F); // 64 COM lines
    OLED_init(0x00,0xD3); // display offset
    OLED_init(0x00,0x00); // 0 offset
    OLED_init(0x00,0x40); // Set first line as the start line of the display
    OLED_init(0x00,0x8D);
    OLED_init(0x00,0x14);
    OLED_init(0x00,0x20);
    OLED_init(0x00,0x00);
    OLED_init(0x00,0xA1);
    OLED_init(0x00,0xC8);
    OLED_init(0x00,0xDA);
    OLED_init(0x00,0x12);
    OLED_init(0x00,0x81); // Set contrast control
    OLED_init(0x00,0x80); // Set Contrast to 128
    OLED_init(0x00,0xD9);
    OLED_init(0x00,0xF1);
    OLED_init(0x00,0xDB);
    OLED_init(0x00,0x20);
    OLED_init(0x00,0xA4);
    OLED_init(0x00,0xA6);
    OLED_init(0x00,0x2E); // Deactivate scroll
    OLED_init(0x00,0xAF); // Display on in normal mode
    __delay_cycles(100);
    OLED_Off();
    __delay_cycles(1000);
    __delay_cycles(10000);
    OLED_Off();
    __delay_cycles(10000);


    int counting = 0;
    int win = 0;
    char time[1];
    char time_tens[1];
    char time_hun[1];



    LED_Transmit(0);

    while(1){
        if (start == 0){
            TB0CCTL0 &= ~CCIE; //stop timer
            input = P3IN; //read inputs
            input_p = P6IN;
            if(input_p != 0){ //Default Paddle Input
                paddle_1_col1 = 0x7C;
                paddle_1_col2 = 0x7E;

                paddle_2_col1 = 0x00;
                paddle_2_col2 = 0x02;
                ball_1x = 0x3D;
                ball_2x = 0x41;
            }
            else if (input_p == 0){ //Paddle input is switched to on
                paddle_1_col1 = 0x5E; //Bring paddles closer
                paddle_1_col2 = 0x60;

                paddle_2_col1 = 0x1E; //bring paddles closer
                paddle_2_col2 = 0x20;
                ball_1x = 0x3D;
                ball_2x = 0x41;
            }
            time_start = 1; //Assert new game flag
            new_round = 0; //Assert new round flag
            OLED_setXY(0x34, 0x7F, 0x02, 0x01); //Center Coordinates for start message
            OLED_Data("Press");
            OLED_setXY(0x34, 0x7F, 0x03, 0x02);
            OLED_Data("Start!");
            if (win == 1){ //If there was just a game and winner
                OLED_setXY(0x2C, 0x46, 0x05, 0x03);
                OLED_Data("Time:");
                sprintf(time,"%d",get_time_ones); //Seconds (tens) to string
                sprintf(time_tens,"%d",get_time_tens); //Seconds (hundreds) to string
                sprintf(time_hun,"%d",get_time_hundred); //seconds (thousands) to string
                OLED_setXY(0x48, 0x7F, 0x05, 0x03); //Display previous game time
                OLED_Data(time_hun);
                OLED_setXY(0x4E, 0x7F, 0x05, 0x03);
                OLED_Data(time_tens);
                OLED_setXY(0x54, 0x7F, 0x05, 0x03);
                OLED_Data(time);
                OLED_setXY(0x5A, 0x7F, 0x05, 0x03);
                OLED_Data("s"); //seconds(S)
            }


        }
        else if (start == 1){ //if game has started
            TB0CCTL0 |= CCIE; //start timer
            updatePaddle();
            updateBall();
            if (counting == 3){
                ADCCTL0 |= ADCENC | ADCSC; //starts adc
                __delay_cycles(1000);
                counting = 0;
            }
            counting++;
        }

        if (p1_wincount == 4 || p2_wincount == 4){ //if someone won
            LED_Transmit(0);
            OLED_Off();

            OLED_setXY(0x34, 0x7F, 0x03, 0x02);
            if(p1_wincount == 4){ //if player 2 won
                __delay_cycles(1000);
                LED_Transmit(3);//transmit win condition to LEDS
                OLED_Data("P2");
            }
            else if (p2_wincount == 4){ //if player 1 won
                LED_Transmit(4); //Transmit Win condition to LEDS
                __delay_cycles(1000);

                OLED_Data("P1");
            }
            OLED_setXY(0x34, 0x7F, 0x04, 0x03);
            OLED_Data("Wins!");
            __delay_cycles(300000);
            OLED_Off();
            p1_wincount = 0; //reset game
            p2_wincount = 0; //reset game
            new_round = 0;     //reset game
            win = 1;         //reset game
            start = 0;       //reset game
        }

    }

    return 0;
}


//Function to update ball
void updateBall(void){
    ball_off(ball_1x,ball_2x,ball_y1,ball_y2); //clear ball to write a new ball

    if(ball_1x == (paddle_2_col2 + 0x01)){ //if the ball its P1 Paddle
        x_contact = 1; //flag to go left
        if (ball_y1 == paddle_2_row2 || ball_y2 == paddle_2_row1){ //if ball hits middle of paddle
            y_contact = 0; //deflect straight
        }
        else if (ball_y1 == paddle_2_row1 && ball_y2 != paddle_2_row2){
            y_contact = 1; //if ball hits top of paddle, deflect up
        }
        else if (ball_y2 == paddle_2_row2 && ball_y1 != paddle_2_row1){
            y_contact = 2; //if ball hits bottom of paddle, deflect down
        }
        else{
            y_contact = 0; //reset flags for a new round
            x_contact = 0;
            ball_1x = paddle_2_col2+0x01; //reset ball
            ball_2x = ball_1x+4;;
            ball_y1 = paddle_2_row2;
            ball_y2 = paddle_2_row1;
            p1_wincount++; //increment win
            LED_Transmit(0x02); //transmit to LED to update scoreboard
            __delay_cycles(1000);
            new_round = 0; //signal new round
        }
    }

    else if (ball_2x == (paddle_1_col1 - 0x01)){
        x_contact = 2;
        if (ball_y1 == paddle_1_row2 || ball_y2 == paddle_1_row1){
            y_contact = 0;
        }
        else if (ball_y1 == paddle_1_row1 && ball_y2 != paddle_1_row2){
            y_contact = 1;
        }
        else if (ball_y2 == paddle_1_row2 && ball_y1 != paddle_1_row1){
            y_contact = 2;
        }
        else{
            y_contact = 0;
            x_contact = 2;
            ball_1x = paddle_1_col1-5;
            ball_2x = ball_1x+4;
            ball_y1 = paddle_1_row2;
            ball_y2 = paddle_1_row1;
            p2_wincount++;
            LED_Transmit(0x01);
            __delay_cycles(1000);
            new_round = 0;

        }
    }

    if (y_slope == 15){ //Slope is 15 cols over 1 row
        if(ball_y1 == 0x00){ //if hits bottom of screen
            y_contact = 1;
        }
        else if (ball_y2 == 0x07){ //if hits top of screen
            y_contact = 2;
        }
        switch (y_contact){
        case 0:
            ball_y1 = ball_y1; //slope = 0, straight line
            ball_y2 = ball_y2;
            break;
        case 1: //hit bottom, slope = 1/15
            ball_y1 = ball_y1 + 0x01;
            ball_y2 = ball_y2 + 0x01;
            break;
        case 2:
            ball_y1 = ball_y1 - 0x01; //deflect down slope = -1/15
            ball_y2 = ball_y2 - 0x01;
            break;
        default:
            ball_y1 = ball_y1 + 0x01;
            ball_y2 = ball_y2 + 0x01;
            break;
        }
        y_slope = 0;


    }
    switch (x_contact){ //update x axis each clock cycle
    case 0:
        ball_1x++;
        ball_2x++;
        break;
    case 1:
        ball_1x++;
        ball_2x++;
        break;
    case 2:
        ball_1x--;
        ball_2x--;
        break;
    default:
        break;
    }
    OLED_setXY(ball_1x,ball_2x,ball_y1,ball_y2); //write new ball
    OLED_Data("o");
    if (input != 0){ //speed input slow
        __delay_cycles(5000);
    }
    else if (input == 0){ //speed input fast
        __delay_cycles(1);
    }

    y_slope++;



}
//function to update paddle from user input
void updatePaddle(void){
    if (time_start == 1){ //new game restart clock
        get_time_ones=0;
        get_time_tens=0;
        get_time_hundred =0;
        tens_flag = 0;
        hundreds_flag = 0;
        thousands_flag = 0;
        time_start = 0;
    }
    if (new_round == 0){ //new round, clear and update display
        OLED_Off();
        new_round = 1;
        OLED_setXY(paddle_1_col1, paddle_1_col2, paddle_1_row2, paddle_1_row1);
        OLED_Data("|");
        OLED_setXY(paddle_2_col1, paddle_2_col2, paddle_2_row2, paddle_2_row1);
        OLED_Data("|");

    }
    else {
        if (new_paddle_1_row1 != paddle_1_row1){ //if ADC range != current paddle dimensions, change them and update
            ball_off(paddle_1_col1, paddle_1_col2, paddle_1_row2, paddle_1_row1);
            paddle_1_row1 = new_paddle_1_row1;
            paddle_1_row2 = new_paddle_1_row2;
            OLED_setXY(paddle_1_col1, paddle_1_col2, paddle_1_row2, paddle_1_row1);
            OLED_Data("|");
        }
        else if(new_paddle_2_row1 != paddle_2_row1){ //if ADC range != current paddle dimensions, change them and update
            ball_off(paddle_2_col1, paddle_2_col2,paddle_2_row2, paddle_2_row1);
            paddle_2_row1 = new_paddle_2_row1;
            paddle_2_row2 = new_paddle_2_row2;
            OLED_setXY(paddle_2_col1, paddle_2_col2, paddle_2_row2, paddle_2_row1);
            OLED_Data("|");
        }
    }

}
//Interrupt to read time up to 999 seconds
#pragma vector = TIMER0_B0_VECTOR //time logic
__interrupt void ISR_TB0_CCR0(void){
    get_time_ones++;
    if (get_time_ones == 10){ //if time = 10 seconds, assert ones flags and reset 10s seconds
        get_time_ones = 0;
        hundreds_flag = 1;
    }
    if (hundreds_flag == 1){ //increment 10s flag
        get_time_tens++;
        tens_flag = 0;
        hundreds_flag = 0;
    }
    if (get_time_tens == 10){ //if  time = 100 seconds, assert 100s flag, reset tens and ones
        get_time_tens = 0;
        hundreds_flag = 1;
        thousands_flag = 1;
    }
    if (thousands_flag == 1){// increment 100s flag
        get_time_hundred++;
        hundreds_flag = 0;
        thousands_flag = 0;
    }
    if (get_time_hundred == 10){ //if time = 1000s, reset time
        get_time_hundred = 0;
        hundreds_flag = 0;
        thousands_flag = 0;
    }
    TB0CCTL0 &= ~CCIFG;


}
//Interrupt to Start Game
#pragma vector = PORT2_VECTOR //start button interrupt
__interrupt void ISR_Port2_S7(void){
    start = 1;
    P2IFG &= ~BIT7;
}

#pragma vector=ADC_VECTOR //ADC (2 Channel)
__interrupt void ADC_ISR(void){
    ADC_Value = ADCMEM0; //get adc value
    if (count == 0){
        count =2;
    }
    if (count == 2){
        if (ADC_Value > 0 && ADC_Value < 585){
            new_paddle_1_row1 = 0x01;
            new_paddle_1_row2 = 0x00;
        }
        else if (ADC_Value > 585 && ADC_Value < 1170){
            new_paddle_1_row1 = 0x02;
            new_paddle_1_row2 = 0x01;
        }
        else if (ADC_Value > 1170 && ADC_Value < 1755){
            new_paddle_1_row1 = 0x03;
            new_paddle_1_row2 = 0x02;
        }
        else if (ADC_Value > 1755 && ADC_Value < 2340){
            new_paddle_1_row1 = 0x04;
            new_paddle_1_row2 = 0x03;
        }
        else if (ADC_Value > 2340 && ADC_Value < 2925){
            new_paddle_1_row1 = 0x05;
            new_paddle_1_row2 = 0x04;
        }
        else if (ADC_Value > 2925 && ADC_Value < 3510){
            new_paddle_1_row1 = 0x06;
            new_paddle_1_row2 = 0x05;
        }
        else if (ADC_Value > 3510 && ADC_Value < 4096){
            new_paddle_1_row1 = 0x07;
            new_paddle_1_row2 = 0x06;
        }

    }
    else if (count == 1){
        if (ADC_Value > 0 && ADC_Value < 585){
            new_paddle_2_row1 = 0x01;
            new_paddle_2_row2 = 0x00;
        }
        else if (ADC_Value > 585 && ADC_Value < 1170){
            new_paddle_2_row1 = 0x02;
            new_paddle_2_row2 = 0x01;
        }
        else if (ADC_Value > 1170 && ADC_Value < 1755){
            new_paddle_2_row1 = 0x03;
            new_paddle_2_row2 = 0x02;
        }
        else if (ADC_Value > 1755 && ADC_Value < 2340){
            new_paddle_2_row1 = 0x04;
            new_paddle_2_row2 = 0x03;
        }
        else if (ADC_Value > 2340 && ADC_Value < 2925){
            new_paddle_2_row1 = 0x05;
            new_paddle_2_row2 = 0x04;
        }
        else if (ADC_Value > 2925 && ADC_Value < 3510){
            new_paddle_2_row1 = 0x06;
            new_paddle_2_row2 = 0x05;
        }
        else if (ADC_Value > 3510 && ADC_Value < 4096){
            new_paddle_2_row1 = 0x07;
            new_paddle_2_row2 = 0x06;
        }
    }
    count--;
}
//I2C Interrupt
#pragma vector = EUSCI_B1_VECTOR //I2C Logic
__interrupt void EUSCI_B1_I2C_ISR(void){
    if (i2c_1==1){
        UCB1TXBUF = xy[n];
        n++;
    }
    else if (i2c_0 == 1){
        UCB1TXBUF = transmit[n];
        n++;
    }
    else if (i2c_2 == 1){
        UCB1TXBUF = led;
        i2c_2 = 0;

    }
}

