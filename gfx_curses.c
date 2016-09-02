#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "gfx.h"
#include "game.h"
#include "piece.h"
#include "scores.h"
#include "timing.h"


/*
 * windows
 */

WINDOW *wboard;
WINDOW *wstats;

/*
 * windows sizes
 */

#define GBOARD_W  (BOARD_W * 2 + 2)
#define GBOARD_H  (BOARD_H + 2)
#define STATS_W  10
#define STATS_H  (GBOARD_W)

/*
 * box styles
 */

#define GFX_PITBG   '.'
#define GFX_BLOCK   ' '
#define GFX_STBLK   ' '
#define GFX_BGCOL   COLOR_WHITE


static gtimer_t animtimer;


static void putbox(WINDOW *w, int y, int x, int style)
{
    mvwaddch(w, y+1, 2*x+1, style);
    mvwaddch(w, y+1, 2*x+2, style);
    return;
}

int gfx_init()
{
    int sx,sy;

    if (initscr() == NULL) {
        fprintf(stderr, "gfx_init(): error initializing curses\n");
        return -1;
    }
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "gfx_init(): your terminal doens't support colors\n");
        return -1;
    }
    cbreak(); noecho();
    nodelay(stdscr,TRUE);
    curs_set(0);
    start_color();

    init_pair(0,COLOR_WHITE,COLOR_RED);
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_YELLOW);
    init_pair(3,COLOR_WHITE,COLOR_BLUE);
    init_pair(4,COLOR_WHITE,COLOR_MAGENTA);
    init_pair(5,COLOR_WHITE,COLOR_CYAN);
    init_pair(6,COLOR_WHITE,COLOR_WHITE);
    init_pair(7,COLOR_WHITE,COLOR_RED);
    init_pair(8,COLOR_BLACK,COLOR_BLACK);

    init_pair(10,COLOR_GREEN,COLOR_GREEN);

    /* create windows */
    sx = COLS/2 - (GBOARD_W+STATS_W)/2 + 1;
    sy = LINES*0.4 - GBOARD_H/2 + 1;
    wboard = newwin(GBOARD_H, GBOARD_W, sy, sx);
    sx = sx + GBOARD_W;

    wstats = newwin(STATS_H, STATS_W, sy, sx);

    refresh();

    /* set animation timer */
    gtimer_set(&animtimer, 180);
    return 0;
}


int gfx_clear()
{
    erase();
    refresh();
    return 0;
}


int gfx_redraw_menu()
{
    static char neon[] = "TETRIS TETRIS ";
    static unsigned int phase = 0;
    int i;
    mvwaddch (wboard, 8, 8, 'N' | A_UNDERLINE);
    mvwprintw(wboard, 8, 9, "%s", "EW GAME");
    mvwaddch (wboard, 9, 9, 'S' | A_UNDERLINE);
    mvwprintw(wboard, 9,10, "%s", "CORES");
    mvwaddch (wboard,10,10, 'Q' | A_UNDERLINE);
    mvwprintw(wboard,10,11, "%s", "UIT");
    if (gtimer_check(&animtimer)) {
        for (i = 0; i<14; i++) {
            mvwaddch(wboard,6,5+i,neon[(i+phase)%14]);
        }
        gtimer_restart(&animtimer);
        phase = (phase+1);
    }
    wrefresh(wboard);
    return 0;
}


