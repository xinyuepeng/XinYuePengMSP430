//******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
//
//  Description: Toggle P4.0 using software and TB_0 ISR. Toggles every
//  50000 SMCLK cycles. SMCLK provides clock source for TBCLK.
//  During the TB_0 ISR, P4.0 is toggled and 50000 clock cycles are added to
//  CCR0. TB_0 ISR is triggered every 50000 cycles. CPU is normally off and
//  used only during TB_ISR.
//  ACLK = n/a, MCLK = SMCLK = TBCLK = default DCO ~800kHz
//
//           MSP430F149
//         ---------------
//     /|\|            XIN|-
//      | |               |
//      --|RST        XOUT|-
//        |               |
//        |           P4.0|-->LED
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  BCSCTL2 = 0;                //MCLK = DCOCLK, SMCLK=DCOCLK, = EXT CRYTAL 32.768KHZ
  
  P4DIR |= 0x01;                            // P4.0 output
#if 0   
  TBCCTL0 = CCIE;                           // CCR0 interrupt enabled
  TBCCR0 = 50000;
  TBCTL = TBSSEL_1 + MC_2;                  // SMCLK, contmode
#else
  TACCR0 = 0x0A;      //10ms

    CCTL1 = OUTMOD_7;
    CCR1 = 0x05;
    
    CCTL2 = OUTMOD_7;
    CCR2 = 0x06;    
    TACTL = TASSEL_1 + MC_1;
    
    TBCCR0 = 0xA4;      //10ms
    TBCTL = TBSSEL_1 + MC_1 + TBIE;
#endif

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}
