#ifndef __pool_
#define __pool_

#include "types.h"

extern Amoeba pool[Population];
extern Pixel pic[MAXHEIGHT][MAXWIDTH];
extern Amoeba best_now;
extern int height, width;

void init_pool(Pixel pixel_average);/*initializing pool (global variable)*/
void iterate_generation();/*iterating generation inside pool*/

#endif
