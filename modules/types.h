#ifndef __types_
#define __types_

#include "const.h"

typedef enum coordinate
{
    Y,
    X
} COORDINATE;

typedef struct pixel
{
    int R;
    int G;
    int B;
    int m;
} Pixel;

typedef struct triangle
{
    int point[3][2];
    int R;
    int G;
    int B;
} Triangle;

typedef struct amoeba
{
    Triangle gene[GeneNum];
    Pixel appearance[MAXHEIGHT][MAXWIDTH];
    int evaluation;
} Amoeba;

#endif
