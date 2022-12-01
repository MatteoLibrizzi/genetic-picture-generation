#ifndef __poolOps_
#define __poolOps_

#include "types.h"

void init_pool(Amoeba* pool,Pixel** pic, Pixel pixel_average, int height, int width);
void iterate_generation(Amoeba* pool,Pixel** pic,Amoeba* best_now, int height, int width);

#endif
