/*
MIT License

Copyright (c) 2009 fwjmath
Copyright (c) 2022 fwjmath

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include "modules/types.h"
#include "modules/imageIO.h"
#include "modules/utils.h"
#include "modules/amoeba.h"
#include "modules/pool.h"
#include "modules/view.h"

Pixel pic[MAXHEIGHT][MAXWIDTH];
Amoeba pool[Population];
Amoeba best_now;
int height, width;

int main(int argc, char **argv)
{
	clock_t start,end;
	start = clock();

	int genNumber = 10;
	char *imageInputPath = "images/input.ppm";
	/*default values*/

	if (argc < 2)
	{
		printf("Please insert arguments!\n");
		printf("Running with default parametres:\ngenNumber = %d\ninputImagePath = %s\n", genNumber, imageInputPath);
	}
	else if (argc == 2)
	{
		genNumber = strtol(argv[1], NULL, 10);
		printf("Missing argument!\nRunning with default image path:\ninputImagePath = %s\n", imageInputPath);
	}
	else if (argc == 3)
	{
		genNumber = strtol(argv[1], NULL, 10);
		imageInputPath = argv[2];
		printf("Using arguments:\ngenNumber = %d\ninputImagePath = %s\n", genNumber, imageInputPath);
	}

	char *imageOutputPath = "images/output.bmp";/*output*/
	Pixel pixel_average;
	int iternum;

	MLV_create_window("TP7", "TP7", 1, 1);/*creating window, will resize later*/

	MLV_Image *inputImage = MLV_load_image(imageInputPath);/*loading input image*/
	MLV_Image *outputImage;

	MLV_get_image_size(inputImage, &width, &height);/*getting size*/

	if (height > MAXHEIGHT || width > MAXWIDTH)/*making sure it fits*/
	{
		printf("Image is too big, edit the constats in const.h to fit\n");
		exit(1);
	}

	MLV_change_window_size(width * 2, height);/*resizing based on image size*/

	MLV_draw_image(inputImage, 0, 0);/*drawing input*/
	MLV_actualise_window();

	init_pic(inputImage, &pixel_average);/*initializing pic, internal representation*/
	init_pool(pixel_average);/*initializing our genetic pool*/

	best_now = pool[0];/*initializing best_now as the best value*/

	iternum = 0;

	clock_t startIteration, endIteration;
	double cpu_time_used;

	while (genNumber > iternum)
	{
		startIteration = clock();

		iterate_generation();/*getting next generation*/

		endIteration = clock();
		cpu_time_used += ((double)(endIteration - startIteration)) / CLOCKS_PER_SEC;

		if (!(iternum % 100))/*print information*/
		{
			getBestImageNow(&outputImage);

			MLV_draw_image(outputImage, width, 0);
			MLV_actualise_window();

			print_info(iternum);
		}

		iternum++;
	}

	printf("avg time spent calculating the next generation: %f\n", cpu_time_used / iternum);


	getBestImageNow(&outputImage);/*putting the best image inside object*/

	if (MLV_save_image_as_bmp(outputImage, imageOutputPath))/*saving the best image as file*/
	{
		printf("ERROR couldn't save!\n");
	}

	MLV_wait_seconds(10);

	MLV_free_image(inputImage);
	MLV_free_image(outputImage);
	MLV_free_window();

	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Total runtime: %fs\n",cpu_time_used);
	return 0;
}