#ifndef __types_
#define __types_

#define GeneNum 100
#define CommutateNum 10
#define Population 30
#define Mutation_Rate_Mask 2047

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
    Pixel appearance[300][300];
    int evaluation;
} Amoeba;

#endif
