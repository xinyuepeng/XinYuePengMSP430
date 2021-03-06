#include "msp430x14x.h"
#include "SysConfig.h"

    EXTERN g_current            ;
    EXTERN count                ;
    EXTERN Thread_started       ;
    EXTERN Context_Switch       ;
    EXTERN check_threadevents   ;
;    PUBLIC wdt_isr
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
    call    #check_threadevents
    TST.B   Thread_started
    jz     just_exit
    DEC     count
    jnz     just_exit
    mov     #MAX_TIME, count
    jmp     Context_Switch
just_exit    
    reti

timerb_isr
    PUSH    R14
    //XOR.B   #0x01, &P4OUT
    ADD     &TBIV, R14
    POP     R14
    call    #check_threadevents
    TST.B   Thread_started
    jz     just_exit
    DEC     count
    jnz     just_exitb
    mov     #MAX_TIME, count
    jmp     Context_Switch
just_exitb    
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