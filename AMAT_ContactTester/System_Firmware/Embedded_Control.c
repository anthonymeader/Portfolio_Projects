#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

int i = 0;
int j = 0;
int k = 0;
unsigned int min;
unsigned int min_adc;

volatile int n = 0;
unsigned int resistance_array[15];

//Actuation Global Variables
unsigned int retraction_distace = 0;
unsigned int retraction = 0;
volatile unsigned int pressure;
volatile int foward = 1;
volatile int reverse = 0;
int reset_motor = 0;
//--------------------------

//UART Global Variables
unsigned int DataIn = 0; //UART Instructions Received
unsigned int previous_data = 1;

//ADC Global Variables
volatile unsigned int resistance_ADC;
volatile unsigned int ADC_Value_1;
volatile unsigned int ADC_Value_2;
int channel;
int previous_channel = 0;
//--------------------
volatile uint16_t resistance = 0;



//pressure on a9
void read_pressure(void){
    channel = 0;

    if (previous_channel == 1){
        ADCCTL0  &= ~ADCENC; //CLEAR THIS REGISTER TO CHANGE CHANNELS
        ADCMCTL0 &= ~ADCINCH_4; //Clear channel select 4
        ADCMCTL0 |= ADCINCH_9; //Select channel 9
        ADCCTL0 |= ADCENC | ADCSC; //start interrupt
    }
    else if (previous_channel == 0){
        ADCCTL0 |= ADCENC | ADCSC; //start interrupt
    }
    else{
        ADCMCTL0 |= ADCINCH_9; //Select channel 9
        ADCCTL0 |= ADCENC | ADCSC; //start interrupt
    }
    __delay_cycles(200);

}
//resistance on a4
void read_resistance(void){
    channel = 1;

    if (previous_channel == 0){
        ADCCTL0  &= ~ADCENC; //CLEAR THIS REGISTER TO CHANGE CHANNELS
        ADCMCTL0 &= ~ADCINCH_9; //Clear channel select 9
        ADCMCTL0 |= ADCINCH_4; //Select channel 4
        ADCCTL0 |= ADCENC | ADCSC; //start interrupt
    }
    else if (previous_channel == 1){
        ADCCTL0 |= ADCENC | ADCSC; //start interrupt
    }
    else{
        ADCMCTL0 |= ADCINCH_4; //Select channel 4
        ADCCTL0 |= ADCENC | ADCSC; //start interrupt
    }
    __delay_cycles(200);
}

