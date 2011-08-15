#ifndef SYSTEM_H
#define SYSTEM_H

#include "SysConfig.h"

inline void SystemInit(void);
inline void SystemReady(void);
unsigned int interrupt_disable(void);
void interrupt_enable(unsigned int cpustat);
#endif