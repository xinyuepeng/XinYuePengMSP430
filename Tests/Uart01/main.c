#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 115200 Echo ISR, HF XTAL ACLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM0,
//  USART0 RX interrupt triggers TX Echo.
//  ACLK = MCLK = UCLK0 = LFXT1 = 8MHz
//  Baud rate divider with 8MHz XTAL = 8000000/115200 = 0069 (0045h)
//  //* An external 8Mhz XTAL on XIN XOUT is required for ACLK *//
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//	
//
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 8MHz
//          --|RST          XOUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 115200 - 8N1
//            |             P3.5|<------------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= XTS;                           // ACLK= LFXT1= HF XTAL

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1 (safe)
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK= ACLK
  UBR00 = 0x45;                             // 8MHz 115200
  UBR10 = 0x00;                             // 8MHz 115200
  UMCTL0 = 0x00;                            // 8MHz 115200 modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}
#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 115200 Echo ISR, XT2 HF XTAL ACLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM0,
//  USART0 RX interrupt triggers TX Echo. Though not required, MCLK= XT2.
//  ACLK = n/a, MCLK = SMCLK = UCLK0 = XT2 = 8MHz
//  Baud rate divider with 8Mhz XTAL = 8000000/115200 = 0069 (0045h)
//  //* An external 8MHz XTAL on X2IN X2OUT is required for XT2CLK *//	
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//	
//
//
//                MSP430F149
//             -----------------
//         /|\|            XT2IN|-
//          | |                 | 8Mhz
//          --|RST        XT2OUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 115200 - 8N1
//            |             P3.5|<------------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD

  BCSCTL1 &= ~XT2OFF;                       // XT2on

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_2 + SELS;                 // MCLK = SMCLK = XT2 (safe)
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL1;                          // UCLK = SMCLK
  UBR00 = 0x45;                             // 8MHz 115200
  UBR10 = 0x00;                             // 8MHz 115200
  UMCTL0 = 0x00;                            // 8MHz 115200 modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}
#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, RX ISR used. In the Mainloop UART0
//  is made ready to receive one character with interrupt active. The Mainloop
//  waits in LPM3. The UART0 ISR forces the Mainloop to exit LPM3 after
//  receiving one character which echo's back the received character.
//  ACLK = UCLK0 = LFXT1 = 32768, MCLK = SMCLK = DCO~ 800k
//  Baud rate divider with 32768hz XTAL @2400 = 32768Hz/2400 = 13.65 (000Dh)
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//                MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.4|----------->
//           |                 | 2400 - 8N1
//           |             P3.5|<-----------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x0D;                             // 32k/2400 - 13.65
  UBR10 = 0x00;                             //
  UMCTL0 = 0x6B;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
  TXBUF0 = 0x55;
  // Mainloop
  for (;;)
  {
    _BIS_SR(LPM3_bits + GIE);               // Enter LPM3 w/interrupt
    while (!(IFG1 & UTXIFG0));              // USART0 TX buffer ready?
    TXBUF0 = RXBUF0;                        // RXBUF0 to TXBUF0
  }
}

// UART0 RX ISR will for exit from LPM3 in Mainloop
#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  _BIC_SR_IRQ(LPM3_bits);                   // Clear LPM3 bits from 0(SR)
}

#endif

#if 0 //Cannot test.
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 9600 Echo ISR, HF XTAL ACLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM0,
//  USART0 RX interrupt triggers TX Echo. Though not required, MCLK = LFXT1
//  ACLK = MCLK = UCLK0 = LFXT1 = 3.58MHz
//  Baud rate divider with 3.58Mhz XTAL @9600 = 3.58MHz/9600 = 372 (0174h)
//  //* An external 3.58Mhz XTAL on XIN XOUT is required for ACLK *//	
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 3.58MHz
//          --|RST          XOUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 9600 - 8N1
//            |             P3.5|<------------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= XTS;                           // ACLK = LFXT1 = HF XTAL

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1 (safe)
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x74;                             // 3.58Mhz/9600 - 372
  UBR10 = 0x01;                             //
  UMCTL0 = 0x00;                            // no modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}
#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, RX ISR used. In the Mainloop UART0
//  is made ready to receive one character with interrupt active. The Mainloop
//  waits in LPM3. The UART0 ISR forces the Mainloop to exit LPM3 after
//  receiving one character which echo's back the received character.
//  ACLK = UCLK0 = LFXT1 = 32768, MCLK = SMCLK = DCO~ 800k
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (0003h 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//                MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.4|----------->
//           |                 | 9600 - 8N1
//           |             P3.5|<-----------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x03;                             // 32k/9600 - 3.41
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4A;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
  TXBUF0 = 0x55;
  // Mainloop
  for (;;)
  {
    _BIS_SR(LPM3_bits + GIE);               // Enter LPM3 w/interrupt
    while (!(IFG1 & UTXIFG0));              // USART0 TX buffer ready?
    TXBUF0 = RXBUF0;                        // RXBUF0 to TXBUF0
  }
}

