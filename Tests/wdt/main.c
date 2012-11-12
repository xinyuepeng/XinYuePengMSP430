#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - WDT, Toggle P4.0, Interval Overflow ISR, DCO SMCLK
//
//  Description: Toggle P4.0 using software timed by the WDT ISR. Toggle rate
//  is approximately 30ms based on default ~800kHz DCO/SMCLK clock source
//  used in this example for the WDT.
//  ACLK= n/a, MCLK= SMCLK= default DCO ~800kHz
//
//		             MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P4.0|-->LED
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

void main(void)
{
  WDTCTL = WDT_MDLY_32;                     // Set WDT interval to ~30ms
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P4DIR |= 0x01;                            // Set P4.0 to output direction

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Watchdog Timer interrupt service routine
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  P4OUT ^= 0x01;                            // Toggle P1.0 using exclusive-OR
}
#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - WDT, Toggle P4.0, Interval Overflow ISR, 32kHz ACLK
//
//  Description: Toggle P4.0 using software timed by WDT ISR. Toggle rate is
//  exactly 250ms based on 32kHz ACLK WDT clock source.  In this example the
//  WDT is configured to divide 32768 watch-crystal(2^15) by 2^13 with an ISR
//  triggered @ 4Hz.
//  ACLK= LFXT1= 32768, MCLK= SMCLK= DCO ~800kHz
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//		             MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P4.0|-->LED
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

void main(void)
{
  WDTCTL = WDT_ADLY_250;                    // WDT 250ms, ACLK, interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P4DIR |= 0x01;                            // Set P1.0 to output direction

  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/interrupt
}

// Watchdog Timer interrupt service routine
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  P4OUT ^= 0x01;                            // Toggle P1.0 using exclusive-OR
}


#endif


#if 1 //Cannot not test.
//******************************************************************************
//  MSP-FET430P140 Demo - WDT, Toggle P1.0, Interval Overflow ISR, HF XTAL ACLK
//
//  Description: Toggle P1.0 using software timed by the WDT ISR. In this
//  example the WDT is clocked with ACLK which is the same as the XTAL and
//  divided by the WDT by 512 triggering and ISR that will toggle P1.0.
//  ACLK= MCLK= LFXT1= HF XTAL
//  //* HF XTAL REQUIRED AND NOT INSTALLED ON FET *//
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//	
//
//		             MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | HF XTAL (455k - 8Mhz)
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
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
  WDTCTL = WDT_ADLY_16;                     // ACLK/512, interval timer
  BCSCTL1 |= XTS;                           // ACLK = LFXT1 = HF XTAL

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1 (safe)
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Watchdog Timer interrupt service routine
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0 using exclusive-OR
}

#endif