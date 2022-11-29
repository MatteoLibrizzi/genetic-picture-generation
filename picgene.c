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

#define GeneNum 100
#define CommutateNum 10
#define Population 30
#define Mutation_Rate_Mask 2047

typedef enum coordinate {Y, X} COORDINATE;

typedef struct pixel{
	int R;
	int G;
	int B;
	int m;
} Pixel;

typedef struct triangle{
	int point[3][2];
	int R;
	int G;
	int B;
} Triangle;

typedef struct amoeba{
	Triangle gene[GeneNum];
	Pixel appearance[300][300];
	int evaluation;
} Amoeba;



Pixel pic[300][300];
Pixel pixel_average;
int height, width, iternum;
Amoeba pool[Population];
Amoeba best_now;

void init_pic(){
	FILE* fio;
	int i,j,k,r,g,b;
	char* trash;
	fio=fopen("geometric.ppm","r");

	fscanf(fio, "%s", &trash);/* read P3 string */

	fscanf(fio,"%d%d", &height, &width);
	printf("%d %d\n",height,width);

	int pixelCounter;
	int rAcc = 0, gAcc = 0, bAcc = 0;

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			fscanf(fio,"%d%d%d",&r,&g,&b);
			pic[i][j].R = r;
			rAcc += r;

			pic[i][j].G = g;
			gAcc += g;

			pic[i][j].B = b;
			bAcc += b;

			pic[i][j].m = 1;

			pixelCounter++;
		}
	}
	fclose(fio);
	rAcc /= pixelCounter;
	gAcc /= pixelCounter;
	bAcc /= pixelCounter;

	pixel_average = (Pixel) {rAcc, gAcc, bAcc, 1};
	return;
}

int sgn_int(int a){
	return a > 0 ? 1 : (a < 0 ? -1 : 0);
}

void cover_triangle(Amoeba *obj, int gen)/*draws triangle according to the picture*/
{
	int i, j, k, l, p, maxx, minx, maxy, miny, sgn1, sgn2;
	Triangle tri = obj->gene[gen];

	maxx = 0;/*I think names should be swapped but until I can be sure I'm leaving it like this*/
	minx = height;
	maxy = 0;
	miny = width;

	for(i=0;i<3;i++){/*here we look for the point most to the left/right/up/down*/
		if(tri.point[i][Y]>maxx) 
			maxx = tri.point[i][Y];

		if(tri.point[i][Y]<minx) 
			minx = tri.point[i][Y];

		if(tri.point[i][X]>maxy) 
			maxy = tri.point[i][X];

		if(tri.point[i][X]<miny) 
			miny = tri.point[i][X];
	}

	for(p=0;p<3;p++){
		l = (p+1);
		if(l==3) l=0;/*likely refactor*/

		sgn1 = tri.point[l][Y] - tri.point[p][Y];/*analyze one of the points with the other ones (0,1) (1,2) (2,0)*/
		sgn2 = tri.point[l][X] - tri.point[p][X];/*sgn1 Y sgn2 X ?*/

		if(sgn_int(sgn1) == 0){/*the two points have the same Y coordinate*/
			i = tri.point[p][Y];/*point A is p*/
			j = tri.point[p][X];
			sgn2 = sgn_int(sgn2);/*which point has the biggest X coordinate ? +1->l    -1->p*/
			do{/*change the values for those specific pixels included between the two points*/
				obj->appearance[i][j].m = -abs(obj->appearance[i][j].m);
				j += sgn2;/*either go left or right depending on which of the points is rightmost*/
			}while(j != tri.point[l][X]);/*continue until we reach point B (l)*/
		} else 
		if(sgn_int(sgn2) == 0) {/*they have the same X coordinate*/
			i = tri.point[p][Y];
			j = tri.point[p][X];
			sgn1 = sgn_int(sgn1);
			do{
				obj->appearance[i][j].m = -abs(obj->appearance[i][j].m);
				i += sgn1;
			}while(i != tri.point[l][Y]);
		} else 
		if(abs(sgn1) > abs(sgn2)){/*if obj1 != 0 and obj2 == 0 impossible because of previus condition?*/
			i=tri.point[p][0];
			j=tri.point[p][1];
			k=0;
			do{
				obj->appearance[i][j].m=-abs(obj->appearance[i][j].m);
				i+=sgn_int(sgn1);
				k+=abs(sgn2);
				if((abs(k)<<1)>abs(sgn1)){
					j+=sgn_int(sgn2);
					k-=abs(sgn1);
				}
			}while((i!=tri.point[l][0])||(j!=tri.point[l][1]));
			obj->appearance[i][j].m=-abs(obj->appearance[i][j].m);
		} else {/*in case both coordinates of the points are different*/
			i = tri.point[p][Y];
			j = tri.point[p][X];
			k = 0;
			do{
				obj->appearance[i][j].m = -abs(obj->appearance[i][j].m);
				j += sgn_int(sgn2);
				k += abs(sgn1);
				if((abs(k)<<1) > abs(sgn2)){
					i += sgn_int(sgn1);
					k -= abs(sgn2);
				}
			}while((i != tri.point[l][0]) || (j != tri.point[l][1]));
			obj->appearance[i][j].m = -abs(obj->appearance[i][j].m);
		}
	}
	for(i=minx;i<=maxx;i++){
		k=miny;
		while(obj->appearance[i][k].m >= 0) k++;/*checking which side the triangle is on???*/
		l=maxy;
		while(obj->appearance[i][l].m >= 0) l--;
		for(j=k;j<=l;j++){/*finally copying the pixel values*/
			obj->appearance[i][j].m = 1+abs(obj->appearance[i][j].m); 
			obj->appearance[i][j].R += tri.R;
			obj->appearance[i][j].G += tri.G;
			obj->appearance[i][j].B += tri.B;
		}
	}
	return;
}

