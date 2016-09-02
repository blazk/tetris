#ifndef BLOCKS_H
#define BLOCKS_H

#define BLOCKS    7
#define BLOCK_W   4
#define BLOCK_H   4

/*
 * piece shape
 */
typedef int frame_t[BLOCK_H][BLOCK_W];
typedef const int (*framep_t)[BLOCK_H][BLOCK_W];

/*
 * piece object
 */
typedef struct piece_t {
    unsigned int type;      /* piece type */
    int x,y;                /* position of the upper left corner */
    unsigned int fn;        /* frame number */
} piece_t;

extern int piece_maxframes[];


piece_t        *piece_rand(piece_t *piece);
const frame_t  *piece_frame_get  (piece_t *piece);
const frame_t  *piece_frame_next (piece_t *piece);
const frame_t  *piece_frame_prev (piece_t *piece);
const frame_t  *piece_frame_rand (piece_t *piece);

#endif
