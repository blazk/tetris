#include <string.h>
#include "scores.h"


score_t scores[SCORES_MAX];
char nick[NICK_MAX];
int f_nickform_needs_redraw;

void scores_init()
{
    int i;
    for (i = 0; i < SCORES_MAX; i++) {
        strncpy(scores[i].nick, "EMPTY    ", NICK_MAX);
        scores[i].nick[NICK_MAX+1] = '\0';
        scores[i].value = 0;
    }
    return;
}


int scores_ishigh(int value)
{
    int i;
    for (i = 0; i < SCORES_MAX; i++) {
        if (scores[i].value < value)
            return 1;
    }
    return 0;
}


int scores_insert(char *nick, int value)
{
    int i,j;
    for (i = 0; i < SCORES_MAX; i++) {
        if (scores[i].value < value) {
            /* move all smaller scores down the list */
            for (j=i; j < SCORES_MAX-1; j++) {
                strncpy(scores[j+1].nick, scores[j].nick, NICK_MAX);
                scores[j+1].value = scores[j].value;
            }
            strncpy(scores[i].nick, nick, NICK_MAX);
            scores[i].value = value;
            return 0;
        }
    }
    return -1;
}