void delete_triangle(Amoeba *obj, int gen)/*removes triangle*/
{
	int i, j, k, l, p, maxx, minx, maxy, miny, sgn1, sgn2;
	Triangle tri;
	tri=obj->gene[gen];
	maxx=0;
	minx=height;
	maxy=0;
	miny=width;
	for(i=0;i<3;i++){
		if(tri.point[i][0]>maxx) maxx=tri.point[i][0];
		if(tri.point[i][0]<minx) minx=tri.point[i][0];
		if(tri.point[i][1]>maxy) maxy=tri.point[i][1];
		if(tri.point[i][1]<miny) miny=tri.point[i][1];
	}
	for(p=0;p<3;p++){
		l=p+1;
		if(l==3) l=0;
		sgn1=tri.point[l][0]-tri.point[p][0];
		sgn2=tri.point[l][1]-tri.point[p][1];
		if(sgn_int(sgn1)==0){
			i=tri.point[p][0];
			j=tri.point[p][1];
			sgn2=sgn_int(sgn2);
			do{
				obj->appearance[i][j].m=-abs(obj->appearance[i][j].m);
				j+=sgn2;
			}while(j!=tri.point[l][1]);
		}
		else if(sgn_int(sgn2)==0){
			i=tri.point[p][0];
			j=tri.point[p][1];
			sgn1=sgn_int(sgn1);
			do{
				obj->appearance[i][j].m=-abs(obj->appearance[i][j].m);
				i+=sgn1;
			}while(i!=tri.point[l][0]);
		}
		else if(abs(sgn1)>abs(sgn2)){
			i=tri.point[p][0];
			j=tri.point[p][1];
			k=0;
			do{
				obj->appearance[i][j].m=-abs(obj->appearance[i][j].m);
				i+=sgn_int(sgn1);
				k+=abs(sgn2);
				if((abs(k)<<1)>abs(sgn1)){
					j+=sgn_int(sgn2);
					k-=abs(sgn1);
				}
			}while((i!=tri.point[l][0])||(j!=tri.point[l][1]));
			obj->appearance[i][j].m=-abs(obj->appearance[i][j].m);
		}
		else{
			i=tri.point[p][0];
			j=tri.point[p][1];
			k=0;
			do{
				obj->appearance[i][j].m=-abs(obj->appearance[i][j].m);
				j+=sgn_int(sgn2);
				k+=abs(sgn1);
				if((abs(k)<<1)>abs(sgn2)){
					i+=sgn_int(sgn1);
					k-=abs(sgn2);
				}
			}while((i!=tri.point[l][0])||(j!=tri.point[l][1]));
			obj->appearance[i][j].m=-abs(obj->appearance[i][j].m);
		}
	}
	for(i=minx;i<=maxx;i++){
		k=miny;
		while(obj->appearance[i][k].m>=0) k++;
		l=maxy;
		while(obj->appearance[i][l].m>=0) l--;
		for(j=k;j<=l;j++){
			obj->appearance[i][j].m=abs(obj->appearance[i][j].m)-1; 
			obj->appearance[i][j].R-=tri.R;
			obj->appearance[i][j].G-=tri.G;
			obj->appearance[i][j].B-=tri.B;
		}
	}
	return;
}

