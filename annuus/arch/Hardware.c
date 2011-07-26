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

    BCSCTL2 |= SELM_2 + SELS;
    
    P1DIR |= 0x0C;
    P1SEL |= 0x0C;
    
    //P1DIR |= 0x01;
    //P1SEL &= 0xFE;
    //TACCR0 = 0xFFF;   
    //TACCR0 = 0x148;   //20ms
    TACCR0 = 0xA4;      //10ms

    CCTL1 = OUTMOD_7;
    CCR1 = 0x44;
    
    CCTL2 = OUTMOD_7;
    CCR2 = 0x88;
        
    
    TACTL = TASSEL_1 + MC_1 + TAIE;
}