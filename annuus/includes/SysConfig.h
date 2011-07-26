#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include "msp430x14x.h"

#define MAX_TIME    1

#ifndef NULL
#define NULL    0
#endif

#define THREAD_READY            0
#define THREAD_WAIT             1
#define THREAD_RUNNING          2

#define STACK_SIZ               64
#define STACK_OFFSET            (STACK_SIZ-2)
#define THREAD_STATE_OFFSET     (2*3+STACK_SIZ)
#define THREAD_TIMEOUT_OFFSET   (THREAD_STATE_OFFSET + 2);

#define PRIVILEDGE_LEVEL    16

#endif