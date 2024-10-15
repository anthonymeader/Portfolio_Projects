//---EELE 465 Project 6 MASTER---
//Authors: Anthony Meader, Riley Holmes
//April, 11th 2024
//-------------------------------

#include <msp430.h> 
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//---KEYPAD VARIABLES---
unsigned int unsCol = 0;
unsigned int unsRow = 0;
int key = 0;
char keyPress;
unsigned int letterPress = 3;

int n = 0;
//----------------------

//----LM19 VARIABLES----
//-----ADC VALUES-------
unsigned int ADC_Value;
float voltage; //voltage being read
double temp_C;
volatile int ave_c = 0;
volatile int ave_k = 0;
float total = 0;

volatile float average[9];
volatile int ave_cnt=9;
//----------------------
unsigned int temp_compare = 0; //Mode C Select Compare
int press = 9; //moving average
//----LM92 VARIABLES----
volatile int LM92_data = 0;
volatile int LSB = 0;
volatile int MSB = 0;
volatile float LM92_temp = 0;
volatile float LM92_average[9];
volatile int LM92_ave_cnt=9;
volatile float LM92_total = 0;
volatile float LM92_ave_c = 0;
volatile float LM92_comp = 0;
volatile int LM92_send= 0;

//-----I2C Variables----
unsigned int received[];
unsigned int transmit[];
int data_cnt = 0;

//---------RTC----------
int time[2];
unsigned int sec = 0;
unsigned int previous_sec = 0;
unsigned int sec1 = 0;
unsigned int sec2 = 0;
int on = 1;
int once = 0;
volatile uint16_t stamp = 0;
volatile uint16_t stamp1 = 0;
volatile uint16_t timeTr = 1000;
volatile int sub = 0;

//LCD Transmit Packet
char mov_ave[15];
char seconds[4] = {'0','0','0'};
char lm_19_send[4];
char lm_92_send[6];
char patterns[];
int trans_count = 0;

volatile uint32_t i;

int get_temp = 0;
int get_time = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P6DIR |= BIT6;
    P6DIR |= BIT5;

    P1SEL1 |= BIT2; //channel A2
    P1SEL0 |= BIT2;

    //I2C Config

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
    UCB1IE |= UCRXIE0; //Enable I2C Interrupt TX




    //ADC Config
    ADCCTL0 &= ~ADCSHT;
    ADCCTL0 |= ADCSHT_2; //16 conversion cycles
    ADCCTL0 |= ADCON;
   // ADCCTL0 |= ADCSREF_5; //external positive reference
    ADCCTL1 |= ADCSSEL_2;
    ADCCTL1 |= ADCSHP;
    ADCCTL2 &= ~ADCRES;
    ADCCTL2 |= ADCRES_2; //12 bit res
    ADCMCTL0 |= ADCINCH_2; //Channel A1
    ADCIE |= ADCIE0;

    //TIMER (.5 Second)
    TB0CTL |= TBCLR;
    TB0CTL |= TBSSEL__ACLK;
    TB0CTL |= MC__UP;
    TB0CCR0 = 16384;

    TB0CCTL0 |= CCIE;
    TB0CCTL0 &= ~CCIFG;

    __enable_interrupt();
    Transmit_Data(0x48,0x0,1); //init lm92
    Transmit_Data(0x68,0x0,1); //init RTC

    while(1){
        n = 0;
        colIn();
        if (get_temp == 1){ //every half second
           get_temp = 0;
           Receive_Data(0x48,2); //read lm92
           ADC(); //readlm19
           if (letterPress == 2){
              Transmit_Data(0x010,temp_compare,1); //Transmit to LEDS
           }
           else{
               Transmit_Data(0x010,letterPress,1); //Transmit to LEDS
           }
           if (trans_count >= 9){
           TB0CCTL0 &= ~CCIE;
           sprintf(mov_ave,"%d",press); //put press in array //1
           sprintf(lm_19_send,"%d",ave_c); //put press in array //4
           sprintf(seconds,"%d",timeTr); //put press in array
           sprintf(lm_92_send,"%d",LM92_send); //put press in array
           sprintf(patterns, "%d",letterPress);
           strcat(mov_ave,lm_19_send);
           strcat(mov_ave,patterns);
           strcat(mov_ave,seconds);
           strcat(mov_ave,lm_92_send);
           data_cnt = 0;
           UCB1TBCNT = 13; //Transmit 1 Byte of Data;
           UCB1I2CSA = 0x020;
           UCB1CTLW0 |= UCTR; // Put in transmit mode
           UCB1CTLW0 |= UCTXSTT; //generate the start condition*/
           __delay_cycles(2000);
           TB0CCTL0 |= CCIE;
           }
           trans_count++;
        }

        if (get_time == 2){
            get_time = 0;
            Transmit_Data(0x68,0x0,1);
            Receive_Data(0x68,1); //read RTC
        }
        switch (letterPress){
        case 0://heat
            P6OUT &= ~BIT5;
            P6OUT |= BIT6;
            break;
        case 1://cool
            P6OUT &= ~BIT6;
            P6OUT |= BIT5;
            break;
        case 2:
            if (LM92_comp > ave_c){ //cool down device
                temp_compare = 1;
                P6OUT &= ~BIT6;
                P6OUT |= BIT5;
            }
            else if (LM92_comp < ave_c){ //heat up device
                temp_compare = 0;
                P6OUT &= ~BIT5;
                P6OUT |= BIT6;
            }
            break;
        case 3:
            timeTr = 1000;
            P6OUT &= ~BIT6;
            P6OUT &= ~BIT5;
        default:
            break;
        }
    }
    return 0;
}

