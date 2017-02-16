#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

struct Timer {
    uint32_t ticks;
};

void timerReset(struct Timer *timer);
float timerGetElapsedTime(struct Timer *timer);
uint32_t timerGetElapsedTicks(struct Timer *timer);

#endif
