#include <stdlib.h>
#include <stdio.h>
#include "piece.h"
#include "game.h"
#include "ai.h"
#include "timing.h"


gtimer_t falltimer;


/******************
 * misc.functions *
 ******************/


static void update_score(game_t *game, int nlines)
{
    int award = 0;
    switch (nlines) {
        case 1: award = 40; break;
        case 2: award = 100; break;
        case 3: award = 300; break;
        case 4: award = 1200; break;
    }
    award = award * (game->level + 1);
    game->score += award;
    game->redraw = 1;
    return;
}


static void prepare_next_piece(game_t *game)
{
    piece_rand(&(game->nextpiece));
    game->nextpiece.x = BOARD_W/2 - BLOCK_W/2;
    game->nextpiece.y = 0;
    return;
}


static int spawn_piece(game_t *game)
{
    game->piece = game->nextpiece;
    if (board_collision(&game->board, &game->piece)) {
        return -1;
    }
    prepare_next_piece(game);
    gtimer_set(&falltimer, 1000);
    return 0;
}


/*************************************
 * game functions (public interface) *
 *************************************/


int game_reset(game_t *game)
{
    board_reset(&game->board);
    game->score    = 0;
    game->level    = 0;
    game->redraw   = 1;
    game->gameover = 0;
    game->quality  = 0;
    game->ai       = 0;
    game->rethink  = 1;
    game->iter     = 0;
    game->itag     = 0;
    game->ftag     = 0.0;
    prepare_next_piece(game);
    spawn_piece(game);
    game->bestpiece=game->piece;
    return 0;
}



int game_update(game_t *game, int event)
{
    piece_t oldpiece = game->piece;
    int mvdown = 0;
    int nrows;
    if (game->gameover) {
        return 0;
    }
    switch (event) {
        case EVENT_ROTL:
            piece_frame_prev(&(game->piece));
            break;
        case EVENT_ROTR:
            piece_frame_next(&(game->piece));
            break;
        case EVENT_LEFT:
            game->piece.x--;
            break;
        case EVENT_RIGHT:
            game->piece.x++;
            break;
        case EVENT_DOWN:
            game->piece.y++;
            mvdown = 1;
            break;
    }
    if (board_collision(&game->board, &game->piece)) {
        game->piece = oldpiece;
        if (mvdown == 1) {
            if (game->piece.y == 0) {
                game->gameover = 1;
                return 0;
            }
            board_fix_piece(&game->board, &game->piece);
            nrows = board_delete_rows(&game->board);
            if (nrows != 0) {
                update_score(game, nrows);
            }
            game->rethink = 1;
            game->redraw = 1;
            if (spawn_piece(game) != 0) {
                game->gameover = 1;
                return 0;
            }
        }
    } else {
        game->redraw = 1;
    }
    return 0;
}