// UART0 RX ISR will for exit from LPM3 in Mainloop
#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  _BIC_SR_IRQ(LPM3_bits);                   // Clear LPM3 bits from 0(SR)
}
#endif

#if 0 //Cannot test
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 19200 Echo ISR, HF XTAL ACLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM0,
//  USART0 RX interrupt triggers TX Echo. Though not required, MCLK = LFXT1.
//  ACLK = MCLK = UCLK0 = 3.58MHz
//  Baud rate divider with 3.58Mhz XTAL @19200 = 3.58MHz/19200 = 186 (00BAh)
//  //* An external 3.58Mhz XTAL on XIN XOUT is required for ACLK *//
//
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 3.58MHz
//          --|RST          XOUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 19200 - 8N1
//            |             P3.5|<------------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= XTS;                           // ACLK = LFXT1 = HF XTAL

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1 (safe)
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0xBA;                             // 3.58Mhz 19200 - 186
  UBR10 = 0x00;                             //
  UMCTL0 = 0x00;                            // no modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}

#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 19200 Echo ISR, XT2 HF XTAL ACLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM0,
//  USART0 RX interrupt triggers TX Echo. Though not required, MCLK = XT2.
//  ACLK = n/a, MCLK = SMCLK = UCLK0 = XT2 = 8MHz
//  Baud rate divider with 8MHz XTAL @19200 = 8MHz/19200 = 416.66 ~ 417 (01A0h)
//  //* An external 8MHz XTAL on X2IN X2OUT is required for XT2CLK *//
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//		
//
//
//                MSP430F149
//             -----------------
//         /|\|            XT2IN|-
//          | |                 | 8MHz
//          --|RST        XT2OUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 19200 - 8N1
//            |             P3.5|<------------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  BCSCTL1 &= ~XT2OFF;                       // XT2on

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_2 + SELS;                 // MCLK = SMCLK = XT2 (safe)

  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL1;                          // UCLK = SMCLK
  UBR00 = 0xA0;                             // 8Mhz/19200 ~ 417
  UBR10 = 0x01;                             //
  UMCTL0 = 0x00;                            // no modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
  TXBUF0 = 0x55;
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}

#endif

#if 0
//******************************************************************************
//   MSP-FET430P140 Demo - USART0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, USART0 RX ISR at high-speed used
//  with ultra-low power techniques. Normal operation in LPM3, Set_DCO
//  function needs to be called to configure DCO that is used for UART baud
//  generation. On valid RX character, character echoed back. Use start-bit
//  edge detect - URXSE - to automatically (re)enable DCO and trigger ISR. ISR
//  must make sure DCO clock source remains enabled for the UART to receive
//  full character.
//  Software needs to make sure a character has been completely TX'ed, or RX'ed
//  before entering LPM3, which disables DCO required for the USART baud rate
//  generator. In the example, TX'ing is checked using the TXEPT bit directly.
//  RX'ing is checked using the SSEL0 clock select bit as a flag. This is
//  possible because UCLK0 = SMCLK when either both SSEL1 and SSEL0 or just
//  SSEL1 = 1. In the example, when SSEL1 = SSEL0 = 1 there is no RX'ing, and
//  LPM3 is allowed. When SSEL 1 = 1 and  SSEL0 = 0 SMCLK is selected, but
//  RX'ing is active and the DCO is required, thus LPM3 is not allowed.
//  ACLK = LFXT1/8 = 32768/8,  MCLK = SMCLK = UCLK0 = DCOCLK ~ 2MHz
//  Baud rate divider with 2MHz = 2MHz/9600 ~ 208 (00D0h)
//  //* An external 32kHz watch crystal on XIN XOUT is required for ACLK *//	
//
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32768Hz
//          --|RST          XOUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 9600 - 8N1
//            |             P3.5|<------------
//
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

