#ifndef __imageIO_
#define __imageIO_

#include "types.h"

void init_pic (char* imgPPM, Pixel* pic, Pixel* pixel_average, int* height, int* width);
void print_best(Amoeba best_now, int height, int width);
void print_info(Amoeba best_now, int iternum);

#endif
