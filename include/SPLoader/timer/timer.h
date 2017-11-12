/*
** File: include/SPLoader/timer/timer.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_TIMER_TIMER_H
#define _SPLOADER_TIMER_TIMER_H

#include <SPLoader/timer/TimerCallback.h>

#ifndef __ASM__

int spl_timer_init(void);

int spl_timer_start(TimerCallback callback);

int spl_timer_stop(void);

#endif  // __ASM__

#endif