//-------------------------TIMERS------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void){
    //P6OUT &= ~BIT6;
    TB0CCTL0 &= ~CCIFG;
    get_temp++;
    get_time++;
}
//---------------------------ADC-------------------------------------

void ADC(void){
    ADCCTL0 |= ADCENC | ADCSC; //starts adc
    TB0CCTL0 &= ~CCIFG; //clears timer
    while((ADCIFG & ADCIFG0) == 0){} //wait for ADC to clear

    voltage = (ADC_Value*3.3)/(4095); //gets voltage value from equaiton
    double in = 2196200 +((1.8639-voltage)/.00000388); //equation
    double root = sqrt(in); //equation
    temp_C = -1481.96 + root; //combine equation for temp
    if(ave_cnt != 0){ //populates array when array is empty
       ave_cnt--;
       average[ave_cnt] = temp_C;
    }
    else{ //shifts bits to make room for new data
         for(i=8; i>0; i--){
             average[i] = average[i-1];
         }
         average[0] = temp_C;

         for(i=0; i<press; i++){
             total = total +average[i];
         }
         ave_c = (total/press)*100; //convert to celcius average
         total = 0;
    }
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    ADC_Value = ADCMEM0; //get adc value
}
//---------------------------RTC----------------------------------
void RTC_in(){      //call this function when a state is on

    sec1 = received[0] & 0xF0; //tens place
    sec1 = sec1>>4;
    sec2 = received[0] & 0x0F; //once place
    sec = (sec1*10)+  sec2;
    if (on == 1){
        timeTr++;
    }
    if (timeTr >= 1300){
        letterPress = 3;
        timeTr = 1000;
    }

}
//---------------------------I2C----------------------------------
void Transmit_Data(unsigned int Slave_Address, unsigned int Data, int count){
    transmit[0]=Data;
    //transmit[1]=second;
    data_cnt = 0;
    UCB1TBCNT = count; //Transmit 1 Byte of Data;
    UCB1I2CSA = Slave_Address;
    UCB1CTLW0 |= UCTR; // Put in transmit mode
    UCB1CTLW0 |= UCTXSTT; //generate the start condition*/
    while((UCB1IFG & UCSTPIFG)==0){}
        UCB1IFG &= ~UCSTPIFG;
}
void Receive_Data(unsigned int Slave_Address, int count){
    //transmit[0]=first;
    //transmit[1]=second;
    data_cnt = 0;
    UCB1TBCNT = count; //Transmit 1 Byte of Data;
    UCB1I2CSA = Slave_Address;
    UCB1CTLW0 &= ~UCTR; // Put in receive mode
    UCB1CTLW0 |= UCTXSTT; //generate the start condition*/
    __delay_cycles(10000);
  //while((UCB1IFG & UCSTPIFG)==0){}
      //UCB1IFG &= ~UCSTPIFG;
    switch (Slave_Address){
    case 0x48:
        LM92_in();
        break;
    case 0x068:
        RTC_in();
        break;
    default:
        break;

    }
}
#pragma vector = EUSCI_B1_VECTOR
__interrupt void EUSCI_B1_I2C_ISR(void){
    switch(UCB1IV){
    case 0x16:
        received[data_cnt] = UCB1RXBUF;
        data_cnt = data_cnt + 1;
        break;
    case 0x18:
        if (UCB1I2CSA == 0x020){
            UCB1TXBUF = mov_ave[data_cnt];
            data_cnt++;
        }
        else{
        UCB1TXBUF = transmit[data_cnt];
        data_cnt = data_cnt + 1;
        }
        break;
    default:
        break;
    }
}
//-------------------------END I2C-----------------------------------

//---------------------------LM92------------------------------------


void LM92_in(void){
    MSB = (received[0]<<8);
    LSB = received[1];
    LM92_data = ((MSB + LSB)>>3);
    LM92_temp = (LM92_data*.0625)*10000;
    if(LM92_ave_cnt != 0){
        LM92_ave_cnt--;
        LM92_average[LM92_ave_cnt] = LM92_temp;
    }
    else{
        for(i=8; i>0;i--){
            LM92_average[i] = LM92_average[i-1];
        }
        LM92_average[0] = LM92_temp;

        for(i=0; i<press; i++){
            LM92_total = LM92_total + LM92_average[i];
        }
        LM92_ave_c = (LM92_total/press)*100;
        LM92_comp = LM92_ave_c/10000;
        LM92_total = 0;
        LM92_send = round(LM92_comp);

    }
}
//-----------------------KEYPAD STUFF--------------------------------

