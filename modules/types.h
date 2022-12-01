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
} Pixel;/*Pixel obj, m is the multiplier for the colors*/

typedef struct triangle
{
    int point[3][2];
    int R;
    int G;
    int B;
} Triangle;/*triangle containing the colors, and the coordinates*/

typedef struct amoeba
{
    Triangle gene[GeneNum];
    Pixel appearance[MAXHEIGHT][MAXWIDTH];
    int evaluation;
} Amoeba;/*mutating object, ideally evaluation improves every generation*/

#endif
