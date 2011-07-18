#include "SysConfig.h"
#include "Hardware.h"

#include "SysConfig.h"
#include "Hardware.h"
#include "thread.h"

void delay(int num)
{
    int i;
    for(;num > 0; num--)
      for(i = 0; i < 1024; i++);
}

void thread1(void)
{
    unsigned char flag = 0;
    
    while(1)
    {
        if(flag > 0)
          P4OUT |= 0x01;
        else
          P4OUT &= 0xFE;
        
        delay(10);
        flag = ~flag;
    }
}

void thread2(void)
{
    unsigned char flag = 0;
    
    while(1)
    {
        if(flag > 0)
          P4OUT |= 0x02;
        else
          P4OUT &= 0xFD;
        
        delay(20);
        flag = ~flag;
    }
}

thread_ctb_t tsk[2] =
{
  0
};

void main(void)
{
    IFG1 = 0;
    WDTCTL=WDT_MDLY_32;         //WDT 32ms Internal Timer
    P4DIR = 0x03;
    
    IFG1 &= ~WDTIFG;            //Clear pending WDT interrupts
    IE1 |= WDTIE;               //Enable WDT interrupt
    
    
    tsk[0].next = &tsk[1];
    tsk[0].thread_entry = thread1;
    tsk[1].next = &tsk[0];
    tsk[1].thread_entry = thread2;
    g_current = &tsk[0];
    count = MAX_TIME;
    _EINT();
    PrepareThread(&tsk[0]);
    PrepareThread(&tsk[1]);
    Start_threads();
    while(1)
    {
    }
}