
#include "types.h"
#include "amoeba.h"
#include <stdlib.h>

void init_pool(Amoeba *pool, Pixel* pic, Pixel pixel_average, int height, int width){
    int i, j, k, l;
    srand(time(NULL));

    for (i = 0; i < Population; i++)
    {
        for (j = 0; j < height; j++)
        {
            for (k = 0; k < width; k++)
            {
                pool[i].appearance[j][k] = pixel_average;
            }
        }
        for (j = 0; j < GeneNum; j++)
        {
            for (k = 0; k < 3; k++)
            {
                pool[i].gene[j].point[k][0] = rand() % height; /*picking a random point to place the triangle*/
                pool[i].gene[j].point[k][1] = rand() % width;
            }
            pool[i].gene[j].R = rand() & 255; /*same as % 256*/
            pool[i].gene[j].G = rand() & 255; /*picking random colors*/
            pool[i].gene[j].B = rand() & 255;
            cover_triangle(&pool[i], j, height, width);
        }
        evaluate(&pool[i], pic, height, width);
    }
    return;
}