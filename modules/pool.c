
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
}

void iterate_generation(Amoeba *pool,Pixel* pic, Amoeba *best_now, int height, int width){
    int i, j, k, l;
    int tri[Population][2];
    /* sort by evaluation */
    for (i = 0; i < Population; i++)
    {
        tri[i][0] = pool[i].evaluation;
        tri[i][1] = i;
    }
    for (i = 0; i < Population; i++)
    {
        for (j = 0; j < Population - 1; j++)
        {
            if (tri[j][0] > tri[j + 1][0])
            {
                l = tri[j][0];
                tri[j][0] = tri[j + 1][0];
                tri[j + 1][0] = l;
                l = tri[j][1];
                tri[j][1] = tri[j + 1][1];
                tri[j + 1][1] = l;
            }
        }
    }
    /* Update Best Amoeba */
    if (best_now->evaluation > pool[tri[0][1]].evaluation)
        *best_now = pool[tri[0][1]];
    /* Replace "failure" with "success", with commutation */
    for (i = 0; i < Population / 3; i++)
    {
        pool[tri[Population - 1 - i][1]] = pool[tri[i][1]];
        j = rand() % (Population / 3);
        j += Population / 3;
        for (k = 0; k < CommutateNum; k++)
        {
            l = rand() % GeneNum;
            delete_triangle(&pool[tri[Population - 1 - i][1]], l, height, width);
            pool[tri[Population - 1 - i][1]].gene[l] = pool[j].gene[l];
            cover_triangle(&pool[tri[Population - 1 - i][1]], l, height, width);
        }
        evaluate(&pool[tri[Population - 1 - i][1]], pic, height, width);
    }
    /* Mutation */
    for (i = 0; i < Population; i++)
    {
        l = 0;
        for (j = 0; j < GeneNum; j++)
        {
            for (k = 0; k < 9; k++)
            {
                if ((rand() & Mutation_Rate_Mask) == 14)
                {
                    delete_triangle(&pool[i], j, height, width);
                    if (k < 6)
                    {
                        if ((k & 1) == 0)
                        {
                            pool[i].gene[j].point[(k >> 1) % 3][0] += (rand() % (height >> 1)) - (height >> 2);
                            if (pool[i].gene[j].point[(k >> 1) % 3][0] < 0)
                                pool[i].gene[j].point[(k >> 1) % 3][0] += height;
                            if (pool[i].gene[j].point[(k >> 1) % 3][0] >= height)
                                pool[i].gene[j].point[(k >> 1) % 3][0] -= height;
                        }
                        else
                        {
                            pool[i].gene[j].point[(k >> 1) % 3][1] += (rand() % (width >> 1)) - (width >> 2);
                            if (pool[i].gene[j].point[(k >> 1) % 3][1] < 0)
                                pool[i].gene[j].point[(k >> 1) % 3][1] += width;
                            if (pool[i].gene[j].point[(k >> 1) % 3][1] >= width)
                                pool[i].gene[j].point[(k >> 1) % 3][1] -= width;
                        }
                    }
                    else
                    {
                        if (k == 6)
                        {
                            pool[i].gene[j].R += rand() % 255;
                            pool[i].gene[j].R &= 255;
                        }
                        else if (k == 7)
                        {
                            pool[i].gene[j].G += rand() % 255;
                            pool[i].gene[j].G &= 255;
                        }
                        else
                        {
                            pool[i].gene[j].B += rand() % 255;
                            pool[i].gene[j].B &= 255;
                        }
                    }
                    cover_triangle(&pool[i], j, height, width);
                    l = 1;
                }
            }
        }
        if (l == 1)
        {
            evaluate(&pool[i], pic, height, width);
        }
    }
}