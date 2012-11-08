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

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, Toggle P4.0, Overflow ISR, DCO SMCLK
//
//  Description: Toggle P4.0 using software and Timer_B overflow ISR.
//  In this example an ISR triggers when TB overflows.  Inside the TB
//  overflow ISR P4.0 is toggled. Toggle rate is approximatlely 12Hz.
//  Proper use of the TBIV interrupt vector generator is demonstrated.
//  ACLK = n/a, MCLK = SMCLK = TBCLK = default DCO ~800kHz.
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
  P4DIR |= 0x01;                            // Set P4.0 to output direction
  TBCTL = TBSSEL_2 + MC_2 + TBIE;           // SMCLK, contmode, interrupt

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Timer_B7 Interrupt Vector (TBIV) handler
#pragma vector = TIMERB1_VECTOR
__interrupt void Timer_B(void)
{
 switch( TBIV )
 {
   case  2: break;                          // CCR1 not used
   case  4: break;                          // CCR2 not used
   case 14: P4OUT ^= 0x01;                  // overflow
            break;
 }
}


#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, Toggle P4.0, Overflow ISR, 32kHz ACLK
//
//  Description: Toggle P4.0 using software and the Timer_B overflow ISR.
//  In this example an ISR triggers when TB overflows. Inside the ISR P1.0
//  is toggled. Toggle rate is exactly 0.5Hz. Proper use of the TBIV
//  interrupt vector generator is demonstrated.
//  ACLK = TBCLK = 32kHz, MCLK = SMCLK = default DCO ~800kHz
//  //* An external watch crystal on XIN XOUT is required for ACLK *//	
//
//           MSP430F149
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
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
  P4DIR |= 0x01;                            // Set P4.0 to output direction
  TBCTL = TBSSEL_1 + MC_2 + TBIE;           // ACLK, contmode, interrupt

  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interrupt
}

// Timer_B7 Interrupt Vector (TBIV) handler
#pragma vector = TIMERB1_VECTOR
__interrupt void Timer_B(void)
{
 switch( TBIV )
 {
   case  2: break;                          // CCR1 not used
   case  4: break;                          // CCR2 not used
   case 14: P4OUT ^= 0x01;                  // overflow
           break;
  }
}

#endif

#if 0
//*****************************************************************************
//  MSP-FET430P140 Demo - Timer_B, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
//
//  Description: Toggle P1.0 using software and the TB_0 ISR. Timer_B is
//  configured for up mode, thus the timer overflows when TBR counts
//  to CCR0. In this example, CCR0 is loaded with 1000-1.
//  Toggle rate = 32768Hz/(2*1000) = 16.384Hz
//  ACLK = TBCLK = 32kHz, MCLK = SMCLK = default DCO ~800kHz
//  //* An external watch crystal on XIN XOUT is required for ACLK *//	
//
//           MSP430F149
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//*****************************************************************************

#include  <msp430x14x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P4DIR |= 0x01;                            // Set P1.0 to output direction
  TBCTL = TBSSEL_1 + TBCLR;                 // ACLK, clear TBR
  TBCCTL0 = CCIE;                           // TRCCR0 interrupt enabled
  TBCCR0 = 1000-1;
  TBCTL = TBSSEL_1 + MC_1;                  // ACLK, upmode

  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interrupt
}

// Timer B0 interrupt service routine
#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
    P4OUT ^= 0x01;                          // Toggle P1.0 using exclusive-OR
}


#endif

#if 0
//*******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, PWM TB1-6, Up Mode, DCO SMCLK
//
//  Description: This program generates six PWM outputs on P4.1-6 using
//  Timer_B configured for up mode. The value in CCR0, 512-1, defines the PWM
//  period and the values in CCR1-6 the PWM duty cycles. Using ~800kHz SMCLK
//  as TBCLK, the timer period is ~640us.
//  ACLK = na, SMCLK = MCLK = TBCLK = default DCO ~800kHz.
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P4.1/TB1|--> CCR1 - 75% PWM
//           |         P4.2/TB2|--> CCR2 - 25% PWM
//           |         P4.3/TB3|--> CCR3 - 12.5% PWM
//           |         P4.4/TB4|--> CCR4 - 6.25% PWM
//           |         P4.5/TB5|--> CCR5 - 3.125% PWM
//           |         P4.6/TB6|--> CCR6 - 1.5625% PWM
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
  P4DIR |= 0x7e;                            // P4.1 - P4.6 output
  P4SEL |= 0x7e;                            // P4.1 - P4.6 TBx options
  TBCCR0 = 512-1;                           // PWM Period
  TBCCTL1 = OUTMOD_7;                       // CCR1 reset/set
  TBCCR1 = 384;                             // CCR1 PWM duty cycle
  TBCCTL2 = OUTMOD_7;
  TBCCR2 = 128;
  TBCCTL3 = OUTMOD_7;
  TBCCR3 = 64;
  TBCCTL4 = OUTMOD_7;
  TBCCR4 = 32;
  TBCCTL5 = OUTMOD_7;
  TBCCR5 = 16;
  TBCCTL6 = OUTMOD_7;
  TBCCR6 = 8;
  TBCTL = TBSSEL_2 + MC_1;                  // SMCLK, up mode

  _BIS_SR(CPUOFF);                          // Enter LPM0
}


