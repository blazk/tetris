#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "game.h"
#include "piece.h"
#include "scores.h"
#include "timing.h"


static gtimer_t animtimer;


static float boxcolors[][3] = {
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0},
    {1.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 1.0, 1.0},
    {0.5, 0.0, 1.0}
};


static void putbox(int y, int x, int style)
{
    glPushMatrix();
    glTranslatef(x*1.0, 19-y*1.0, 0);
    glBegin(GL_TRIANGLES);
    /* front */
    glColor3f (boxcolors[style][0]*1.0,
               boxcolors[style][1]*1.0,
               boxcolors[style][2]*1.0);
    glVertex3f(0.00f, 0.00f, 1.00f);
    glColor3f (boxcolors[style][0]*0.8,
               boxcolors[style][1]*0.8,
               boxcolors[style][2]*0.8);
    glVertex3f(1.00f, 0.00f, 1.00f);
    glVertex3f(0.00f, 1.00f, 1.00f);
    glVertex3f(0.00f, 1.00f, 1.00f);
    glVertex3f(1.00f, 0.00f, 1.00f);
    glColor3f (boxcolors[style][0]*0.65,
               boxcolors[style][1]*0.65,
               boxcolors[style][2]*0.65);
    glVertex3f(1.00f, 1.00f, 1.00f);
    /* back */
    glColor3f (boxcolors[style][0]*0.2,
               boxcolors[style][1]*0.2,
               boxcolors[style][2]*0.2);
    glVertex3f(0.00f, 0.00f, 0.00f);
    glVertex3f(1.00f, 0.00f, 0.00f);
    glVertex3f(0.00f, 1.00f, 0.00f);
    glVertex3f(0.00f, 1.00f, 0.00f);
    glVertex3f(1.00f, 1.00f, 0.00f);
    glVertex3f(1.00f, 0.00f, 0.00f);
    /* bottom */
    glColor3f (boxcolors[style][0]*0.5,
               boxcolors[style][1]*0.5,
               boxcolors[style][2]*0.5);
    glVertex3f(0.00f, 0.00f, 1.00f);
    glColor3f (boxcolors[style][0]*0.4,
               boxcolors[style][1]*0.4,
               boxcolors[style][2]*0.4);
    glVertex3f(1.00f, 0.00f, 1.00f);
    glVertex3f(0.00f, 0.00f, 0.00f);
    glVertex3f(1.00f, 0.00f, 1.00f);
    glVertex3f(0.00f, 0.00f, 0.00f);
    glColor3f (boxcolors[style][0]*0.35,
               boxcolors[style][1]*0.35,
               boxcolors[style][2]*0.35);
    glVertex3f(1.00f, 0.00f, 0.00f);
    /* right */
    glColor3f (boxcolors[style][0]*0.5,
               boxcolors[style][1]*0.5,
               boxcolors[style][2]*0.5);
    glVertex3f(1.00f, 0.00f, 1.00f);
    glVertex3f(1.00f, 0.00f, 0.00f);
    glVertex3f(1.00f, 1.00f, 1.00f);
    glVertex3f(1.00f, 0.00f, 0.00f);
    glVertex3f(1.00f, 1.00f, 0.00f);
    glVertex3f(1.00f, 1.00f, 1.00f);
    /* left */
    glColor3f (boxcolors[style][0]*0.7,
               boxcolors[style][1]*0.7,
               boxcolors[style][2]*0.7);
    glVertex3f(0.00f, 0.00f, 1.00f);
    glVertex3f(0.00f, 0.00f, 0.00f);
    glVertex3f(0.00f, 1.00f, 1.00f);
    glVertex3f(0.00f, 0.00f, 0.00f);
    glVertex3f(0.00f, 1.00f, 0.00f);
    glVertex3f(0.00f, 1.00f, 1.00f);
    glEnd();
    glPopMatrix();
    return;
}


static void init_gl(int width, int height)
{
    glViewport(0, 0, width, height);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,15.0f,50.0f);
    glMatrixMode(GL_MODELVIEW);
}



int gfx_init()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "gfx_init(): %s\n", SDL_GetError());
        return -1;
    }
    atexit(SDL_Quit);
    if (SDL_SetVideoMode(1024, 768, 0, SDL_OPENGL) == NULL) {
        fprintf(stderr, "gfx_init(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_WM_SetCaption("BKTetris", NULL);
    init_gl(1024, 768);

    /* set animation timer */
    gtimer_set(&animtimer, 180);
    return 0;
}

static void draw_pit()
{
    static int x,y;
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_TRIANGLES);
    glColor4f(0.3f, 0.7f, 0.8f, 0.45f);
    glVertex3f(0.0f, 20.0f, 0.0f);
    glColor4f(0.3f, 0.7f, 1.0f, 0.85f);
    glVertex3f(0.0f, 0.0f,  0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    glColor4f(0.3f, 0.7f, 0.8f, 0.45f);
    glVertex3f(0.0f, 20.0f, 0.0f);
    glColor4f(0.3f, 0.7f, 0.8f, 0.25f);
    glVertex3f(10.0f, 20.0f,0.0f);
    /* bottom */
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.0f, -0.01f,  0.0f);
    glVertex3f(10.0f, -0.01f, 0.0f);
    glVertex3f(0.0f, -0.01f, -1.0f);
    glVertex3f(10.0f,-0.01f, 0.0f);
    glVertex3f(0.0f, -0.01f, -1.0f);
    glVertex3f(10.0f,-0.01f, -1.0f);
    /* grid */
    glColor4f(1.0f, 1.0f, 1.0f, 0.10f);
    for (x=0; x<9; x++) {
        glVertex3f(0.98f+x, 0.0f, 0.02f);
        glVertex3f(1.02f+x, 0.0f, 0.02f);
        glVertex3f(0.98f+x, 20.0f, 0.02f);
        glVertex3f(1.02f+x, 0.0f, 0.02f);
        glVertex3f(1.02f+x, 20.0f, 0.02f);
        glVertex3f(0.98f+x, 20.0f, 0.02f);
    }
    for (y=0; y<19; y++) {
        glVertex3f(0.0f,  0.98f+y, 0.02f);
        glVertex3f(10.0f, 0.98f+y, 0.02f);
        glVertex3f(0.0f,  1.02f+y, 0.02f);
        glVertex3f(10.0f, 0.98f+y, 0.02f);
        glVertex3f(10.0f, 1.02f+y, 0.02f);
        glVertex3f(0.0f,  1.02f+y, 0.02f);
    }
    glEnd();
    glDisable(GL_BLEND);
}

