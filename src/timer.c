//
// Created by oholly on 14. 2. 2020.
//

#include "../include/timer.h"

static uint32_t getSeconds()
{
    return (uint32_t)(clock()/CLOCKS_PER_SEC);
}


void timer_start(timerArgs_t *timer, uint32_t period)
{
    timer->s_period = period;
    timer->s_lastTrigger = getSeconds();
    timer->state = 1;
}

void timer_stop(timerArgs_t *timer)
{
    timer->state = 0;
}

uint8_t timer_run(timerArgs_t *timer)
{
    if(timer->state && getSeconds() > (timer->s_lastTrigger + timer->s_period)) {
        timer->s_lastTrigger = getSeconds();
        return 1;
    }

    return 0;
}