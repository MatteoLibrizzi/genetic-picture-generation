#ifndef __imageIO_
#define __imageIO_

#include "types.h"
#include <MLV/MLV_all.h>

extern Pixel pic[MAXHEIGHT][MAXWIDTH];
extern Amoeba best_now;
extern int height, width;

void init_pic(MLV_Image *img, Pixel *pixel_average);/*initializes pic, which has to be a global variable*/
void print_info(int iternum);/*prints information of the CURRENT generation, uses best_now*/
void getBestImageNow(MLV_Image **image); /*returns inside the obj the best image we have, using best_now*/

#endif