int gfx_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return 0;
}


int gfx_redraw_menu()
{
    int i;
    if (gtimer_check(&animtimer)) {
        for (i = 0; i<14; i++) {
            ;
        }
        gtimer_restart(&animtimer);
    }
    return 0;
}


static void draw_next_piece(game_t *game)
{
    static int x,y,val;
    static const frame_t *frame;
    frame = piece_frame_get(&(game->nextpiece));
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 5.0f);
    for (y = -1; y < BLOCK_H; y++) {
        for (x = 0; x < BLOCK_W; x++) {
            if (y>=0 && y<BLOCK_H && x>=0 && x<BLOCK_W) {
                val = (*frame)[y][x];
            } else {
                val = 0;
            }
            if (val != 0) {
                putbox(y,x+3,game->nextpiece.type+1);
            }
        }
    }
    glPopMatrix();
    return;
}


static void draw_fixed_pieces(game_t *game)
{
    static int x,y;
    /* draw board */
    for (y = 0; y < BOARD_H; y++) {
        for (x = 0; x < BOARD_W; x++) {
            if (game->board.cells[y][x] != 0) {
                putbox(y,x,game->board.cells[y][x]);
            }
        }
    }
    return;
}


static void draw_piece(game_t *game)
{
    static int x,y,bx,by,val;
    static const frame_t *frame;
    frame = piece_frame_get(&(game->piece));
    for (y = 0; y < BLOCK_H; y++) {
        for (x = 0; x < BLOCK_W; x++) {
            bx = x + game->piece.x;
            by = y + game->piece.y;
            val = (*frame)[y][x];
            if (val != 0) {
                putbox(by,bx,game->piece.type+1);
            }
        }
    }
}


int gfx_redraw(game_t *game)
{
    static int x,y;
    static int bx,by;
    static int val;

    gfx_clear();
    glLoadIdentity();
    glTranslatef(-5, -7, -20.0f);
    glRotatef(-45,1,0,0);

    /* draw level / score
    mvwprintw(wstats, 1, 1,"%s",   "TOP   "    );
    mvwprintw(wstats, 2, 1,"%06d",   0         );
    mvwprintw(wstats, 4, 1,"%s",   "SCORE "    );
    mvwprintw(wstats, 5, 1,"%06d",  game->score);
    mvwprintw(wstats, 16,1,"%s",   "LEVEL "    );
    mvwprintw(wstats, 17,1,"%02d",  game->level);
    mvwprintw(wstats, 17,2,"%s",   "QUALITY"   );
    mvwprintw(wstats, 18,2,"%8.3f", game->quality);
    mvwprintw(wstats, 19,2,"%s",   "FTAG"   );
    mvwprintw(wstats, 20,2,"%8.3f", game->ftag);
    */

    glPushMatrix();
    glScalef(1.0f,1.0f,-1.0f);
    draw_next_piece(game);
    draw_fixed_pieces(game);
    draw_piece(game);
    glPopMatrix();
    draw_pit();
    draw_next_piece(game);
    draw_fixed_pieces(game);
    draw_piece(game);

    glFlush();
    SDL_GL_SwapBuffers();
    return 0;
}


int gfx_redraw_scores()
{
    /*
    mvwaddstr(wboard,1,6,"HIGH SCORES");
    mvwaddstr(wboard,3,6,"   -=*=-   ");
    for (i = 0; i < SCORES_MAX; i++) {
        mvwprintw(wboard,i+4,4,"%8s%06d", scores[i].nick, scores[i].value);
    }
    wrefresh(wboard);
    */
    return 0;
}


int gfx_redraw_gameover()
{
    /*
    mvwaddstr(wboard,7,5, "             ");
    mvwaddstr(wboard,8,5, "  GAME OVER  ");
    mvwaddstr(wboard,9,5, "             ");
    mvwaddstr(wboard,10,5," press space ");
    mvwaddstr(wboard,11,5," to restart, ");
    mvwaddstr(wboard,12,5," 'q' to quit ");
    mvwaddstr(wboard,13,5,"             ");
    wrefresh(wboard);
    */
    return 0;
}


int gfx_redraw_nickform()
{
    /*
    mvwaddstr(wboard,10,4,"YOUR NAME:");
    mvwprintw(wboard,11,4,"%s", nick);
    wrefresh(wboard);
    */
    return 0;
}


int gfx_done()
{
    return 0;
}
