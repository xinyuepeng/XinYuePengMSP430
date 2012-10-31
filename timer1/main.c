#include <msp430x14x.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
#if 1    
    BCSCTL1 &= ~XT2OFF;
    P4DIR |= 0x01;
    TACTL = TASSEL_1 + MC_2 + TAIE;
  
    CCR0 = 512-1;
    CCTL0 = OUTMOD_7 + CCIE;
#if 0    
    CCTL1 = OUTMOD_7 + CCIE;
    CCR1 = 384;
    
    CCTL2 = OUTMOD_7 + CCIE;
    CCR2 = 128;
#endif    
    _BIS_SR(LPM3_bits + GIE);
    P4OUT ^= 0x01;
    while(1);
#else
    P1DIR |= 0x0C;
    P1SEL |= 0x0C;
    P4DIR |= 0x01;
    P4OUT ^= 0x01;
    
    CCR0 = 512-1;
    CCTL1 = OUTMOD_7;
    CCR1 = 384;
    
    CCTL2 = OUTMOD_7;
    CCR2 = 128;
    
    TACTL = TASSEL_1 + MC_1;
    _BIS_SR(LPM3_bits);
#endif    
}

__interrupt void TIMOVH_C(void)
{
    P4OUT ^= 0x01;
}

__interrupt void TIMMOD1_C(void)
{
}

#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A0(void)
{
    P4OUT ^= 0x01;
}

#if 1
#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A(void)
{
    switch(TAIV)
    {
    case 2: 
      P4OUT ^= 0x01;
      break;        
    case 4: break;
    case 10:
        
        break;
    }
}
#endif