#pragma vector = PORT3_VECTOR
__interrupt void ISR_Port3_S1(void){
    if ((P3IN != 240) && (n == 0)){
        unsCol = P3IN; // read upper 4 bits
        unsCol &= (0xF0); //& to get rid of all unwanted bits except the asserted
        rowIn(); //read rows
        unsRow = P3IN; //read lower 4 bits
        unsRow &= (0x0F); //& and to get rid of all unwanted bits except the asserted
        key = unsCol |= unsRow; //or to get a single 8 bit value with the row and col
        findKey(); //based on 'key' value, return the correct character pressed
        n=1;
    }
    else if ((P3IN == 240) && (n == 1)){//clears upon button release
       for(i=0;i<3000;i++){}
    //clear interrupts
        P3IFG &= ~BIT4;
        P3IFG &= ~BIT5;
        P3IFG &= ~BIT6;
        P3IFG &= ~BIT7;
    }
}

void colIn(void){ //Read Col Values and interrupt if pressed
    P3DIR &= ~BIT4; //INIT col low input
    P3REN |= BIT4;
    P3OUT &= ~BIT4;
    P3DIR &= ~BIT5;
    P3REN |=  BIT5;
    P3OUT &= ~BIT5;
    P3DIR &= ~BIT6;
    P3REN |=  BIT6;
    P3OUT &= ~BIT6;
    P3DIR &= ~BIT7;
    P3REN |=  BIT7;
    P3OUT &= ~BIT7;

    //Set up interrupt based of  BIT 4-7 (Col inputs)
    P3IES &= ~BIT4; //Config IRQ Sensitivity L-to-H
    P3IFG &= ~BIT4; //Clear Interupt Flag
    P3IE |= BIT4; //Enable BITX Interrupt
    P3IES &= ~BIT5;
    P3IFG &= ~BIT5;
    P3IE |= BIT5;
    P3IES &= ~BIT6;
    P3IFG &= ~BIT6;
    P3IE |= BIT6;
    P3IES &= ~BIT7;
    P3IFG &= ~BIT7;
    P3IE |= BIT7;

    __enable_interrupt();

    P3DIR |= BIT0; //INIT row HIGH output
    P3OUT |= BIT0;
    P3DIR |= BIT1;
    P3OUT |= BIT1;
    P3DIR |= BIT2;
    P3OUT |= BIT2;
    P3DIR |= BIT3;
    P3OUT |= BIT3;
    //return 0;
}
void rowIn(void){ //Read Row Values
    P3DIR &= ~BIT0; //INIT row low input
    P3REN |= BIT0;
    P3OUT &= ~BIT0;
    P3DIR &= ~BIT1;
    P3REN |= BIT1;
    P3OUT &= ~BIT1;
    P3DIR &= ~BIT2;
    P3REN |= BIT2;
    P3OUT &= ~BIT2;
    P3DIR &= ~BIT3;
    P3REN |= BIT3;
    P3OUT &= ~BIT3;

    P3DIR |= BIT4; //INIT COL HIGH output
    P3OUT |= BIT4;
    P3DIR |= BIT5;
    P3OUT |= BIT5;
    P3DIR |= BIT6;
    P3OUT |= BIT6;
    P3DIR |= BIT7;
    P3OUT |= BIT7;
   // return 0;
}

void findKey(void){
    switch(key){
    case 0x11:
        keyPress = '1';
        press = 1; //gets assigned to average window variable "press" when "#" is pressed twice
        break;
    case 0x12:
        keyPress = '2';
        press = 2;
        break;
    case 0x14:
        keyPress = '3';
        press = 3;
        break;
    case 0x18:
        keyPress = 'A';
        letterPress = 0;
        break;
    case 0x21:
        keyPress = '4';
        press = 4;
        break;
    case 0x22:
        keyPress = '5';
        press = 5;
        break;
    case 0x24:
        keyPress = '6';
        press = 6;
        break;
    case 0x28:
        keyPress = 'B';
        letterPress = 1;
        break;
    case 0x41:
        keyPress = '7';
        press = 7;
        break;
    case 0x42:
        keyPress = '8';
        press = 8;
        break;
    case 0x44:
        keyPress = '9';
        press = 9;
        break;
    case 0x48:
        keyPress = 'C';
        letterPress = 2;

        break;
    case 0x81:
        keyPress = '*';
        break;
    case 0x82:
        keyPress = '0';
        press = 0;
        break;
    case 0x84:
        keyPress = '#';
        break;
    case 0x88:
        keyPress = 'D';
        letterPress = 3;
        break;
    default:
        keyPress = NULL;
        break;

    }
}
//-------------------------END KEYPAD-------------------------