void Set_DCO(void);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= DIVA_3;                        // ACLK= LFXT1CLK/8
  Set_DCO();                                // Set DCO

  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character, SWRST = 1
  UTCTL0 |= SSEL1 + SSEL0 + URXSE;          // UCLK = SMCLK, start edge detect
  UBR00 = 0xD0;                             // 9600 from 2Mhz
  UBR10 = 0x00;
  UMCTL0 = 0x00;                            // No modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
    TXBUF0 = 0x55;
  for (;;)
  {
    while (!(UTCTL0 & TXEPT));              // Confirm no TXing before --> LPM3
    _DINT();                                // Disable interrupts for flag test
    _NOP();
    if (!(UTCTL0 & SSEL0))
       _BIS_SR(LPM0_bits + GIE);            // RX'ing char, enter LPM0, int's active
    else
      _BIS_SR(LPM3_bits + GIE);             // Enter LPM3, int's active
  }
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  if ((IFG1 & URXIFG0))                     // Test URXIFG0
  {
    while (!(IFG1 & UTXIFG0));              // USART0 TX buffer ready?
    TXBUF0 = RXBUF0;                        // RXBUF0 to TXBUF0
    _BIC_SR_IRQ(LPM3_bits);                 // Exit LPM3 after reti
    UTCTL0 |= SSEL0;                        // SSEL0 = 1, no RX activity
  }
  else                                      // Start edge
  {
    UTCTL0 &= ~URXSE;                       // Clear URXS signal
    UTCTL0 |= URXSE;                        // Re-enable edge detect
    _BIC_SR_IRQ(SCG1 + SCG0);               // DCO reamins on after reti
    UTCTL0 &= ~SSEL0;                       // SSEL0= 0, RX activity
  }
}

//------------------------------------------------------------------------------
void Set_DCO (void)                         // Set DCO to selected frequency
//------------------------------------------------------------------------------
{
#define DELTA 488                           // Target DCO = DELTA*(4096) = 2MHz

  unsigned int Compare, Oldcapture = 0;

  CCTL2 = CM_1 + CCIS_1 + CAP;              // CAP, ACLK
  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

  while (1)
  {
    while (!(CCIFG & CCTL2));               // Wait until capture occured
    CCTL2 &= ~CCIFG;                        // Capture occured, clear flag
    Compare = CCR2;                         // Get current captured SMCLK
    Compare = Compare - Oldcapture;         // SMCLK difference
    Oldcapture = CCR2;                      // Save current captured SMCLK
    if (DELTA == Compare) break;            // If equal, leave "while(1)"

    else if (DELTA < Compare)
    {
      DCOCTL--;
      if (DCOCTL == 0xFF)                   // DCO is too fast, slow it down
      {
        if (!(BCSCTL1 == (XT2OFF + DIVA_3)))
        BCSCTL1--;                          // Did DCO role under?, Sel lower RSEL
      }
    }
    else
    {
      DCOCTL++;                             // DCO is too slow, speed it down
      if (DCOCTL == 0x00)
      {
        if (!(BCSCTL1 == (XT2OFF + DIVA_3 + 0x07)))
        BCSCTL1++;                          // Did DCO role over? Sel higher RSEL
      }
    }
  }

  CCTL2 = 0;                                // Stop CCR2
  TACTL = 0;                                // Stop Timer_A
}

#endif

#if 0
//*****************************************************************************
//  MSP-FET430P140 Demo - USART0, Ultra-Low Pwr UART 19200 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, USART0 RX ISR at high-speed used
//  with ultra-low power techniques. Normal operation in LPM3, Set_DCO
//  subroutine needs to be called to configure DCO that is used for UART baud
//  generation. On valid RX character, character echoed back. Use start-bit
//  edge detect - URXSE - to automatically (re)enable DCO and trigger ISR. ISR
//  must make sure DCO clock source remains enabled for the UART to receive
//  full character.
//  Software needs to make sure a character has been completely TX'ed, or RX'ed
//  before entering LPM3, which disables DCO required for the USART baud rate
//  generator. In the example, TX'ing is checked using the TXEPT bit directly.
//  RX'ing is checked using the SSEL0 clock select bit as a flag. This is
//  possible because UCLK0 = SMCLK when either both SSEL1 and SSEL0 or just
//  SSEL1 = 1. In the example, when SSEL1 = SSEL0 = 1 there is no RX'ing, and
//  LPM3 is allowed. When SSEL 1 = 1 and  SSEL0 = 0 SMCLK is selected, but
//  RX'ing is active and the DCO is required, thus LPM3 is not allowed.
//  ACLK = LFXT1/8 = 32768/8,  MCLK = SMCLK = UCLK0 = DCOCLK = 1048576
//  Baud rate divider with 1048576hz= 1048576Hz/19200 ~ 55 (0036h)
//  //* An external 32kHz watch crystal on XIN XOUT is required for ACLK *//	
//
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32768Hz
//          --|RST          XOUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 19200 - 8N1
//            |             P3.5|<------------
//
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//*****************************************************************************

#include  <msp430x14x.h>

