#include "msp430x14x.h"

    PUBLIC timera_isr
    PUBLIC timerb_isr
    RSEG    CODE
;wdt_isr
;    XOR.B   #0x10, &P1OUT
;    TST.B   Thread_started
;    jz     just_exit
;    DEC     count
;    jnz     just_exit
;    mov     #MAX_TIME, count
;    jmp     Context_Switch
;just_exit    
;    reti

timera_isr
    PUSH    R14
    //XOR.B   #0x01, &P4OUT
    ADD     &TAIV, R14
    POP     R14
    reti

timerb_isr
    PUSH    R14
    XOR.B   #0x01, &P4OUT
    ADD     &TBIV, R14
    POP     R14
    reti

;============================================================
    COMMON  INTVEC(1)           ; Interrupt vectors
;============================================================
;    ORG     WDT_VECTOR
;    DW      wdt_isr
    
    ORG     TIMERA1_VECTOR
    DW      timera_isr
    
    ORG     TIMERB1_VECTOR
    DW      timerb_isr
    END