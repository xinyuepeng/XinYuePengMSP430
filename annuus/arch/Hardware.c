#include "SysConfig.h"
#include "Hardware.h"

void Hardware_Initialize(void)
{
    IFG1 = 0;
    //WDTCTL=WDT_MDLY_32;         //WDT 32ms Internal Timer
    WDTCTL = WDTPW + WDTHOLD;
    P4DIR = 0x03;
    
    IFG1 &= ~WDTIFG;            //Clear pending WDT interrupts
    IE1 |= WDTIE;               //Enable WDT interrupt

    //BCSCTL2 |= SELM_2 + SELS;
    BCSCTL2 = 0;                //MCLK = DCOCLK, SMCLK=DCOCLK, = EXT CRYTAL 32.768KHZ
      
    P1DIR |= 0x0C;
    P1SEL |= 0x0C;
    
    //P1DIR |= 0x01;
    //P1SEL &= 0xFE;
    //TACCR0 = 0xFFF;   
    //TACCR0 = 0x148;   //20ms
#if 0    
    TACCR0 = 0xA4;      //10ms

    CCTL1 = OUTMOD_7;
    CCR1 = 0x01;
    
    CCTL2 = OUTMOD_7;
    CCR2 = 0x98;
        
    TACTL = TASSEL_1 + MC_1 + TAIE;
#else
    TACCR0 = 0x0A;      //10ms

    CCTL1 = OUTMOD_7;
    CCR1 = 0x05;
    
    CCTL2 = OUTMOD_7;
    CCR2 = 0x06;    
    TACTL = TASSEL_1 + MC_1;
    
    TBCCR0 = 0xA4;      //10ms
    TBCTL = TBSSEL_1 + MC_1 + TAIE;
#endif
}

#define SOUND_ALARM 0x10

void Alarm_BuzzorOff(void)
{
    IE1 &= ~0x01;
    P1SEL &= ~SOUND_ALARM;
    P1OUT &= ~SOUND_ALARM;
    P1DIR |= SOUND_ALARM;
}

void Alarm_BuzzorOn(void)
{
    WDTCTL = 0x5A12;
    IE1 |= 0x01;
}

#if 1
#pragma vector=WDT_VECTOR
__interrupt void Alarm_INTSound(void)
{
    P1SEL &=~ SOUND_ALARM;
    P1OUT ^=  SOUND_ALARM;    
    P1DIR |=  SOUND_ALARM; 
}
#endif