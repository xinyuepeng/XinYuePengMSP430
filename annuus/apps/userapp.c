#include "SysConfig.h"
#include "Hardware.h"

#include "SysConfig.h"
#include "Hardware.h"
#include "thread.h"
#include "System.h"

unsigned char freq = 0xA0;

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
#if 1     
        if(flag > 0)
          P4OUT |= 0x01;
        else
          P4OUT &= 0xFE;
#endif        
        //delay(10);
        ms_sleep(500);
        flag = ~flag;
    }
}

void thread2(void)
{
    unsigned char flag = 0;
      
    while(1)
    {
#if 1     
        if(flag > 0)
          P4OUT |= 0x02;
        else
          P4OUT &= 0xFD;
#endif        
        //delay(20);
        ms_sleep(100);
        flag = ~flag;
    }
}

void keythread(void)
{
    unsigned char temp;
    P1DIR &= ~0x60;
    P1SEL &= ~0x60;
    
    while(1)
    {
        temp = P1IN & 0x60;
        if(temp == 0x40)
        {
            if(freq > 0xa0)
              freq = 10;
            else
                freq += 30;
            CCR1 = freq;
        }
        else if(temp == 0x20)
        {
            if(freq < 30)
              freq = 0xA0;
            else    
            freq -= 30;
            CCR1 = freq;
        }
        ms_sleep(50);
    }
}


thread_ctb_t tsk[3] =
{
  0
};
extern thread_ctb_t idle_tsk;
void main(void)
{
    Hardware_Initialize();
    SystemInit();
    
    CreateThread(&tsk[0], thread1);
    CreateThread(&tsk[1], thread2);
    CreateThread(&tsk[2], keythread);
    count = MAX_TIME;
    
    _EINT();
    SystemReady();
    PrepareThread(&tsk[0]);
    PrepareThread(&tsk[1]);
    PrepareThread(&tsk[2]);
    
    Start_threads();    
    while(1)
    {
    }
    //Cannot get here.
}