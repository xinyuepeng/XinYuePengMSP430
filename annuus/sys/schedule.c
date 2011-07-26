#include "SysConfig.h"
#include "thread.h"

void ms_sleep(unsigned int ms)
{
    _DINT();
    g_current->timeout = ms;
    g_current->state = THREAD_WAIT;
    Giveup_cpu();
    _EINT();
}