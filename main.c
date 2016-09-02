#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <SDL.h>
#include "piece.h"
#include "game.h"
#include "gfx.h"
#include "timing.h"
#include "scores.h"
#include "ai.h"



#define MENU_PLAY   1
#define MENU_QUIT   2
#define MENU_SCORES 3
#define EVENT_QUIT  10

static game_t game;
/* gtimer_t falltimer; */



int init()
{
    int ret;
    ret = gfx_init();
    if(ret!=0) {
        fprintf(stderr, "init(): error initializing graphics");
        goto error;
    }
    game_reset(&game);
    scores_init();
    /* srand(time(NULL)); */
    return 0;
error:
    gfx_done();
    return -1;
}


int done()
{
    gfx_done();
    return 0;
}


static int gameover_menu()
{
    gfx_redraw_gameover();
    /*
    while(1) {
        key = getch();
        if (key != ERR) {
            switch (key) {
                case ' ':
                    return MENU_PLAY;
                    break;
                case 'q':
                    return MENU_QUIT;
                    break;
            }
        }
        gsleep();
    }
    */
    return MENU_QUIT;
}


static int play_loop()
{
    int event;
    static SDL_Event keyevent;
    static gtimer_t sidetimer;
    static gtimer_t downtimer;
    static piece_t best_piece;

    gfx_redraw(&game);
    gtimer_set(&sidetimer, 220);
    gtimer_set(&downtimer, 500);

    game.gameover = 0;
    for (;;) {
        event = 0;

        /************
         **** AI ****
         ************/

        if (game.ai == 1) {
            while (SDL_PollEvent(&keyevent))
            {
                switch(keyevent.type) {
                    case SDL_KEYDOWN:
                        switch(keyevent.key.keysym.sym) {
                            case SDLK_d:
                                game.ai = 0;
                                game.score = 0;
                                break;
                            case SDLK_q:
                                game.ai = 0;
                                game.gameover = 1;
                                event = EVENT_QUIT;
                                continue;
                            default:
                                break;
                        }
                }
            }
            if (game.rethink) {
                ai_best_move(&game, &best_piece);
            }
            if (gtimer_check(&sidetimer)) {
                if (game.piece.fn != best_piece.fn) {
                    event = EVENT_ROTR;
                } else {
                    if (game.piece.x < best_piece.x) {
                        event = EVENT_RIGHT;
                    } else if (game.piece.x > best_piece.x) {
                        event = EVENT_LEFT;
                    } else {
                        if (gtimer_check(&downtimer)) {
                            event = EVENT_DOWN;
                            gtimer_restart(&downtimer);
                            gtimer_set(&falltimer,1);
                        }
                    }
                }
                gtimer_restart(&sidetimer);
            }

        /*************
         *** human ***
         *************/

        } else {
            while (SDL_PollEvent(&keyevent))
            {
                switch(keyevent.type){
                    case SDL_KEYDOWN:
                        switch(keyevent.key.keysym.sym){
                            case SDLK_d:
                                game.ai = 1;
                                game.score = 0;
                                break;
                            case SDLK_z:
                                event=EVENT_ROTL;
                                printf("ROTL\n");
                                break;
                            case SDLK_x:
                                printf("ROTR\n");
                                event=EVENT_ROTR;
                                break;
                            case SDLK_LEFT:
                                printf("LEFT\n");
                                event=EVENT_LEFT;
                                break;
                            case SDLK_RIGHT:
                                printf("RIGHT\n");
                                event=EVENT_RIGHT;
                                break;
                            case SDLK_DOWN:
                                printf("DOWN\n");
                                event=EVENT_DOWN;
                                gtimer_set(&falltimer,1);
                                break;
                            case SDLK_q:
                                printf("QUIT\n");
                                event = EVENT_QUIT;
                                break;
                            default:
                                break;
                        }
                }
            }
        }
        if (gtimer_check(&falltimer)) {
            event = EVENT_DOWN;
            gtimer_restart(&falltimer);
        }
        if (event != 0) {
            game_update(&game, event);
            if (game.redraw) {
                printf("## play_loop(): redraw...\n");
                gfx_redraw(&game);
                game.redraw = 0;
            }
            if (event==EVENT_QUIT || game.gameover)
                break;
        }
        gsleep();
    }
    return 0;
}


static char *get_nick()
{
    int i,key;
    i = -1;
    f_nickform_needs_redraw = 1;
    while(1) {
        if (f_nickform_needs_redraw) {
            gfx_redraw_nickform();
            f_nickform_needs_redraw = 0;
        };
        key = getch();
        if (key != ERR) {
            if (key == KEY_BACKSPACE && i>=0) {
                f_nickform_needs_redraw = 1;
                nick[i] = ' '; i--;
            }
            if (key == '\n') {
                nick[NICK_MAX-1] = '\0';
                return nick;
            }
            if (key >= 'A' && key <= 'z' && i<NICK_MAX) {
                i++; nick[i] = key;
                f_nickform_needs_redraw = 1;
            }
        }
        gsleep();
    }
    nick[NICK_MAX-1] = '\0';
    return nick;
}

#define BS_HIGHSCORE_DIALOG 1
#define BS_GAMEOVER_MENU 2
#define BS_PLAY

static int board_loop()
{
    int choice;
    while(1) {
        game_reset(&game);
        gfx_redraw(&game);
        play_loop();
        /*
        if (scores_ishigh(game.score)) {
            scores_insert(get_nick(), game.score);
        }
        */
        choice = gameover_menu();
        if (choice == MENU_QUIT) {
            printf("## board_loop(): quit...\n");
            break;
        }
    }
    return 0;
}



int main_menu()
{

    gfx_clear();

    /*
    while(1) {
        key = getch();
        if (key != ERR) {
            switch (key) {
                case 'q':
                    return MENU_QUIT;
                    break;
                case 'n':
                    return MENU_PLAY;
                    break;
                case 's':
                    return MENU_SCORES;
                    break;
            }
        }
        gfx_redraw_menu();
        gsleep();
    }
    */
    return MENU_PLAY;
}


void show_scores()
{
    int key;
    gfx_clear();
    while (1) {
        key = getch();
        if (key != ERR) {
            switch (key) {
                case 'q':
                    return;
                    break;
            }
        }
        gfx_redraw_scores();
        gsleep();
    }
}


int main()
{
    int choice, ret;
    ret = init();
    if(ret != 0) {
        fprintf(stderr, "main(): initialization error!");
        goto error;
    }
    /*
    choice = 0;
    while (choice != MENU_QUIT) {
        choice = main_menu();
        switch (choice) {
            case MENU_PLAY:
                board_loop();
                break;
            case MENU_SCORES:
                show_scores();
                break;
        }
    }
    */
    board_loop();
    done();
    return 0;
error:
    done();
    exit(1);
}