void Set_DCO (void);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= DIVA_3;                        // ACLK= LFXT1CLK/8
  Set_DCO();                                // Set DCO

  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character, SWRST = 1
  UTCTL0 |= SSEL1 + SSEL0 + URXSE;          // UCLK = SMCLK, start edge detect
  UBR00 = 0x36;                             // 19200 from 1Mhz
  UBR10 = 0x00;
  UMCTL0 = 0x6B;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
  TXBUF0 = 0x55;
  
  for (;;)
  {
    while (!(UTCTL0 & TXEPT));              // Confirm no TXing before --> LPM3
    _DINT();                                // Disable interrupts for flag test
    _NOP();
    if (!(UTCTL0 & SSEL0))
       _BIS_SR(LPM0_bits + GIE);            // RX'ing char, enter LPM0, int's active
    else
      _BIS_SR(LPM3_bits + GIE);             // Enter LPM3, int's active
  }
}


#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  if ((IFG1 & URXIFG0))                     // Test URXIFG0
  {
    while (!(IFG1 & UTXIFG0));              // USART0 TX buffer ready?
    TXBUF0 = RXBUF0;                        // RXBUF0 to TXBUF0
    _BIC_SR_IRQ(LPM3_bits);                 // Exit LPM3 after reti
    UTCTL0 |= SSEL0;                        // SSEL0 = 1, no RX activity
  }
  else                                      // Start edge
  {
    UTCTL0 &= ~URXSE;                       // Clear URXS signal
    UTCTL0 |= URXSE;                        // Re-enable edge detect
    _BIC_SR_IRQ(SCG1 + SCG0);               // DCO reamins on after reti
    UTCTL0 &= ~SSEL0;                       // SSEL0= 0, RX activity
  }
}

//------------------------------------------------------------------------------
void Set_DCO (void)                         // Set DCO to selected frequency
//------------------------------------------------------------------------------
{
#define DELTA 256                           // Target DCO = DELTA*(4096) = 1048576

  unsigned int Compare, Oldcapture = 0;

  CCTL2 = CM_1 + CCIS_1 + CAP;              // CAP, ACLK
  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

  while (1)
  {
    while (!(CCIFG & CCTL2));               // Wait until capture occured
    CCTL2 &= ~CCIFG;                        // Capture occured, clear flag
    Compare = CCR2;                         // Get current captured SMCLK
    Compare = Compare - Oldcapture;         // SMCLK difference
    Oldcapture = CCR2;                      // Save current captured SMCLK
    if (DELTA == Compare) break;            // If equal, leave "while(1)"

    else if (DELTA < Compare)
    {
      DCOCTL--;
      if (DCOCTL == 0xFF)                   // DCO is too fast, slow it down
      {
        if (!(BCSCTL1 == (XT2OFF + DIVA_3)))
        BCSCTL1--;                          // Did DCO role under?, Sel lower RSEL
      }
    }
    else
    {
      DCOCTL++;                             // DCO is too slow, speed it down
      if (DCOCTL == 0x00)
      {
        if (!(BCSCTL1 == (XT2OFF + DIVA_3 + 0x07)))
        BCSCTL1++;                          // Did DCO role over? Sel higher RSEL
      }
    }
  }

  CCTL2 = 0;                                // Stop CCR2
  TACTL = 0;                                // Stop Timer_A
}
#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 9600 Echo ISR, 32kHz ACLK + DCO
//
//  Description: Echo a received character, USART0 RX ISR at high-speed using
//  32kHz XTAL and DCO. Normal operation in LMP0, DCO continuously stabilized,
//  interrupt driven using CCR2. DCO used for UART baud generation. On valid
//  RX character, character echoed back.
//  ACLK = LFXT1/8 = 32768/8, MCLK = SMCLK = UCLK0 = DCOCLK  = 2MHz
//  Baud rate divider with 2MHz = 2MHz/9600 ~ 208 (00D0h)
//  //* An external 32kHz watch crystal on XIN XOUT is required for ACLK *//	
//
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32768Hz
//          --|RST          XOUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 9600 - 8N1
//            |             P3.5|<------------
//
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

#define DELTA 488                           // target DCO = DELTA*(4096) = 2MHz

unsigned int Compare, Oldcapture;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= DIVA_3;                        // ACLK= LFXT1CLK/8
  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL1;                          // UCLK = SMCLK
  UBR00 = 0xD0;                             // 2MHz 9600
  UBR10 = 0x00;                             //
  UMCTL0 = 0x00;                            // no modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
  TXBUF0 = 0x55;
  
  CCTL2 = CM_1 + CCIS_1 + CAP + CCIE;       // CAP, ACLK, interrupt
  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}


// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A(void)
{
  switch( TAIV )
  {
    case  2: break;                         // CCR1 not used
    case  4:
    {
    Compare = CCR2;                         // Get current captured SMCLK
    Compare = Compare - Oldcapture;         // SMCLK difference
    Oldcapture = CCR2;                      // Save current captured SMCLK
    if (DELTA < Compare)
      {
        DCOCTL--;
        if (DCOCTL == 0xFF)                 // DCO is too fast, slow it down
        {
          if (!(BCSCTL1 == (XT2OFF + DIVA_3)))
          BCSCTL1--;                        // Did DCO role under?, Sel lower RSEL
        }
      }
    else
      {
        DCOCTL++;                           // DCO is too slow, speed it down
        if (DCOCTL == 0x00)
        {
          if (!(BCSCTL1 == (XT2OFF + DIVA_3 + 0x07)))
          BCSCTL1++;                        // Did DCO role over? Sel higher RSEL
        }
      }
    }
  case 10: break;                           // not used
  }
}

#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 19200 Echo ISR, 32kHz ACLK + DCO
//
//  Description: Echo a received character, USART0 RX ISR at high-speed using
//  32kHz XTAL and DCO. Normal operation in LMP0, DCO continuously stabilized,
//  interrupt driven using CCR2. DCO used for UART baud generation. On valid
//  RX character, character echoed back.
//  ACLK = LFXT1/8 = 32768/8,  MCLK = SMCLK = UCLK0 = DCOCLK = 1048576
//  Baud rate divider with 1048576hz= 1048576Hz/19200 ~ 55 (0036h)
//  //* An external 32kHz watch crystal on XIN XOUT is required for ACLK *//	
//
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32768Hz
//          --|RST          XOUT|-
//            |                 |
//            |             P3.4|------------>
//            |                 | 19200 - 8N1
//            |             P3.5|<------------
//
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

#define DELTA 256                           // target DCO = DELTA*(4096) = 1048576

unsigned int Compare, Oldcapture;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= DIVA_3;                        // ACLK= LFXT1CLK/8
  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL1;                          // UCLK = SMCLK
  UBR00 = 0x36;                             // 19200 from 1Mhz
  UBR10 = 0x00;                             //
  UMCTL0 = 0x6B;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
  TXBUF0 = 0x55;
  
  CCTL2 = CM_1 + CCIS_1 + CAP + CCIE;       // CAP, ACLK, interrupt
  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}


// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A(void)
{
  switch( TAIV )
  {
    case  2: break;                         // CCR1 not used
    case  4:
    {
    Compare = CCR2;                         // Get current captured SMCLK
    Compare = Compare - Oldcapture;         // SMCLK difference
    Oldcapture = CCR2;                      // Save current captured SMCLK
    if (DELTA < Compare)
      {
        DCOCTL--;
        if (DCOCTL == 0xFF)                 // DCO is too fast, slow it down
        {
          if (!(BCSCTL1 == (XT2OFF + DIVA_3)))
          BCSCTL1--;                        // Did DCO role under?, Sel lower RSEL
        }
      }
    else
      {
        DCOCTL++;                           // DCO is too slow, speed it down
        if (DCOCTL == 0x00)
        {
          if (!(BCSCTL1 == (XT2OFF + DIVA_3 + 0x07)))
          BCSCTL1++;                        // Did DCO role over? Sel higher RSEL
        }
      }
    }
  case 10: break;                           // not used
  }
}
#endif

#if 0//no resistor...cannot test??
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 9600 Echo ISR, No XTAL, Rosc DCO Only
//
//  Description: Echo a received character, USART0 RX ISR at high-speed using
//  no XTAL and external resistor for DCO stabilized. DCO used for UART baud
//  generation. On valid RX character, character echoed back. Using a 100k
//  resistor on ROSC, with default DCO setting, set DCOCLK ~ 2MHz.
//  ACLK = n/a  MCLK = SMCLK = UCLK0 = DCOCLK  ~ 2MHz
//  Baud rate divider with 2Mhz = 2MHz/9600 ~ 208 (00D0h)
//  //* Use of external resistor reduces temperature sensitivity of DCOCLK *//
//
//               MSP430F149
//            -----------------
//  /|\   /|\|              XIN|-
//   |     | |                 |
//  100k   --|RST          XOUT|-
//   |       |                 |
//   +-------|P2.5/ROSC    P3.4|------------>
//           |                 |  9600 - 8N1
//           |             P3.5|<------------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL2 |= DCOR;                          // Rosc
  _BIS_SR(OSCOFF);                          // XTAL not used
  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL1;                          // UCLK = SMCLK
  UBR00 = 0xD0;                             // 2MHz 9600
  UBR10 = 0x00;                             //
  UMCTL0 = 0x00;                            // no modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
    TXBUF0 = 0x55;
    
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}



