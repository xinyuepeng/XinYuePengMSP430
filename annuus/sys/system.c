#include "SysConfig.h"
#include "thread.h"

int count;
unsigned char Thread_started = 0;
thread_ctb_t idle_tsk;

void system_idle(void)
{
    while(1)
    {
        _NOP();
    }
}


void SystemInit(void)
{    
#ifdef DBG_IDLE_SINGLE
    idle_tsk.thread_entry = system_idle;
#else
    CreateThread(&idle_tsk, system_idle);
#endif
}

void SystemReady(void)
{
    PrepareThread(&idle_tsk);
}

