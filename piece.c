#include <stdlib.h>
#include "piece.h"


typedef struct blkdef_t {
    const frame_t *frame;      /* piece shapes */
    const int nframes;         /* number of frames */
} blkdef_t;


static const int piece1[][4][4] = {
    {
        { 0,1,0,0 },
        { 0,1,0,0 },
        { 0,1,0,0 },
        { 0,1,0,0 }
    },
    {
        { 0,0,0,0 },
        { 1,1,1,1 },
        { 0,0,0,0 },
        { 0,0,0,0 }
    }
};

static const int piece2[][4][4] = {
    {
        { 0,1,0,0 },
        { 0,1,0,0 },
        { 1,1,0,0 },
        { 0,0,0,0 }
    },
    {
        { 1,0,0,0 },
        { 1,1,1,0 },
        { 0,0,0,0 },
        { 0,0,0,0 }
    },
    {
        { 0,1,1,0 },
        { 0,1,0,0 },
        { 0,1,0,0 },
        { 0,0,0,0 }
    },
    {
        { 0,0,0,0 },
        { 1,1,1,0 },
        { 0,0,1,0 },
        { 0,0,0,0 }
    }
};

static const int piece3[][4][4] = {
    {
        { 0,1,0,0 },
        { 0,1,0,0 },
        { 0,1,1,0 },
        { 0,0,0,0 }
    },
    {
        { 0,0,0,0 },
        { 1,1,1,0 },
        { 1,0,0,0 },
        { 0,0,0,0 }
    },
    {
        { 1,1,0,0 },
        { 0,1,0,0 },
        { 0,1,0,0 },
        { 0,0,0,0 },
    },
    {
        { 0,0,1,0 },
        { 1,1,1,0 },
        { 0,0,0,0 },
        { 0,0,0,0 }
    }
};

static const int piece4[][4][4] = {
    {
        { 0,1,1,0 },
        { 0,1,1,0 },
        { 0,0,0,0 },
        { 0,0,0,0 }
    }
};

static const int piece5[][4][4] = {
    {
        { 0,1,0,0 },
        { 1,1,1,0 },
        { 0,0,0,0 },
        { 0,0,0,0 }
    },
    {
        { 0,1,0,0 },
        { 0,1,1,0 },
        { 0,1,0,0 },
        { 0,0,0,0 }
    },
    {
        { 0,0,0,0 },
        { 1,1,1,0 },
        { 0,1,0,0 },
        { 0,0,0,0 }
    },
    {
        { 0,1,0,0 },
        { 1,1,0,0 },
        { 0,1,0,0 },
        { 0,0,0,0 }
    }
};

static const int piece6[][4][4] = {
    {
        { 1,1,0,0 },
        { 0,1,1,0 },
        { 0,0,0,0 },
        { 0,0,0,0 }
    },
    {
        { 0,1,0,0 },
        { 1,1,0,0 },
        { 1,0,0,0 },
        { 0,0,0,0 }
    }
};

static const int piece7[][4][4] = {
    {
        { 0,1,1,0 },
        { 1,1,0,0 },
        { 0,0,0,0 },
        { 0,0,0,0 }
    },
    {
        { 0,1,0,0 },
        { 0,1,1,0 },
        { 0,0,1,0 },
        { 0,0,0,0 }
    }
};


static const blkdef_t blkdef[BLOCKS] = {
    { piece1, 2, },
    { piece2, 4, },
    { piece3, 4, },
    { piece4, 1, },
    { piece5, 4, },
    { piece6, 2, },
    { piece7, 2, }
};


int piece_maxframes[BLOCKS] = { 2,4,4,1,4,2,2 };


int myrand(int end) {
    return (double) rand() * (end + 1) / RAND_MAX;
}


piece_t *piece_rand(piece_t *piece)
{
    piece->type = myrand(BLOCKS-1);
    piece->x = 0;
    piece->y = 0;
    piece_frame_rand(piece);
    return piece;
}

const frame_t *piece_frame_get(piece_t *piece)
{
    return &blkdef[piece->type].frame[piece->fn];
}


const frame_t *piece_frame_prev(piece_t *piece)
{
    piece->fn = (piece->fn - 1) % blkdef[piece->type].nframes;
    return &blkdef[piece->type].frame[piece->fn];
}


const frame_t *piece_frame_next(piece_t *piece)
{
    piece->fn = (piece->fn + 1) % blkdef[piece->type].nframes;
    return &blkdef[piece->type].frame[piece->fn];
}


const frame_t *piece_frame_rand(piece_t *piece)
{
    piece->fn = myrand(blkdef[piece->type].nframes-1);
    return &(blkdef[piece->type]).frame[piece->fn];
}
