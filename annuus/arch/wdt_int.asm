#include "msp430x14x.h"
#include "SysConfig.h"

    EXTERN g_current  ;
    EXTERN count      ;
    EXTERN Thread_started;
    EXTERN Context_Switch;
    PUBLIC  wdt_isr
    RSEG    CODE
wdt_isr
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
    
    END