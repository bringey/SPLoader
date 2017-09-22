/*
** File: include/SPLoader/timer/timer.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_TIMER_TIMER_H
#define _SPLOADER_TIMER_TIMER_H

#error "DEPRECATED! do not use"

#include <SPLoader/timer/TimerCallback.h>

#ifndef __ASM__

int timer_init(void);

int timer_start(TimerCallback callback);

int timer_stop(void);

#endif  // __ASM__

#endif