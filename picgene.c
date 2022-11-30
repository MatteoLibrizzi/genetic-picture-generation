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
#include "io.h"
#include <time.h>
#include "modules/types.h"
#include "modules/imageIO.h"
#include "modules/utils.h"
#include "modules/amoeba.h"
#include "modules/pool.h"
#include "modules/view.h"

int main(int argc, char** argv){
	int genNumber = 10;
	char *imageInputPath = "images/input.ppm";

	if (argc <= 1) {
		printf("Please insert arguments!\n");
		printf("Running with default parametres:\ngenNumber = %d\ninputImagePath = %s\n",genNumber,imageInputPath);
	} else if (argc <= 2) {
		genNumber = strtol(argv[1],NULL,10);
		printf("Missing argument!\nRunning with default image path:\ninputImagePath = %s\n",imageInputPath);
	}
	
	char* imageOutputPath = "images/output.ppm";
	Pixel pixel_average;
	int height, width, iternum;

	Amoeba* pool = (Amoeba*) malloc(sizeof(Amoeba)*Population);/*malloc is absolutely necessary here :c*/

	pic_getDimensions(imageInputPath, &height, &width);
	Pixel pic[height][width];


	MLV_Image *inputImage = drawInputImage(imageInputPath, height, width);
	MLV_Image *outputImage;


	init_pic(imageInputPath, pic, &pixel_average, height, width);
	init_pool(pool,pic, pixel_average, height,width);

	Amoeba best_now = pool[0];

	iternum=0;
	while(genNumber>=iternum) {
		iterate_generation(pool,pic,&best_now,height,width);
		iternum++;
		getBestImageNow(&outputImage,best_now,height,width);
		drawImage(outputImage,height,width);
	}
	print_info(best_now, iternum);
	print_best(best_now, height,width,imageOutputPath);

	MLV_wait_seconds(10);

	MLV_free_image(inputImage);
	MLV_free_image(outputImage);
	MLV_free_window();
	free(pool);
	return 0;
}