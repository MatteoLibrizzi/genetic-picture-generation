#ifndef __view_
#define __view_

MLV_Image *drawInputImage(char *imageInput, int height, int width);
void drawOutputImage(char *imageOutput, int height, int width, MLV_Image imageInput);
void getBestImageNow(MLV_Image **image, Amoeba best_now, int height, int width);
void drawImage(MLV_Image *image, int height, int width);

#endif
