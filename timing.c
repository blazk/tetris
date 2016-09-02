#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <curses.h>
#include "timing.h"


void gtimer_restart(gtimer_t *gtimer) {
    static struct timeval tv;
    static struct timezone tz;
    gettimeofday(&tv, &tz);
    gtimer->t = (time_t) ((tv.tv_sec * 1000 + tv.tv_usec / 1000) + gtimer->delay);
    return;
}


void gtimer_set(gtimer_t *gtimer, time_t delay)
 {
    gtimer->delay = delay;
    gtimer_restart(gtimer);
    return;
}


int gtimer_check(gtimer_t *gtimer)
{
    static struct timeval tv;
    static struct timezone tz;
    gettimeofday(&tv, &tz);

    if ((time_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000) > gtimer->t) {
        return 1;
    } else {
        return 0;
    }
}


void gsleep()
{
    napms(7);
    return;
}
