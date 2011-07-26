#include "thread.h"

thread_ctb_t * g_header = NULL;
thread_ctb_t * tailer = NULL;
thread_ctb_t * g_current;

void check_threadevents(void)
{
    thread_ctb_t *tmp = g_current;
    do {
        tmp = tmp->next;
        if(tmp->timeout > 0) {
            tmp->timeout = tmp->timeout > 5 ? (tmp->timeout-5) : 0;
            if(!tmp->timeout)
              tmp->state = THREAD_READY;
        }   
    } while(tmp != g_current);
}

ptr_thread_ctb do_schedule(void)
{
    thread_ctb_t *tmp = g_current;
    do {
        tmp = tmp->next;
        if(tmp->state == THREAD_READY)
            break;
    } while(tmp != g_current);
    
    return tmp;
}

void CreateThread(ptr_thread_ctb thread_ctb, ptr_thread_entry_t entry)
{    
    if(!thread_ctb)
        return;
    
    thread_ctb->thread_entry = entry;
    if(NULL == g_header || NULL == tailer)
    {
        g_header = tailer = thread_ctb;
        thread_ctb->next = g_header;
        g_current = g_header;
    }
    else
    {
        thread_ctb->next = g_header;
        tailer->next = thread_ctb;
        tailer = thread_ctb;
    }
}