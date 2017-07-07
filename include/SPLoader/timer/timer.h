
#ifndef _SPLOADER_TIMER_TIMER_H
#define _SPLOADER_TIMER_TIMER_H

#include <SPLoader/timer/TimerCallback.h>

int timer_init(void);

int timer_start(TimerCallback callback);

int timer_stop(void);


#endif