#ifndef THREAD_H
#define THREAD_H

#include "SysConfig.h"

void Start_threads(void);

typedef struct thread_ctb * ptr_thread_ctb;
typedef void (* ptr_thread_entry_t)(void);
typedef struct thread_ctb {
    ptr_thread_ctb next;
    ptr_thread_entry_t thread_entry;
    unsigned short *stackpoint;
    unsigned char stack[STACK_SIZ];    
} thread_ctb_t;

extern thread_ctb_t * g_current;
extern unsigned char Thread_started;
extern int count;

void PrepareThread(ptr_thread_ctb thread);
void CreateThread(ptr_thread_ctb thread_ctb, ptr_thread_entry_t entry);
#endif