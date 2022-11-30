
#include "types.h"

#include <MLV/MLV_all.h>

MLV_Image *drawInputImage(char *imageInput, int height, int width)
{
    MLV_create_window("TP7", "TP7", width*2, height);
    MLV_Image *im = MLV_load_image(imageInput);
    MLV_draw_image(im, 0, 0);
    MLV_actualise_window();
    return im;
}

void drawOutputImageThenFree(char* imageOutput,int height, int width, MLV_Image* imageInput) {
    MLV_Image *im2 = MLV_load_image(imageOutput);
    MLV_draw_image(im2, width, 0);
    MLV_actualise_window();
    MLV_wait_seconds(20);

    MLV_free_image(imageInput);
    MLV_free_image(im2);
    MLV_free_window();
}

void drawImage(MLV_Image* image, int height, int width) {
    MLV_draw_image(image, width, 0);
    MLV_actualise_window();
}

void getBestImageNow(MLV_Image** image, Amoeba best_now,int height,int width) {
    *image = MLV_create_image(width,height);

    int i, j;
    for (i=0;i<height;i++) {
        for (j=0;j<width;j++) {
            Pixel pix = best_now.appearance[i][j];
            MLV_Color color = MLV_rgba(pix.R,pix.G,pix.B,pix.m);
            MLV_set_pixel_on_image(i,j,color,*image);
        }
    }

}