/*
** File: include/SPLoader/timer/TimerCallback.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_TIMER_TIMERCALLBACK_H
#define _SPLOADER_TIMER_TIMERCALLBACK_H

#ifndef __ASM__

typedef void (*TimerCallback)(unsigned tickCount);

#endif  // __ASM__

#endif