#endif

#if 0
//*******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, PWM TB1-6, Up Mode, 32kHz ACLK
//
//  Description: This program generates six PWM outputs on P4.1-6 using
//  Timer_B configured for up mode. The value in CCR0, 512-1, defines the PWM
//  period and the values in CCR1-6 the PWM duty cycles. Using 32kHz ACLK
//  as TBCLK, the timer period is 15.6ms. Normal operating mode is LPM3.
//  ACLK = TBCLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO ~800kHz.
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |         P4.1/TB1|--> CCR1 - 75% PWM
//           |         P4.2/TB2|--> CCR2 - 25% PWM
//           |         P4.3/TB3|--> CCR3 - 12.5% PWM
//           |         P4.4/TB4|--> CCR4 - 6.25% PWM
//           |         P4.5/TB5|--> CCR5 - 3.125% PWM
//           |         P4.6/TB6|--> CCR6 - 1.5625% PWM
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
  P4DIR |= 0x7e;                            // P4.1 - P4.6 output
  P4SEL |= 0x7e;                            // P4.1 - P4.6 TBx options
  TBCCR0 = 512-1;                           // PWM Period
  TBCCTL1 = OUTMOD_7;                       // CCR1 reset/set
  TBCCR1 = 384;                             // CCR1 PWM duty cycle
  TBCCTL2 = OUTMOD_7;
  TBCCR2 = 128;
  TBCCTL3 = OUTMOD_7;
  TBCCR3 = 64;
  TBCCTL4 = OUTMOD_7;
  TBCCR4 = 32;
  TBCCTL5 = OUTMOD_7;
  TBCCR5 = 16;
  TBCCTL6 = OUTMOD_7;
  TBCCR6 = 8;

  TBCTL = TBSSEL_1 + MC_1;                  // ACLK, up mode

  _BIS_SR(LPM3_bits);                       // Enter LPM3
}


#endif

#if 0// Cannot test now

//*******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, PWM TB1-2, Up Mode, HF XTAL ACLK
//
//  Description: This program generates two PWM outputs on P4.1,2 using
//  Timer_B configured for up mode. The value in CCR0, 512-1, defines the PWM
//  period and the values in CCR1 and CCR2 the PWM duty cycles. Using HF XTAL
//  ACLK as TBCLK, the timer period is HF XTAL/512 with a 75% duty cycle
//  on P4.1 and 25% on P4.2.
//  ACLK = MCLK = TBCLK = HF XTAL
//  //* HF XTAL REQUIRED AND NOT INSTALLED ON FET *//
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//	
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | HF XTAL (455k - 8MHz)
//         --|RST          XOUT|-
//           |                 |
//           |         P4.1/TB1|--> CCR1 - 75% PWM
//           |         P4.2/TB2|--> CCR2 - 25% PWM
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

void main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 |= XTS;                           // ACLK = LFXT1 = HF XTAL

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_3;                        // MCLK= LFXT1 (safe)
  P4DIR |= 0x06;                            // P4.1 and P4.2 output
  P4SEL |= 0x06;                            // P4.2 and P4.2 TB1/2 options
  TBCCR0 = 512-1;                           // PWM Period
  TBCCTL1 = OUTMOD_7;  			                // CCR1 reset/set
  TBCCR1 = 384;                             // CCR1 PWM duty cycle
  TBCCTL2 = OUTMOD_7;  			                // CCR2 reset/set
  TBCCR2 = 128;                             // CCR2 PWM duty cycle
  TBCTL = TBSSEL_1 + MC_1;                  // ACLK, up mode

  _BIS_SR(CPUOFF);                          // Enter LPM0
}


#endif