#endif

#if 0//cannot test
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, UART 19200 Echo ISR, No XTAL, Rosc DCO Only
//
//  Description: Echo a received character, USART0 RX ISR at high-speed using
//  no XTAL and external resistor for DCO stabilized. DCO used for UART baud
//  generation. On valid RX character, character echoed back. Using a 100k
//  resistor on ROSC, with default DCO setting, set DCOCLK ~ 2MHz.
//  ACLK = n/a  MCLK = SMCLK = UCLK0 = DCOCLK  ~ 2MHz
//  Baud rate divider with 2Mhz = 2MHz/19200 ~ 104 (0068h)
//  //* Use of external resistor reduces temperature sensitivity of DCOCLK *//
//
//               MSP430F149
//            -----------------
//  /|\   /|\|              XIN|-
//   |     | |                 |
//  100k   --|RST          XOUT|-
//   |       |                 |
//   +-------|P2.5/ROSC    P3.4|------------>
//           |                 |  19200 - 8N1
//           |             P3.5|<------------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL2 |= DCOR;                          // Rosc
  _BIS_SR(OSCOFF);                          // XTAL not used
  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL1;                          // UCLK = SMCLK
  UBR00 = 0x68;                             // 2MHz 19200
  UBR10 = 0x00;                             //
  UMCTL0 = 0x00;                            // no modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

__interrupt void usart0_rx (void);
USART0RX_ISR(usart0_rx)
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}

#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM3,
//  USART0 RX interrupt triggers TX Echo.
//  ACLK = UCLK0 = LFXT1 = 32768, MCLK = SMCLK = DCO ~800k
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (0003h 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//
//                MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.4|----------->
//           |                 | 9600 - 8N1
//           |             P3.5|<-----------
//
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
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x03;                             // 32k/9600 - 3.41
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4A;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
    TXBUF0 = 0x55;
  
  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interrupt
}


#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}
#endif


#if 0
//******************************************************************************
//  MSP-FET430x140 Demo - USART0, Ultra-Low Pwr UART 9600 String, 32kHz ACLK
//
//  Description: This program demonstrates a full-duplex 9600-baud UART using
//  USART0 and a 32kHz crystal.  The program will wait in LPM3, and will
//  respond to a received 'u' character using 8N1 protocol. The response will
//  be the string 'Hello World.
//  ACLK = LFXT1 = UCLK0, MCLK = SMCLK = default DCO ~ 800KHz
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (000Dh 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.4|----------->
//           |                 | 9600 - 8N1
//           |             P3.5|<-----------
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

static char string1[] = { "Hello World\r\n" };

char i;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P3SEL = 0x30;                             // P3.3,4 = USART0 TXD/RXD

  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character, SWRST=1
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x03;                             // 9600 from 1Mhz
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4A;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0 + UTXIE0;                   // Enable USART0 RX/TX interrupt
  IFG1 &= ~UTXIFG0;                         // Clear inital flag on POR

  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interrupt
}

// UART0 TX ISR
#pragma vector = USART0TX_VECTOR
__interrupt void usart0_tx (void)
{
  if (i < sizeof string1-1)
    TXBUF0 = string1[i++];
}

// UART0 RX ISR
#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  if (RXBUF0 == 'u')                        // 'u' received?
  {
    i = 0;
    TXBUF0 = string1[i++];
  }


}
#endif

#if 0
//******************************************************************************
//  MSP-FET430x140 Demo - USART0, Ultra-Low Pwr UART 9600 RX/TX, 32kHz ACLK
//
//  Description: This program demonstrates a full-duplex 9600-baud UART using
//  USART0 and a 32kHz crystal.  The program waits in LPM3, receives 8 
//  characters, and then echos back the characters.
//  ACLK = LFXT1 = UCLK0, MCLK = SMCLK = default DCO ~ 800KHz
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (000Dh 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.4|----------->
//           |                 | 9600 - 8N1
//           |             P3.5|<-----------
//
//  M. Buccini / G. Morton
//  Texas Instruments Inc.
//  May 2005
//  Built with Code Composer Essentials Version: 1.0
//******************************************************************************

#include  <msp430x14x.h>

static char string1[8];

char i;
char j = 0;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P3SEL = 0x30;                             // P3.3,4 = USART0 TXD/RXD

  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character, SWRST=1
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x03;                             // 9600 from 1Mhz
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4A;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0 + UTXIE0;                   // Enable USART0 RX/TX interrupt
  IFG1 &= ~UTXIFG0;                         // Clear inital flag on POR

  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interrupt
}

