
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

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

    void init_pic(char *imgPPM, Pixel *pic, Pixel *pixel_average, int height, int width)
{
    FILE *fio;
    int i, j, k, r, g, b;
    char *trash;
    int height2,width2;
    fio = fopen(imgPPM, "r");

    fscanf(fio, "%s", &trash); /* read P3 string */

    fscanf(fio, "%d%d", &height2, &width2);
    
    if (height2 != height || width2 != width)
        exit(1);

    int pixelCounter;
    int rAcc = 0, gAcc = 0, bAcc = 0;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            fscanf(fio, "%d%d%d", &r, &g, &b);
            (pic + i * height + j)->R = r;
            rAcc += r;

            (pic + i * height + j)->G = g;
            gAcc += g;

            (pic + i * height + j)->B = b;
            bAcc += b;

            (pic + i * height+ j)->m = 1;

            pixelCounter++;
        }
    }
    fclose(fio);
    rAcc /= pixelCounter;
    gAcc /= pixelCounter;
    bAcc /= pixelCounter;

    *pixel_average = (Pixel){rAcc, gAcc, bAcc, 1};
    return;
}

void print_best(Amoeba best_now, int height, int width, char* imageOutput)
{
    int i, j, k, l, r, g, b, x;
    FILE *fio;
    fio = fopen(imageOutput, "w");
    fprintf(fio, "P3\n");
    fprintf(fio, "%d %d\n", height, width);
    fprintf(fio, "255\n");
    /*fprintf(fio,"%d %d %d %d %d\n",height,width,GeneNum,iternum,best_now.evaluation);

    fprintf(fio,"%d %d %d\n",pixel_average.R,pixel_average.G,pixel_average.B);
    for(i=0;i<GeneNum;i++){
        fprintf(fio,"%d %d %d %d %d %d %d %d %d\n", best_now.gene[i].point[0][0],best_now.gene[i].point[0][1],best_now.gene[i].point[1][0],best_now.gene[i].point[1][1],best_now.gene[i].point[2][0],best_now.gene[i].point[2][1],best_now.gene[i].R,best_now.gene[i].G,best_now.gene[i].B);
    }
    */
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
    return;
}

void print_info(Amoeba best_now, int iternum)
{
    printf("Generation: %d. Best evaluation: %d\n", iternum, best_now.evaluation);
    return;
}