#ifndef TIMING_H
#define TIMING_H

#include <time.h>

typedef struct gtimer_t {
    time_t delay;    /* miliseconds */
    time_t t;
} gtimer_t;

void gsleep();  /* pause 0.01 s */
void gtimer_set(gtimer_t *gtimer, time_t delay);
void gtimer_restart(gtimer_t *gtimer);
int  gtimer_check(gtimer_t *gtimer);


#endif
