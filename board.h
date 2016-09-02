#ifndef _BOARD_H_
#define _BOARD_H_

#include "piece.h"

#define BOARD_W 10
#define BOARD_H 20

typedef struct board_t {
    int cells[BOARD_H][BOARD_W];
} board_t;


void board_reset(board_t *board);
int  board_delete_rows(board_t *board);
int  board_collision(board_t *board, piece_t *piece);
void board_fix_piece(board_t *board, piece_t *piece);

#endif
