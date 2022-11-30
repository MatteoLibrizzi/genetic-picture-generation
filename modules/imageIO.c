
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

void pic_getDimensions(char *imgPPM, int *height, int *width)
{
    FILE *fio;
    int i, j, k, r, g, b;
    char *trash;
    fio = fopen(imgPPM, "r");

    fscanf(fio, "%s", &trash); /* read P3 string */

    fscanf(fio, "%d%d", height, width);
    printf("%d %d\n", *height, *width);

    fclose(fio);
}

void init_pic(MLV_Image *img, Pixel *pic, Pixel *pixel_average, int height, int width)
{
    int i, j, k, r, g, b;

    int pixelCounter;
    int rAcc = 0, gAcc = 0, bAcc = 0, mAcc = 0;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            int *r = &((pic + i * height + j)->R);
            int *g = &((pic + i * height + j)->G);
            int *b = &((pic + i * height + j)->B);
            int *m = &((pic + i * height + j)->m);

            MLV_get_pixel_on_image(img, i, j, r, g, b, m);

            rAcc += *r;
            gAcc += *g;
            bAcc += *b;
            mAcc += *m;

            pixelCounter++;
        }
    }
    rAcc /= pixelCounter;
    gAcc /= pixelCounter;
    bAcc /= pixelCounter;
    mAcc /= pixelCounter;

    *pixel_average = (Pixel){rAcc, gAcc, bAcc, mAcc};
}

void print_best(Amoeba best_now, int height, int width, char *imageOutput)
{
    int i, j, k, l, r, g, b, x;
    FILE *fio;
    fio = fopen(imageOutput, "w");
    fprintf(fio, "P3\n");
    fprintf(fio, "%d %d\n", height, width);
    fprintf(fio, "255\n");
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            x = best_now.appearance[i][j].m;

            r = best_now.appearance[i][j].R;
            r /= x;

            g = best_now.appearance[i][j].G;
            g /= x;

            b = best_now.appearance[i][j].B;
            b /= x;

            fprintf(fio, "%d %d %d\n", r, g, b);
        }
    }
    fclose(fio);
}

void print_info(Amoeba best_now, int iternum)
{
    printf("Generation: %d. Best evaluation: %d\n", iternum, best_now.evaluation);
}