#ifndef __amoeba_
#define __amoeba_

#include "types.h"

extern Pixel pic[MAXHEIGHT][MAXWIDTH];
extern int height, width;

void evaluate(Amoeba* amoeba);/*evaluates the given amoeba, stores the result inside the object*/
void cover_triangle(Amoeba *obj, int gen);/*adds the given triangle to the pic (must be a global variable)*/
void delete_triangle(Amoeba *obj, int gen);/*removes the given triangle from the pic (must be a global variable)*/

#endif