void step_motor(void){
    if (foward == 1 && reverse == 0){ //bring motor foward
        P3DIR |= BIT1;
        P3OUT ^= BIT0;
    }
    else if (reverse == 1 && foward == 0){
        for (i = retraction_distace; i > 0; i--){ //retract motor
            P3OUT ^= BIT0;
            __delay_cycles(350);
        }
        reverse = 0;
        foward = 1;
    }

}
void init_motor(void){
    P3DIR &= ~BIT1; //motor retract
    if (j < reset_motor){
        P3OUT ^= BIT0;
        j = j+1;
        //  __delay_cycles(800);

    }
    else if (j == reset_motor){
        j = 0;
        DataIn = 0x00;
    }
    /* for (i = reset_motor; i > 0; i--){ //make the distance user setable
        P3OUT ^= BIT0;
        __delay_cycles(800);
    }
    P3DIR |= BIT1; //motor forward*/

}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //----ADC Channels----
    P5SEL1 |= BIT1; //A9, pressure adc
    P5SEL0 |= BIT1;
    P1SEL1 |= BIT4; //A4, resistance adc
    P1SEL0 |= BIT4;
    //--------------------

    //----UART Channels---
    P4SEL1 &= ~BIT2; //Rx
    P4SEL0 |= BIT2;
    P4SEL1 &= ~BIT3; //Tx
    P4SEL0 |= BIT3;

    //----UART CONFIG----
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__ACLK; //9600 Baurd Rate Setup
    UCA1BRW = 3;
    UCA1MCTLW |= 0x9200;
    UCA1CTLW0 &= ~UCSWRST;
    //-------------------


    //----Actuator GPIO Pins----
    P3DIR &= ~BIT2;  // Set P3.2 (ENA) as output
    P3DIR |= BIT1;  // Set P3.1 (DIR) as output, rotates counter clockwise, input rotates clockwise
    P3DIR |= BIT0;  // Set P3.0 (PUL) as output
    //--------------------------
    //----Resistance Circuit GPIO----
    P2DIR |= BIT0;
    P2OUT |= BIT0;
    P2DIR |= BIT1;
    P2OUT |= BIT1;
    P2DIR |= BIT2;
    P2OUT |= BIT2;
    //-------------------------------

    //Fans
    P6DIR |= BIT0;
    P6OUT |= BIT0; //Turn Fan On
    //Test LED
    P1DIR |= BIT2;
    P1OUT &= ~BIT2;

    PM5CTL0 &= ~LOCKLPM5;


    //-------ADC CONFIG------
    //check ADCSR, ADCDF
    ADCCTL0 &= ~ADCSHT;
    ADCCTL0 |= ADCSHT_2; //16 conversion cycles
    ADCCTL0 |= ADCON;
    //ADCCTL0 |= ADCSREF_5; //external positive reference

    ADCCTL1 |= ADCSSEL_2;
    ADCCTL1 |= ADCSHP;
    ADCMCTL0 |= ADCINCH_9; //Select channel 9, pressure
    ADCMCTL0 |= ADCSREF_7; //reference select

    ADCCTL2 &= ~ADCRES;
    ADCCTL2 |= ADCRES_2; //12 bit res

    ADCIE |= ADCIE0;
    //-----------------------

    //----Actuator GPIO Pins----
    P3OUT |= BIT2;
    P3OUT |= BIT1;
    P3OUT |= BIT0;

    __enable_interrupt();


    while(1){
        switch(DataIn){
        case 0x41:
            step_motor(); //step motor
            read_pressure(); //read pressure
            previous_channel = 0;
            break;
        case 0x42: //Take resistance measurement
            P2OUT &= ~BIT0; //clearning these GPIO disconnects resistance circuit from pws
            P2OUT &= ~BIT1;
            P2OUT &= ~BIT2;
            resistance = 0;
            min = 0;
            for (k = 0; k<15;k++){
                for(i = 0; i<15;i++){
                    read_resistance();
                    __delay_cycles(500);   //Delay to let buffer catch up
                    previous_channel = 1;
                    resistance_array[i] = resistance_ADC; //ADD resistance to array for DEBUG
                }
                min_adc = resistance_array[0];
                for (i = 0; i<15; i++){
                    if (resistance_array[i] < min_adc){
                        min_adc = resistance_array[i];

                    }
                }
                min = min+min_adc;
            }
            resistance = min/15; //average 15 measurements
            ADC_Value_1 = (resistance >> 4) & 0xFF; //Highest Byte (ie 0xFFF would be 0xFF) (Upper 8 Bits)
            ADC_Value_2 = resistance & 0x0F; //The lower nibble (ie 0xFFF would be 0x0F) (Lower 4 Bits)
            P2OUT |= BIT0; //asserting these connects the PWS back to the resistance circuit
            P2OUT |= BIT1;
            P2OUT |= BIT2;
            DataIn = 0x00; //clear command register
            UCA1IE |= UCTXIE; //start DUT resistance transmission
            break;
        case 0x43: //reset Motor
            foward = 1;
            reverse = 0;
            previous_channel = 1;
            init_motor(); //retract motor farther back when test is stopped
            /*foward = 1;
            reverse = 0;
            previous_channel = 1;
            retraction = 0;
            retraction_distace = 0;
            DataIn = 0x00;*/
            break;
        case 0x44: //Define actuation distance
            previous_data = DataIn;
            pressure = 0;
            resistance = 0;
            DataIn = 0x00;
            break;
        case 0x45:
            previous_data = DataIn;
            DataIn = 0x00;
            break;
        default:
            UCA1IE |= UCRXIE; //wait for command
            break;
        }
        __delay_cycles(200);   //Delay to let buffer catch up


    }
    return 0;
}
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    if (channel == 1){
        resistance_ADC = ADCMEM0;
    }
    else if (channel == 0){
        pressure = ADCMEM0;
        if (pressure > 1000){
            ADC_Value_1 = (pressure >> 4) & 0xFF; //Highest Byte (ie 0xFFF would be 0xFF) (Upper 8 Bits)
            ADC_Value_2 = pressure & 0x0F; //The lower nibble (ie 0xFFF would be 0x0F) (Lower 4 Bits)
            pressure = 0;
            foward = 0;
            reverse = 1;
            P3DIR &= ~BIT1; // reverse motor
            DataIn = 0x00; //clear command
            UCA1IE |= UCTXIE; //start pressure transmission
        }
    }

}
#pragma vector=EUSCI_A1_VECTOR
__interrupt void EUSCI_A1_TX_ISR(void)
{
    switch(UCA1IV){
    case 0x02:
        if (previous_data == 0x44){
            j = 0;
            retraction_distace = UCA1RXBUF * 160; //convert to mm
            if (retraction_distace >= 11000){
                retraction_distace = 11000;
            }
            previous_data = 0x00;
        }
        else if (previous_data == 0x45){
            retraction = UCA1RXBUF * 160; //convert to mm
            reset_motor = 17400-retraction; //init_motor retraction
            previous_data = 0x00;
            if ((retraction + retraction_distace) >= 17400){ //max retraction
                retraction_distace = reset_motor; //for big mounting black
            }
        }
        else{
            DataIn = UCA1RXBUF; //Gets Data from Laptop
        }
        break;
    case 0x04:
        UCA1TXBUF = ADC_Value_1; //Transmit Upper Byte
        __delay_cycles(5000);   //Delay to let buffer catch up
        UCA1TXBUF = ADC_Value_2; //Transmit Lower Nibble
        __delay_cycles(5000);   //Delay to let buffer catch up
        UCA1IFG &= ~UCTXCPTIFG;  //Clear Transmit Interrupt
        break;
    default:
        break;
    }
    // UCA1IE &= ~UCRXIE; //STOP UART
    UCA1IE &= ~UCTXIE;
    // __delay_cycles(10000);
}


