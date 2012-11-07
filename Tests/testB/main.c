#if 0
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
  TBCCTL0 = CCIE;                           // CCR0 interrupt enabled
  TBCCR0 = 50000;
  TBCTL = TBSSEL_1 + MC_2;                  // SMCLK, contmode

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Timer B0 interrupt service routine
#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
  P4OUT ^= 0x01;                            // Toggle P4.0
  TBCCR0 += 50000;                          // Add Offset to CCR0
}

#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
//
//  Description: Toggle P1.0 using software and TB_0 ISR. Timer_B is
//  configured for up mode, thus the timer overflows when TBR counts
//  to CCR0. In this example, CCR0 is loaded with 20000.
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
  P4DIR |= 0x01;                            // Set P1.0 to output direction
  TBCCTL0 = CCIE;                           // TBCCR0 interrupt enabled
  TBCCR0 = 20000;
  TBCTL = TBSSEL_2 + MC_1;                  // SMCLK, upmode

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Timer B0 interrupt service routine
#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
  P4OUT ^= 0x01;                            // Toggle P1.0 using exclusive-OR
}
#endif

#if 1
//******************************************************************************
//  MSP-FET430P140 Demo - Timer_A, Toggle P1.0-3, Cont. Mode ISR, DCO SMCLK
//
//  Description: Use Timer_A CCRx units and overflow to generate four
//  independent timing intervals. For demonstration, CCR0, CCR1 and CCR2
//  output units are optionally selected with port pins P1.1, P1.2 and P1.3
//  in toggle mode. As such, these pins will toggle when respective CCRx
//  registers match the TAR counter. Interrupts are also enabled with all
//  CCRx units, software loads offset to next interval only - as long as the
//  interval offset is added to CCRx, toggle rate is generated in hardware.
//  Timer_A overflow ISR is used to toggle P1.0 with software. Proper use of
//  the TAIV interrupt vector generator is demonstrated.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO ~800kHz
//
//  As coded with TACLK ~800kHz DCO, toggle rates are:
//  P1.1= CCR0 = 800kHz/(2*200) ~2000Hz
//  P1.2= CCR1 = 800kHz/(2*1000) ~400Hz
//  P1.3= CCR2 = 800kHz/(2*10000) ~40Hz
//  P1.0= overflow = 800kHz/(2*65536) ~6Hz
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P1.1/TA0|--> CCR0
//           |         P1.2/TA1|--> CCR1
//           |         P1.3/TA2|--> CCR2
//           |             P1.0|--> Overflow/software
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
  P1SEL |= 0x0E;                            // P1.1 - P1.4 option select
  P1DIR |= 0x0F;                            // P1.0 - P1.4 outputs
  CCTL0 = OUTMOD_4 + CCIE;                  // CCR0 toggle, interrupt enabled
  CCTL1 = OUTMOD_4 + CCIE;                  // CCR1 toggle, interrupt enabled
  CCTL2 = OUTMOD_4 + CCIE;                  // CCR2 toggle, interrupt enabled
  TACTL = TASSEL_2 +  MC_2 + TAIE;          // SMCLK, Contmode, int enabled

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
__interrupt void Timer_A0 (void);
TIMERA0_ISR(Timer_A0)
__interrupt void Timer_A0 (void)
{
  CCR0 += 200;                              // Add Offset to CCR0
}

// Timer_A3 Interrupt Vector (TAIV) handler
__interrupt void Timer_A1(void);
TIMERA1_ISR(Timer_A1)
__interrupt void Timer_A1(void)
{
  switch( TAIV )
  {
  case  2: CCR1 += 1000;                    // Add Offset to CCR1
           break;
  case  4: CCR2 += 10000;                   // Add Offset to CCR2
           break;
  case 10: P1OUT ^= 0x01;                   // Timer_A3 overflow
           break;
 }
}
#endif