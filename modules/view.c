

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