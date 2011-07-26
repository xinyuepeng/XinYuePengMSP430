#include "SysConfig.h"
#include "thread.h"

int count;
unsigned char Thread_started = 0;

static thread_ctb_t idle_tsk;

void system_idle(void)
{
    while(1)
    {
        _NOP();
    }
}

void SystemInit(void)
{
    CreateThread(&idle_tsk, system_idle);
}

void SystemReady(void)
{
    PrepareThread(&idle_tsk);
}