    #include "msp430x14x.h"
#include "SysConfig.h"    

NAME    Threads
    
    PUBLIC  Start_threads
    PUBLIC  Context_Switch
    PUBLIC  PrepareThread

    EXTERN  g_current       ;
    EXTERN  Thread_started;
    
    RSEG    CODE
Start_threads               ;
    DINT
    mov     g_current,        R13  ;
    mov     0004h(R13),       SP
    pop     R15
    pop     R14
    pop     R13
    pop     R12
    pop     R11
    pop     R10
    pop     R9
    pop     R8
    pop     R7
    pop     R6
    pop     R5
    pop     R4
    INC.B   Thread_started     
    EINT
    reti                             ; never get here

PrepareThread               ;
    push    R13
    push    R14
    mov     R12,              R13
    add     #0006h,           R13
    add     #STACK_OFFSET,    R13
    mov     0002h(R12),       0(R13)  ;PC
    mov     SR,               -2(R13) ;Stack
    mov     #0000h,           -4(R13) ;R4
    mov     #0000h,           -6(R13) ;R5
    mov     #0000h,           -8(R13) ;R6
    mov     #0000h,           -10(R13);R7
    mov     #0000h,           -12(R13);R8
    mov     #0000h,           -14(R13);R9
    mov     #0000h,           -16(R13);R10
    mov     #0000h,           -18(R13);R11
    mov     #0000h,           -20(R13);R12
    mov     #0000h,           -22(R13);R13
    mov     #0000h,           -24(R13);R14
    mov     #00F0h,           -26(R13);R15
    sub     #26,               R13
    mov     R13,              0004(R12);SP
    pop     R14
    pop     R13
    ret


Context_Switch              ;
    push    R4
    push    R5
    push    R6
    push    R7
    push    R8
    push    R9
    push    R10
    push    R11
    push    R12
    push    R13
    push    R14
    push    R15
    DINT
    mov     g_current, R14
    mov     sp,     0004h(R14)
    mov     0(R14), g_current
    mov     g_current, R14
    mov     0004h(R14), sp
    pop     R15
    pop     R14
    pop     R13
    pop     R12
    pop     R11
    pop     R10
    pop     R9
    pop     R8
    pop     R7
    pop     R6
    pop     R5
    pop     R4
    reti

    END