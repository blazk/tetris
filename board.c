#include <stdlib.h>
#include <stdio.h>
#include "piece.h"
#include "board.h"


void board_reset(board_t *board)
{
    int x,y;
    for (y=0; y<BOARD_H; y++) {
        for(x=0; x<BOARD_W; x++) {
            board->cells[y][x] = 0;
        }
    }
    return;
}


int board_delete_rows(board_t *board)
{
    int x,y,xx,yy;
    int sum;
    int nlines;
    nlines = 0;
    for (y = 0; y < BOARD_H; y++) {
        sum = 0;
        for (x = 0; x < BOARD_W; x++) {
            if (board->cells[y][x] != 0) {
                sum++;
            }
        }
        if (sum == BOARD_W) {
            for (yy = y; yy > 0; yy--) {
                for (xx = 0; xx < BOARD_W; xx++) {
                    board->cells[yy][xx] = board->cells[yy-1][xx];
                }
            }
            nlines++;
        }
        if (nlines >= 4) break;
    }
    return nlines;
}


int board_collision(board_t *board, piece_t *piece)
{
    int fx,fy;
    int bx,by;
    const frame_t *frame = piece_frame_get(piece);
    for (fy = 0; fy < BLOCK_H; fy++) {
        for (fx = 0; fx < BLOCK_W; fx++) {
            bx = piece->x + fx; 
            by = piece->y + fy;
            if ((*frame)[fy][fx] != 0) {
                if ((bx >= BOARD_W) || (bx < 0))  return 1;
                if ((by >= BOARD_H) || (by < 0))  return 1;
                if (board->cells[by][bx] != 0)    return 1;
            }
        }
    }
    return 0;
}


void board_fix_piece(board_t *board, piece_t *piece)
{
    int fx,fy;
    int bx,by;
    int val;
    const frame_t *frame;
    frame = piece_frame_get(piece);
    for (fx = 0; fx < BLOCK_W; fx++) {
        for (fy = 0; fy < BLOCK_H; fy++) {
            by = piece->y + fy;
            bx = piece->x + fx;
            val = (*frame)[fy][fx];
            if (val != 0) {
                board->cells[by][bx] = piece->type + 1;
            }
        }
    }
    return;
}

