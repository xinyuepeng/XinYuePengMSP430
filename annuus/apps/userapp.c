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
    Hardware_Initialize();

    CreateThread(&tsk[0], thread1);
    CreateThread(&tsk[1], thread2);
    count = MAX_TIME;
    
    _EINT();
    PrepareThread(&tsk[0]);
    PrepareThread(&tsk[1]);
    
    Start_threads();    
    while(1)
    {
    }
}