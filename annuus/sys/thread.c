#include "thread.h"

thread_ctb_t * g_header = NULL;
thread_ctb_t * tailer = NULL;
thread_ctb_t * g_current;

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