#ifndef _GAME_H
#define _GAME_H

#include "board.h"
#include "piece.h"
#include "timing.h"

#define EVENT_ROTL  1
#define EVENT_ROTR  2
#define EVENT_LEFT  3
#define EVENT_RIGHT 4
#define EVENT_DOWN  5


typedef struct game_t {
    board_t board;
    piece_t piece;
    piece_t nextpiece;
    int level;
    int score;
    int redraw;
    int gameover;
    /* AI mode */
    int ai;
    int rethink;
    float quality;
    int iter;
    piece_t bestpiece;
    /* debug values */
    int itag;
    float ftag;
} game_t;

extern gtimer_t falltimer;


int  game_reset(game_t *game);
int  game_update(game_t *game, int event);

#endif
