
#include "types.h"
#include "imageIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>


void init_pic(MLV_Image *img, Pixel *pixel_average)
{
    int i, j, k, r, g, b;

    int pixelCounter;
    int rAcc = 0, gAcc = 0, bAcc = 0, mAcc = 0;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            int *r = &(pic[i][j].R);/*getting the addresses for the colors*/
            int *g = &(pic[i][j].G);
            int *b = &(pic[i][j].B);
            pic[i][j].m = 1;/*fixed value*/

            /*library function takes care of setting the colors*/
            MLV_get_pixel_on_image(img, i, j, r, g, b, NULL);


            /*accumulating to calculate the average*/
            rAcc += *r;
            gAcc += *g;
            bAcc += *b;
            mAcc += pic[i][j].m;

            pixelCounter++;
        }
    }
    rAcc /= pixelCounter;
    gAcc /= pixelCounter;
    bAcc /= pixelCounter;
    mAcc /= pixelCounter;

    /*calculating average*/
    *pixel_average = (Pixel){rAcc, gAcc, bAcc, 1};
}

void print_info(int iternum)
{
    printf("Generation: %d. Best evaluation: %d\n", iternum, best_now.evaluation);
}

void getBestImageNow(MLV_Image **image)
{
    *image = MLV_create_image(width, height);

    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            Pixel pix = best_now.appearance[i][j];
            MLV_Color color = MLV_rgba(pix.R / pix.m, pix.G / pix.m, pix.B / pix.m, 255);
            MLV_set_pixel_on_image(i, j, color, *image);
        }
    }
}