// UART0 TX ISR
#pragma vector = USART0TX_VECTOR
__interrupt void usart0_tx (void)
{
  if (i < sizeof string1)
    TXBUF0 = string1[i++];
}

// UART0 RX ISR
#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  string1[j++] = RXBUF0;
  if (j > sizeof string1-1)
  {
    i = 0;
    j = 0;
    TXBUF0 = string1[i++];
  }
}
#endif

#if 0
//******************************************************************************
//   MSP-FET430P140 Demo - USART0, UART 9600 Full-Duplex Transceiver, 32K ACLK
//
//  Description: UART0 communicates continously as fast as possible full-duplex
//  with another device. Normal mode is LPM3, with activity only during RX and
//  TX ISR's. The TX ISR indicates the UART is ready to send another character.
//  The RX ISR indicates the UART has received a character. At 9600 baud, a full
//  character is tranceived ~1ms.
//  The levels on P1.4/5 are TX'ed. RX'ed value is displayed on P1.0/1.1
//  ACLK = UCLK1 = LFXT1 = 32768, MCLK = SMCLK = DCO~ 800k
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (000Dh 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//                MSP430F149                   MSP430F149
//             -----------------            -----------------
//            |              XIN|-      /|\|              XIN|-
//            |                 | 32KHz  | |                 | 32KHz
//            |             XOUT|-       --|RST          XOUT|-
//            |                 | /|\      |                 |
//            |              RST|---       |                 |
//            |                 |          |                 |
//          ->|P1.4             |          |             P1.0|-> LED
//          ->|P1.5             |          |             P1.1|-> LED
//      LED <-|P1.0             |          |             P1.4|<-
//      LED <-|P1.1             |          |             P1.5|<-
//            |        UTXD/P3.4|--------->|P3.5             |
//            |                 | 9600 8N1 |                 |
//            |        URXD/P3.5|<---------|P3.4             |
//
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
  P1OUT = 0x00;                             // P1.0/1 setup for LED output
  P1DIR = 0x03;
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x03;                             // 32k/9600 - 3.41
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4a;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0 +  UTXIE0;                  // Enable USART0 RX/TX interrupt

  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interrupt
}

#pragma vector = USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  P1OUT = RXBUF0;                           // RXBUF0 to TXBUF0
}

#pragma vector = USART0TX_VECTOR
__interrupt void usart0_Tx (void)
{
  unsigned int i;

  i = P1IN;
  i = i >> 4;
  TXBUF0 = i;                               // Transmit character
}

#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART1, UART 115200 Echo ISR, XT2 HF XTAL SMCLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM0,
//  USART1 RX interrupt triggers TX Echo. Though not required, MCLK= XT2.
//  ACLK = n/a, MCLK = SMCLK = UCLK1 = XT2 = 8MHz
//  Baud rate divider with 8MHz XTAL @115200= 8MHz/115200= 69.44(0045 2Ch)
//  //* An external 8MHz XTAL on XT2IN XT2OUT is required for XT2CLK *//	
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//	
//
//               MSP430F149
//            -----------------
//        /|\|             X2IN|-
//         | |                 | 8MHz
//         --|RST         X2OUT|-
//           |                 |
//           |             P3.6|------------>
//           |                 | 115200 - 8N1
//           |             P3.7|<------------
//
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
  P3SEL |= 0xC0;                            // P3.6,7 = USART1 option select
  BCSCTL1 &= ~XT2OFF;                       // XT2on

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_2 + SELS;                 // MCLK= SMCLK= XT2 (safe)

  ME2 |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD
  UCTL1 |= CHAR;                            // 8-bit character
  UTCTL1 |= SSEL1;                          // UCLK = SMCLK
  UBR01 = 0x45;                             // 8Mhz/115200 - 69.44
  UBR11 = 0x00;                             //
  UMCTL1 = 0x2C;                            // modulation
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  IE2 |= URXIE1;                            // Enable USART1 RX interrupt
  TXBUF1 = 0x55;
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART1RX_VECTOR
__interrupt void usart1_rx (void)
{
  while (!(IFG2 & UTXIFG1));                // USART1 TX buffer ready?
  TXBUF1 = RXBUF1;                          // RXBUF1 to TXBUF1
}
#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART1, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, RX ISR used. In the Mainloop UART1
//  is made ready to receive one character with interrupt active. The Mainloop
//  waits in LPM3. The UART1 ISR forces the Mainloop to exit LPM3 after
//  receiving one character which echo's back the received character.
//  ACLK = UCLK1 = LFXT1 = 32768, MCLK = SMCLK = DCO ~800k
//  Baud rate divider with 32768hz XTAL @2400 = 32768Hz/2400 = 13.65 (000Dh)
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//                MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.6|----------->
//           |                 | 2400 - 8N1
//           |             P3.7|<-----------
//
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
  P3SEL |= 0xC0;                            // P3.6,7 = USART1 option select
  ME2 |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD
  UCTL1 |= CHAR;                            // 8-bit character
  UTCTL1 |= SSEL0;                          // UCLK = ACLK
  UBR01 = 0x0D;                             // 32k/2400 - 13.65
  UBR11 = 0x00;
  UMCTL1 = 0x6B;                            // Modulation
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  IE2 |= URXIE1;                            // Enable USART1 RX interrupt
  TXBUF1 = 0x55;
  
  // Mainloop
  for (;;)
  {
    _BIS_SR(LPM3_bits + GIE);               // Enter LPM3 w/interrupt
    while (!(IFG2 & UTXIFG1));              // USART1 TX buffer ready?
    TXBUF1 = RXBUF1;                        // RXBUF1 to TXBUF1
  }
}

