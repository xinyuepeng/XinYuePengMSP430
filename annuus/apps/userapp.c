#include "SysConfig.h"
#include "Hardware.h"

#include "SysConfig.h"
#include "Hardware.h"
#include "thread.h"
#include "System.h"
void Alarm_BuzzorOff(void);
void Alarm_BuzzorOn(void);


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
    unsigned int status;
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
        status = interrupt_disable();
        flag = ~flag;
        interrupt_enable(status);
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
#if 0  
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
#else
    unsigned char temp = 0;
    while(1)
    {  
        if(temp == 0) {
            TACCR0 = 0x0C;      //10ms

            CCTL1 = OUTMOD_7;
            CCR1 = 0x05;

            CCTL2 = OUTMOD_7;
            CCR2 = 0x06;    
            TACTL = TASSEL_1 + MC_1;
            temp = 1;
        }
        else
        {
            TACCR0 = 0x9;      //10ms

            CCTL1 = OUTMOD_7;
            CCR1 = 0x04;

            CCTL2 = OUTMOD_7;
            CCR2 = 0x05;    
            TACTL = TASSEL_1 + MC_1;
            temp = 0;
        }
        ms_sleep(200);
        TACTL = TASSEL_1 + MC_0;
        ms_sleep(120);
    }
    
    while(1)
    {
      Alarm_BuzzorOn();
      ms_sleep(5000);
      Alarm_BuzzorOff();
      ms_sleep(5000);
    }
#endif    
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