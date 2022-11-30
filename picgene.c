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

Pixel pic[300][300];
Pixel pixel_average;
int height, width, iternum;
Amoeba pool[Population];
Amoeba best_now;


int main(){
	int i;
	char c;
	MLV_create_window("TP7", "TP7", 600, 300);
	MLV_Image *im = MLV_load_image("images/geometric.ppm");
	MLV_draw_image(im,0,0);
	MLV_actualise_window();

	init_pic("images/geometric.ppm",pic,&pixel_average,&height,&width);
	init_pool(pool,pic, pixel_average, height,width);


	best_now = pool[0];
	iternum=0;
	scanf("%d",&i);

	while(i>=iternum) iterate_generation(pool,pic,&best_now,&iternum,height,width);
	print_info(best_now, iternum);
	print_best(best_now, height,width);
	
	MLV_actualise_window();
	MLV_Image *im2 = MLV_load_image("images/result.ppm");
	MLV_draw_image(im2, 300, 0);
	MLV_actualise_window();
	MLV_wait_seconds(20);


	MLV_free_image(im);
	MLV_free_image(im2);
	MLV_free_window();

	return 0;
}