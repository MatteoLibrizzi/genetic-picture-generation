#ifndef __imageIO_
#define __imageIO_

#include "types.h"

void pic_getDimensions(char *imgPPM, int *height, int *width) ;
void init_pic(MLV_Image *img, Pixel *pic, Pixel *pixel_average, int height, int width);
void print_best(Amoeba best_now, int height, int width,char* imageOutput);
void print_info(Amoeba best_now, int iternum);

#endif
