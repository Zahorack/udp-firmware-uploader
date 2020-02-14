//
// Created by oholly on 14. 2. 2020.
//

#ifndef UDP_FIRMWARE_UPLOADER_TIMER_H
#define UDP_FIRMWARE_UPLOADER_TIMER_H

#include <time.h>
#include <stdint.h>

typedef struct {
    uint32_t s_period;
    uint32_t s_lastTrigger;

    uint8_t state;
}timerArgs_t;


void timer_start(timerArgs_t *timer, uint32_t period);
void timer_stop(timerArgs_t *timer);
uint8_t timer_run(timerArgs_t *timer);


#endif //UDP_FIRMWARE_UPLOADER_TIMER_H