#if 0
//*******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, PWM TB1-2, Up/Down Mode, DCO SMCLK
//
//  Description: This program generates two PWM outputs on P4.1,2 using
//  Timer_B configured for up/down mode. The value in CCR0, 128, defines the
//  PWM period/2 and the values in CCR1 and CCR2 the PWM duty cycles. Using
//  ~800kHz SMCLK as TBCLK, the timer period is ~320us with a 75% duty cycle
//  on P4.1 and 25% on P4.2.
//  SMCLK = MCLK = TBCLK = default DCO ~800kHz.
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P4.1/TB1|--> CCR1 - 75% PWM
//           |         P4.2/TB2|--> CCR2 - 25% PWM
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//  (128-32)*2/(128*2)
//******************************************************************************

#include  <msp430x14x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P4DIR |= 0x06;                            // P4.1 and P4.2 output
  P4SEL |= 0x06;                            // P4.2 and P4.2 TB1/2 otions
  TBCCR0 = 128;                             // PWM Period/2
  TBCCTL1 = OUTMOD_6;                       // CCR1 toggle/set
  TBCCR1 = 32;                              // CCR1 PWM duty cycle
  TBCCTL2 = OUTMOD_6;                       // CCR2 toggle/set
  TBCCR2 = 96;                              // CCR2 PWM duty cycle
  TBCTL = TBSSEL_2 + MC_3;                  // SMCLK, up-down mode

  _BIS_SR(CPUOFF);                          // Enter LPM0
}

#endif

#if 0
//*******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, PWM TB1-2, Up/Down Mode, 32kHz ACLK
//
//  Description: This program generates two PWM outputs on P4.1,2 using
//  Timer_B configured for up/down mode. The value in CCR0, 128, defines the
//  PWM period/2 and the values in CCR1 and CCR2 the PWM duty cycles. Using
//  32kHz ACLK as TBCLK, the timer period is 7.8ms with a 75% duty cycle on
//  P4.1 and 25% on P4.2. Normal operating mode is LPM3.
//  ACLK = TBCLK = LFXT1 = 32768Hz, MCLK = default DCO ~800kHz.
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |         P4.1/TB1|--> CCR1 - 75% PWM
//           |         P4.2/TB2|--> CCR2 - 25% PWM
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
  P4DIR |= 0x06;                            // P4.1 and P4.2 output
  P4SEL |= 0x06;                            // P4.1 and P4.2 TB1/2 otions
  TBCCR0 = 128;                             // PWM Period/2
  TBCCTL1 = OUTMOD_6;                       // CCR1 toggle/set
  TBCCR1 = 32;                              // CCR1 PWM duty cycle
  TBCCTL2 = OUTMOD_6;                       // CCR2 toggle/set
  TBCCR2 = 96;                              // CCR2 PWM duty cycle
  TBCTL = TBSSEL_1 + MC_3;                  // ACLK, up-down mode

  _BIS_SR(LPM3_bits);                       // Enter LPM3
}


#endif

#if 1 //Cannot test now
//*******************************************************************************
//  MSP-FET430P140 Demo - Timer_B, PWM TB1-2, Up/Down Mode, HF XTAL ACLK
//
//  Description: This program generates two PWM outputs on P4.1,2 using
//  Timer_B configured for up/down mode. The value in CCR0, 128, defines the
//  PWM period/2 and the values in CCR1 and CCR1 the PWM duty cycles. Using
//  HF XTAL ACLK as TBCLK, the timer period is HF XTAL/256 with a 75% duty
//  cycle on P4.1 and 25% on P4.2.
//  ACLK = MCLK = TBCLK = HF XTAL
//  //* HF XTAL REQUIRED AND NOT INSTALLED ON FET *//
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//	
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | HF XTAL (455k - 8MHz)
//         --|RST          XOUT|-
//           |                 |
//           |         P4.1/TB1|--> CCR1 - 75% PWM
//           |         P4.2/TB2|--> CCR2 - 25% PWM
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

void main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW +WDTHOLD;                  // Stop WDT
  BCSCTL1 |= XTS;                           // ACLK= LFXT1 = HF XTAL

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_3;                        // MCLK= LFXT1 (safe)
  P4DIR |= 0x06;                            // P4.1 and P4.2 output
  P4SEL |= 0x06;                            // P4.2 and P4.2 TB1/2 otions
  TBCCR0 = 128;                             // PWM Period/2
  TBCCTL1 = OUTMOD_6;                       // CCR1 toggle/set
  TBCCR1 = 32;                              // CCR1 PWM duty cycle
  TBCCTL2 = OUTMOD_6;                       // CCR2 toggle/set
  TBCCR2 = 96;                              // CCR2 PWM duty cycle
  TBCTL = TBSSEL_1 + MC_3;                  // ACLK, up-down mode

  _BIS_SR(CPUOFF);                          // Enter LPM0
}


#endif