int gfx_redraw(game_t *game)
{
    int x,y;
    int bx,by;
    int val;
    const frame_t *frame;

    wbkgdset(wstats,' ' | COLOR_PAIR(10));
    for (y=0;y<STATS_H;y++) for(x=0;x<STATS_W;x++) {
        putbox(wstats,y,x,' ');
    }
    wbkgdset(wstats,' ' | COLOR_PAIR(3));

    /* draw level / score */
    mvwprintw(wstats, 1, 1,"%s",   "TOP   "    );
    mvwprintw(wstats, 2, 1,"%06d",   0         );
    mvwprintw(wstats, 4, 1,"%s",   "SCORE "    );
    mvwprintw(wstats, 5, 1,"%06d",  game->score);
    mvwprintw(wstats, 16,1,"%s",   "LEVEL "    );
    mvwprintw(wstats, 17,1,"%02d",  game->level);
    /*
    mvwprintw(wstats, 17,2,"%s",   "QUALITY"   );
    mvwprintw(wstats, 18,2,"%8.3f", game->quality);
    mvwprintw(wstats, 19,2,"%s",   "FTAG"   );
    mvwprintw(wstats, 20,2,"%8.3f", game->ftag);
    */

    /* draw next piece */
    mvwprintw(wstats, 8, 1,"%s",  "NEXT   ");
    frame = piece_frame_get(&(game->nextpiece));
    for (y = -1; y < BLOCK_H; y++) {
        for (x = 0; x < BLOCK_W; x++) {
            if (y>=0 && y<BLOCK_H && x>=0 && x<BLOCK_W) {
                val = (*frame)[y][x];
            } else {
                val = 0;
            }
            if (val != 0) {
                putbox(wstats, y+9,x,GFX_BLOCK | COLOR_PAIR(game->nextpiece.type+1));
            } else {
                putbox(wstats, y+9,x,' ' | COLOR_PAIR(8));
            }
        }
    }
    wbkgdset(wstats,' ' | COLOR_PAIR(10));
    box(wstats,0,0);
    wbkgdset(wstats,' ' | COLOR_PAIR(3));
    wrefresh(wstats);

   /* draw board */

    for (y = 0; y < BOARD_H; y++) {
        for (x = 0; x < BOARD_W; x++) {
            if (game->board.cells[y][x] == 0) {
                putbox(wboard,y,x,GFX_PITBG | COLOR_PAIR(8));
            } else {
                putbox(wboard,y,x,GFX_STBLK |
                        COLOR_PAIR(game->board.cells[y][x]));
            }
        }
    }
    /* draw piece */
    frame = piece_frame_get(&(game->piece));
    for (y = 0; y < BLOCK_H; y++) {
        for (x = 0; x < BLOCK_W; x++) {
            bx = x + game->piece.x;
            by = y + game->piece.y;
            val = (*frame)[y][x];
            if (val != 0) {
                putbox(wboard,by,bx,GFX_BLOCK | COLOR_PAIR(game->piece.type+1));
            } else {
               if ((bx<BOARD_W) && (bx>= 0) && 
                   (by<BOARD_H) && (by>=0)) {
                   if (game->board.cells[by][bx] == 0) {
                       putbox(wboard,by,bx,GFX_PITBG | COLOR_PAIR(8));
                   }
               }
            }
        }
    }
    wbkgdset(wboard,' ' | COLOR_PAIR(10));
    box(wboard,0,0);
    wbkgdset(wboard,' ' | COLOR_PAIR(3));
    wrefresh(wboard);
    return 0;
}


int gfx_redraw_scores()
{
    int i;
    mvwaddstr(wboard,1,6,"HIGH SCORES");
    mvwaddstr(wboard,3,6,"   -=*=-   ");
    for (i = 0; i < SCORES_MAX; i++) {
        mvwprintw(wboard,i+4,4,"%8s%06d", scores[i].nick, scores[i].value);
    }
    wrefresh(wboard);
    return 0;
}


int gfx_redraw_gameover()
{
    mvwaddstr(wboard,7,5, "             ");
    mvwaddstr(wboard,8,5, "  GAME OVER  ");
    mvwaddstr(wboard,9,5, "             ");
    mvwaddstr(wboard,10,5," press space ");
    mvwaddstr(wboard,11,5," to restart, ");
    mvwaddstr(wboard,12,5," 'q' to quit ");
    mvwaddstr(wboard,13,5,"             ");
    wrefresh(wboard);
    return 0;
}


int gfx_redraw_nickform()
{
    mvwaddstr(wboard,10,4,"YOUR NAME:");
    mvwprintw(wboard,11,4,"%s", nick);
    wrefresh(wboard);
    return 0;
}


int gfx_done()
{
    endwin();
    return 0;
}
