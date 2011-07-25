#include "msp430x14x.h"
#include "SysConfig.h"

    EXTERN g_current  ;
    EXTERN count      ;
    EXTERN Thread_started;
    EXTERN Context_Switch;
;    PUBLIC wdt_isr
    PUBLIC timera_isr
    RSEG    CODE
wdt_isr
;    TST.B   Thread_started
;    jz     just_exit
;    DEC     count
;    jnz     just_exit
;    mov     #MAX_TIME, count
;    jmp     Context_Switch
;just_exit    
    reti

timera_isr
    PUSH    R14
    //XOR.B   #0x01, &P4OUT
    ADD     &TAIV, R14
    POP     R14
    TST.B   Thread_started
    jz     just_exit
    DEC     count
    jnz     just_exit
    mov     #MAX_TIME, count
    jmp     Context_Switch
just_exit    
    reti

;============================================================
    COMMON  INTVEC(1)           ; Interrupt vectors
;============================================================
    ORG     WDT_VECTOR
    DW      wdt_isr
    
    ORG     TIMERA1_VECTOR
    DW      timera_isr
    END