#include <stdio.h>
#include "game.h"
#include "board.h"
#include "ai.h"


static void drop_piece(board_t *board, piece_t *piece)
{
    while(1) {
        piece->y++;
        if (board_collision(board, piece)) {
            piece->y--;
            board_fix_piece(board, piece);
            break;
        }
    }
    return;
}


static int calc_height(game_t *game)
{
    int x,y,h,end;
    h = 0; end = 0;
    for (y=0; y<BOARD_H; y++) {
        for (x=0; x<BOARD_W; x++) {
            if (game->board.cells[y][x] != 0) {
                end = 1;
            }
        }
        if (end) break;
    }
    h = BOARD_H - y - 1;
    return h;
}


static int eval_vholes(game_t *game, int height)
{
    int x,y, nholes, lh;
    nholes = 0;
    for (x=0; x<BOARD_W; x++) {
        lh = 0;
        for (y=BOARD_H-1; y>=BOARD_H-height-1; y--) {
            if (game->board.cells[y][x]==0) {
                lh++;
            } else {
                if (lh!=0) {
                    nholes +=lh;
                    lh=0;
                }
            }
        }
    }
    return nholes;
}


static float eval_hholes(game_t *game, int height)
{
    int x,y,hole;
    float nholes;
    nholes = 0.0;
    for (y=BOARD_H-1; y>=BOARD_H-height-1; y--) {
        hole = 0;
        for (x=0;x<BOARD_W;x++) {
            if (game->board.cells[y][x] == 0) {
                if (hole == 0) {
                    hole = 1;
                    nholes = nholes + 1.0;
                }
            } else {
                if (hole == 1) {
                    hole = 0;
                }
            }
        }
    }
    return nholes;
}


static float eval(game_t *game)
{
    game_t g;
    int end, score;
    float q, hgt;
    float whh, wvh, wht, wsc;

    g = *game;
    q = 0.0; end = 0; hgt = 0;

    score = g.score;
    drop_piece(&g.board, &g.piece);
    score = score + board_delete_rows(&g.board);

    /* weights */
    wht = 0.00;    /* importance of keeping stack height low */
    wvh = 2.33;    /* importance of avoiding vertical holes */
    whh = 1.00;    /* importance of avoiding horizontal holes */
    wsc = 0.20;    /* importance of achieving high score */

    /* minimize height */
    hgt = calc_height(&g);
    q -= hgt * wht;

    /* minimize number of vertical holes */
    q -= eval_vholes(&g, hgt) * wvh;

    /* minimize number of horizontal holes */
    q -= eval_hholes(&g, hgt) * whh;

    /* maximize score */
    score = (g.score - score) / (g.level + 1);
    switch (score) {
        case 40:   q += 40.0   * wsc; break;
        case 100:  q += 100.0  * wsc; break;
        case 300:  q += 300.0  * wsc; break;
        case 1200: q += 1200.0 * wsc; break;
    }
    return q;
}



int ai_best_move(game_t *game, piece_t *best_piece)
{
    float bestqual;
    float qual;
    int startx;
    int r;
    int rmax;
    int mvdir;
    piece_t piece_sav;

    startx = game->piece.x;
    bestqual = -99999;
    game->quality = 0;
    qual = bestqual;

    rmax = piece_maxframes[game->piece.type];
    piece_sav = game->piece;
    /* for each rotation ... */
    for (r = 0; r < rmax; r++) {
        game->piece.x = startx;
        game->piece.fn = r;
        mvdir = -1;
        /* for each possible horizontal position */
        while (1) {
            if (board_collision(&game->board, &game->piece)) {
                if (mvdir == -1) {
                    mvdir = 1;
                    game->piece.x = startx + 1;
                    continue;
                } else {
                    break;
                }
            }
            /* is it the best move? */
            qual = eval(game);
            if (qual > bestqual) {
                *best_piece = game->piece;
                bestqual = qual;
            }
            /* try another position */
            game->piece.x += mvdir;
        }
    }
    game->piece = piece_sav;
    game->rethink = 0;
    return 0;
}



#ifdef _EXCLUDE_
/* this version takes into account also next piece,
 * but play is poor */
int ai_best_move2(game_t *game, piece_t *best_piece)
{
    float bestqual;
    float qual;
    int startx;
    int r1,r2;
    int rmax1,rmax2;
    int mvdir,mvdir2;
    game_t g;
    piece_t piece_sav;

    startx = game->piece.x;
    bestqual = -99999;
    game->quality = 0;
    qual = bestqual;

    g = *game;
    rmax1 = piece_maxframes[game->piece.type];
    rmax2 = piece_maxframes[game->nextpiece.type];
    /* for each rotation ... */
    for (r1 = 0; r1 < rmax1; r1++) {
        g.piece.x = startx;
        g.piece.fn = r1;
        mvdir = -1;
        while (1) {
            /* restore board state */
            g.board = game->board;
            g.score = game->score;
            if (board_collision(&g.board, &g.piece)) {
                if (mvdir == -1) {
                    mvdir = 1;
                    g.piece.x = startx + 1;
                    continue;
                } else {
                    /* all possible start locations checked! */
                    break;
                }
            }
            qual = eval(&g);
            drop_piece(&g.board, &g.piece);
            g.score += board_delete_rows(&g.board);
            piece_sav = g.piece;
            /* check all possibilities for nextpiece */
            for (r2=0; r2<rmax2; r2++) {
                g.piece = g.nextpiece;
                g.piece.fn = r2;
                g.piece.x = startx;
                mvdir2 = -1;
                while(1) {
                    if (board_collision(&g.board, &g.piece)) {
                        if (mvdir2 == -1) {
                            mvdir2 = 1;
                            g.piece.x = startx + 1;
                            continue;
                        } else {
                            /* all possible start locations checked! */
                            break;
                        }
                    }
                    qual = qual + eval(&g);
                    if (qual > bestqual) {
                        /* save best piece location */
                        *best_piece = piece_sav;
                        bestqual = qual;
                    }
                    /* move nextpiece to next position */
                    g.piece.x += mvdir2;
                }
            }
            /* move piece to next position */
            g.piece = piece_sav;
            g.piece.x += mvdir;
        }
    }
    game->rethink = 0;
    return 0;
}
#endif

