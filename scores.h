#ifndef SCORES_H
#define SCORES_H

#define NICK_MAX 14
#define SCORES_MAX 14


typedef struct score_t {
    char nick[NICK_MAX+1];
    int  value;
} score_t;


extern  score_t scores[SCORES_MAX];
extern  char    nick[NICK_MAX];
extern  int     f_nickform_needs_redraw;


void scores_init();
int  scores_check(int value);
int  scores_insert(char *nick, int value);
int  scores_ishigh(int val);


#endif