// UART1 RX ISR will for exit from LPM3 in Mainloop
#pragma vector = USART1RX_VECTOR
__interrupt void usart1_rx (void)
{
  _BIC_SR_IRQ(LPM3_bits);                   // Clear LPM3 bits from 0(SR)
}

#endif

#if 0
//******************************************************************************
//  MSP-FET430P140 Demo - USART1, UART 19200 Echo ISR, XT2 HF XTAL SMCLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM0,
//  USART1 RX interrupt triggers TX Echo. Though not required, MCLK= XT2.
//  ACLK =n/a, MCLK = SMCLK = UCLK1 = XT2 = 8MHz
//  Baud rate divider with 8MHz XTAL @19200 = 8MHz/19200 = 416.66 ~ 417 (01A0h)
//  //* An external 8MHz XTAL on X2IN X2OUT is required for XT2CLK *//	
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//	
//
//               MSP430F149
//            -----------------
//        /|\|             X2IN|-
//         | |                 | 8MHz
//         --|RST         X2OUT|-
//           |                 |
//           |             P3.6|------------>
//           |                 | 19200 - 8N1
//           |             P3.7|<------------
//
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
  P3SEL |= 0xC0;                            // P3.6,7 = USART1 option select

  BCSCTL1 &= ~XT2OFF;                       // XT2on

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_2 + SELS;                 // MCLK= SMCLK= XT2 (safe)

  ME2 |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD
  UCTL1 |= CHAR;                            // 8-bit character
  UTCTL1 |= SSEL1;                          // UCLK = SMCLK
  UBR01 = 0xA0;                             // 8Mhz/19200 ~ 417
  UBR11 = 0x01;                             //
  UMCTL1 = 0x00;                            // no modulation
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  IE2 |= URXIE1;                            // Enable USART1 RX interrupt
  TXBUF1 = 0x55;
  
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector = USART1RX_VECTOR
__interrupt void usart1_rx (void)
{
  while (!(IFG2 & UTXIFG1));                // USART1 TX buffer ready?
  TXBUF1 = RXBUF1;                          // RXBUF1 to TXBUF1
}
#endif

#if 1
//******************************************************************************
//  MSP-FET430P140 Demo - USART1, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM3,
//  USART1 RX interrupt triggers TX Echo.
//  ACLK = UCLK1 = LFXT1 = 32768, MCLK = SMCLK = DCO~ 800k
//  Baud rate divider with 32768Hz XTAL @9600 = 32768Hz/9600 = 3.41 (0003h 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//
//                MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.6|----------->
//           |                 | 9600 - 8N1
//           |             P3.7|<-----------
//
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
  P3SEL |= 0xC0;                            // P3.6,7 = USART1 TXD/RXD
  ME2 |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD
  UCTL1 |= CHAR;                            // 8-bit character
  UTCTL1 |= SSEL0;                          // UCLK = ACLK
  UBR01 = 0x03;                             // 32k/9600 - 3.41
  UBR11 = 0x00;                             //
  UMCTL1 = 0x4A;                            // Modulation
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  IE2 |= URXIE1;                            // Enable USART1 RX interrupt
  TXBUF1 = 0x55;
  
  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interrupt
}

#pragma vector = USART1RX_VECTOR
__interrupt void usart1_rx (void)
{
  while (!(IFG2 & UTXIFG1));                // USART1 TX buffer ready?
  TXBUF1 = RXBUF1;                          // RXBUF1 to TXBUF1
}

#endif