void evaluate(Amoeba *obj)
{
	int i, j, s, m1;
	s=0;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			m1=obj->appearance[i][j].m;
			/*if (m1 != 1) printf("!= 1\n");
			else printf("== 1\n");*/
			/*distance between picture and current pixel value gives a score*/
			s 	+= 	abs((obj->appearance[i][j].R)/m1	-pic[i][j].R)
				+	abs((obj->appearance[i][j].G)/m1	-pic[i][j].G)
				+	abs((obj->appearance[i][j].B)/m1	-pic[i][j].B);
		}
	}
	obj->evaluation = s;
	return;
}

void init_pool(){
	int i, j, k, l;
	srand(time(NULL));

	for(i=0;i<Population;i++){
		for(j=0;j<height;j++){
			for(k=0;k<width;k++){
				pool[i].appearance[j][k] = pixel_average;
			}
		}
		for(j=0;j<GeneNum;j++){
			for(k=0;k<3;k++){
				pool[i].gene[j].point[k][0] = rand()%height;/*picking a random point to place the triangle*/
				pool[i].gene[j].point[k][1] = rand()%width;
			}
			pool[i].gene[j].R = rand()&255;/*same as % 256*/
			pool[i].gene[j].G = rand()&255;/*picking random colors*/
			pool[i].gene[j].B = rand()&255;
			cover_triangle(&pool[i],j);
		}
		evaluate(&pool[i]);
	}
	return;
}

void print_best(){
	int i, j, k, l, r, g, b, x;
	FILE* fio;
	fio=fopen("result.ppm","w");
	fprintf(fio,"P3\n");
	fprintf(fio, "%d %d\n", height, width);
	fprintf(fio, "255\n");
	/*fprintf(fio,"%d %d %d %d %d\n",height,width,GeneNum,iternum,best_now.evaluation);

	fprintf(fio,"%d %d %d\n",pixel_average.R,pixel_average.G,pixel_average.B);
	for(i=0;i<GeneNum;i++){
		fprintf(fio,"%d %d %d %d %d %d %d %d %d\n", best_now.gene[i].point[0][0],best_now.gene[i].point[0][1],best_now.gene[i].point[1][0],best_now.gene[i].point[1][1],best_now.gene[i].point[2][0],best_now.gene[i].point[2][1],best_now.gene[i].R,best_now.gene[i].G,best_now.gene[i].B);
	}
	*/
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			r=best_now.appearance[i][j].R;
			x=(best_now.appearance[i][j].m);
			r/=x;
			g=best_now.appearance[i][j].G;
			g/=x;
			b=best_now.appearance[i][j].B;
			b/=x;
			fprintf(fio,"%d %d %d\n",r,g,b);
		}
	}
	fclose(fio);
	return;
}

