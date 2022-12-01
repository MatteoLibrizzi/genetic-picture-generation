
#include "types.h"
#include "amoeba.h"
#include "utils.h"
#include "pool.h"
#include <stdlib.h>

void init_pool(Pixel pixel_average)
{
    int i, j, k, l;
    srand(time(NULL));

    for (i = 0; i < Population; i++)
    {
        for (j = 0; j < height; j++)
        {
            for (k = 0; k < width; k++)
            {
                pool[i].appearance[j][k] = pixel_average;/*setting pixels as the average (our best bet)*/
            }
        }
        for (j = 0; j < GeneNum; j++)
        {
            Triangle* currentTri = &(pool[i].gene[j]);/*address of the triangle to be created*/
            for (k = 0; k < 3; k++)
            {
                currentTri->point[k][0] = rand() % height; /*picking a random point to place the triangle*/
                currentTri->point[k][1] = rand() % width;
            }
            currentTri->R = rand() & 255;     /*same as % 256*/
            currentTri->G = rand() & 255;     /*picking random colors*/
            currentTri->B = rand() & 255;

            cover_triangle(&pool[i], j);/*drawing the triangle*/
        }
        /*evaluating with the new triangles*/
        evaluate(&pool[i]);
    }
}

void iterate_generation()
{
    int i, j, k, l;
    int tri[Population][2];
    /* sort by evaluation */
    for (i = 0; i < Population; i++)
    {
        tri[i][0] = pool[i].evaluation;
        tri[i][1] = i;
    }
    int swapped;
    /*Improved bubble sort*/
    for (i = 0; i < Population-1; i++)
    {
        swapped = 0;
        for (j = 0; j < Population - i - 1; j++)
        {
            if (tri[j][0] > tri[j + 1][0])
            {
                /*swapping*/

                l = tri[j][0];
                tri[j][0] = tri[j + 1][0];
                tri[j + 1][0] = l;
                
                l = tri[j][1];
                tri[j][1] = tri[j + 1][1];
                tri[j + 1][1] = l;

                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }

    /* Update Best Amoeba */
    if (best_now.evaluation > pool[tri[0][1]].evaluation)
        best_now = pool[tri[0][1]];

    /* Replace "failure" with "success", with commutation */
    for (i = 0; i < Population / 3; i++)
    {
        pool[tri[Population - 1 - i][1]] = pool[tri[i][1]];/*replacing worse with better*/

        j = rand() % (Population / 3);
        j += Population / 3;

        for (k = 0; k < CommutateNum; k++)
        {
            l = rand() % GeneNum;
            delete_triangle(&pool[tri[Population - 1 - i][1]], l);/*removing the replaced triangle from pic*/

            pool[tri[Population - 1 - i][1]].gene[l] = pool[j].gene[l];/*replacing with better triangle*/

            cover_triangle(&pool[tri[Population - 1 - i][1]], l);/*inserting it in pic*/
        }

        evaluate(&pool[tri[Population - 1 - i][1]]);/*evaluating new amoeba*/
    }

    /* Mutation */
    for (i = 0; i < Population; i++)
    {
        l = 0;
        for (j = 0; j < GeneNum; j++)
        {
            for (k = 0; k < 9; k++)
            {
                if ((rand() & Mutation_Rate_Mask) == 14)/*we mutate 10 times (k = 0...9) in 2047 cases for every element*/
                {
                    delete_triangle(&pool[i], j);

                    Triangle* currentTri = &(pool[i].gene[j]);

                    /*mutation code, somewhat arbitrary, the best will be kept in the end*/
                    if (k < 6)
                    {
                        if ((k & 1) == 0)
                        {
                            currentTri->point[(k >> 1) % 3][0] += (rand() % (height >> 1)) - (height >> 2);

                            if (currentTri->point[(k >> 1) % 3][0] < 0)
                                currentTri->point[(k >> 1) % 3][0] += height;

                            if (currentTri->point[(k >> 1) % 3][0] >= height)
                                currentTri->point[(k >> 1) % 3][0] -= height;
                        }
                        else
                        {
                            currentTri->point[(k >> 1) % 3][1] += (rand() % (width >> 1)) - (width >> 2);
                            if (currentTri->point[(k >> 1) % 3][1] < 0)
                                currentTri->point[(k >> 1) % 3][1] += width;
                            if (currentTri->point[(k >> 1) % 3][1] >= width)
                                currentTri->point[(k >> 1) % 3][1] -= width;
                        }
                    }
                    else
                    {
                        if (k == 6)
                        {
                            currentTri->R += rand() % 255;
                            currentTri->R &= 255;
                        }
                        else if (k == 7)
                        {
                            currentTri->G += rand() % 255;
                            currentTri->G &= 255;
                        }
                        else
                        {
                            currentTri->B += rand() % 255;
                            currentTri->B &= 255;
                        }
                    }
                    cover_triangle(&pool[i], j);
                    l = 1;
                }
            }
        }
        if (l == 1)/*only evaluating if there was a mutation*/
        {
            evaluate(&pool[i]);
        }
    }
}