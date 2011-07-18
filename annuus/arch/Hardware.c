#include "SysConfig.h"
#include "Hardware.h"

void Hardware_Initialize(void)
{
    IFG1 = 0;
    WDTCTL=WDT_MDLY_32;         //WDT 32ms Internal Timer
    P4DIR = 0x03;
    
    IFG1 &= ~WDTIFG;            //Clear pending WDT interrupts
    IE1 |= WDTIE;               //Enable WDT interrupt
}