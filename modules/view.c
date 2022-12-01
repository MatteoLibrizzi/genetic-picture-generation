
#include "types.h"
#include "view.h"
#include <MLV/MLV_all.h>


void getBestImageNow(MLV_Image** image) {
    *image = MLV_create_image(width,height);

    int i, j;
    for (i=0;i<height;i++) {
        for (j=0;j<width;j++) {
            Pixel pix = best_now.appearance[i][j];
            MLV_Color color = MLV_rgba(pix.R / pix.m, pix.G / pix.m, pix.B / pix.m,255);
            MLV_set_pixel_on_image(i,j,color,*image);
        }
    }

}