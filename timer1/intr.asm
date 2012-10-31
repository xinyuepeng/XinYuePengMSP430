#include "msp430x14x.h"

    EXTERN TIMOVH_C
    EXTERN TIMMOD1_C
    PUBLIC timera_isr
    RSEG    CODE
timera_isr
    push    R14
    xor.b   #0x1, &P4OUT
    ADD     &TAIV, R14
    pop     R14
    reti            ; VECTOR 0: NO interrupt
    JMP     TIMMOD1_C   ; Vector 2: module1
    JMP     TIMMOD2     ; Vector 4: module2
    JMP     TIMMOD3     ; Vector 6: module3
    JMP     TIMMOD4     ; Vector 8: module4
TIMOVH
    JMP     TIMOVH_C    ; Vector 10

TIMMOD1
    BR  #TIMMOD1_C
TIMMOD2
TIMMOD3
TIMMOD4
    RETI

;============================================================
    COMMON  INTVEC(1)           ; Interrupt vectors
;============================================================
    ORG     TIMERA1_VECTOR
    DW      timera_isr
    END