void print_info(){
	printf("Generation: %d. Best evaluation: %d\n", iternum, best_now.evaluation);
	return;
}

void iterate_generation(){
	int i, j, k, l;
	int tri[Population][2];
	/* sort by evaluation */
	for(i=0;i<Population;i++){
		tri[i][0]=pool[i].evaluation;
		tri[i][1]=i;
	}
	for(i=0;i<Population;i++){
		for(j=0;j<Population-1;j++){
			if(tri[j][0]>tri[j+1][0]){
				l=tri[j][0];
				tri[j][0]=tri[j+1][0];
				tri[j+1][0]=l;
				l=tri[j][1];
				tri[j][1]=tri[j+1][1];
				tri[j+1][1]=l;
			}
		}
	}
	/* Update Best Amoeba */
	if(best_now.evaluation>pool[tri[0][1]].evaluation) best_now=pool[tri[0][1]];
	/* Replace "failure" with "success", with commutation */
	for(i=0;i<Population/3;i++){
		pool[tri[Population-1-i][1]]=pool[tri[i][1]];
		j=rand()%(Population/3);
		j+=Population/3;
		for(k=0;k<CommutateNum;k++){
			l=rand()%GeneNum;
			delete_triangle(&pool[tri[Population-1-i][1]],l);
			pool[tri[Population-1-i][1]].gene[l]=pool[j].gene[l];
			cover_triangle(&pool[tri[Population-1-i][1]],l);
		}
		evaluate(&pool[tri[Population-1-i][1]]);
	}
	/* Mutation */
	for(i=0;i<Population;i++){
		l=0;
		for(j=0;j<GeneNum;j++){
			for(k=0;k<9;k++){
				if((rand()&Mutation_Rate_Mask)==14){
					delete_triangle(&pool[i],j);
					if(k<6){
						if((k&1)==0){
							pool[i].gene[j].point[(k>>1)%3][0]+=(rand()%(height>>1))-(height>>2);
							if(pool[i].gene[j].point[(k>>1)%3][0]<0) pool[i].gene[j].point[(k>>1)%3][0]+=height;
							if(pool[i].gene[j].point[(k>>1)%3][0]>=height) pool[i].gene[j].point[(k>>1)%3][0]-=height;
						}
						else{
							pool[i].gene[j].point[(k>>1)%3][1]+=(rand()%(width>>1))-(width>>2);
							if(pool[i].gene[j].point[(k>>1)%3][1]<0) pool[i].gene[j].point[(k>>1)%3][1]+=width;
							if(pool[i].gene[j].point[(k>>1)%3][1]>=width) pool[i].gene[j].point[(k>>1)%3][1]-=width;
						}
					}
					else{
						if(k==6){
							pool[i].gene[j].R+=rand()%255;
							pool[i].gene[j].R&=255;
						}
						else if(k==7){
							pool[i].gene[j].G+=rand()%255;
							pool[i].gene[j].G&=255;
						}
						else{
							pool[i].gene[j].B+=rand()%255;
							pool[i].gene[j].B&=255;
						}
					}
					cover_triangle(&pool[i],j);
					l=1;
				}
			}
		}
		if(l==1) evaluate(&pool[i]);
	}
	iternum++;
	return;
}

int main(){
	int i;
	char c;
	MLV_create_window("TP7", "TP7", 600, 600);
	MLV_Image *im = MLV_load_image("geometric.ppm");
	MLV_draw_image(im,0,0);
	MLV_actualise_window();
	MLV_wait_seconds(1);

	init_pic();
	init_pool();
	best_now = pool[0];
	iternum=0;

	scanf("%d",&i);
	while(i>=iternum) iterate_generation();
	print_info();
	print_best();
	
	MLV_actualise_window();
	MLV_Image *im2 = MLV_load_image("result.ppm");
	MLV_draw_image(im2, 300, 300);
	MLV_actualise_window();
	MLV_wait_seconds(20);


	MLV_free_image(im);
	MLV_free_image(im2);
	MLV_free_window();

	return 0;
}