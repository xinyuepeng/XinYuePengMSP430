#if 1
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
__interrupt void watchdog_timer(void);
WDT_ISR(watchdog_timer)
__interrupt void watchdog_timer(void)
{
  P4OUT ^= 0x01;                            // Toggle P1.0 using exclusive-OR
}
#endif
