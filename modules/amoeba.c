
#include "types.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void evaluate(Amoeba *amoeba, Pixel *pic, int height, int width)
{
    int i, j, m1;
    int r, g, b;
    int s = 0;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            m1 = amoeba->appearance[i][j].m;
            /*distance between picture and current pixel value gives a score*/
            /*color pixel in the amoeba / its transparency - pixel in the picture*/
            r = abs(((amoeba->appearance[i]+j)->R) / m1 - (pic+ i * width +j)->R);
            g = abs(((amoeba->appearance[i]+j)->G) / m1 - ((pic+ i * width +j))->G);
            b = abs(((amoeba->appearance[i]+j)->B) / m1 - ((pic+ i * width +j))->B);
            s += r + g + b;
        }
    }
    amoeba->evaluation = s;
}

void cover_triangle(Amoeba *amoeba, int gen, int height, int width)/*draws triangle according to the picture*/
{
    int i, j, k, l, p, maxx, minx, maxy, miny, sgn1, sgn2;
    Triangle tri = amoeba->gene[gen];

    maxx = 0; /*I think names should be swapped but until I can be sure I'm leaving it like this*/
    minx = height;
    maxy = 0;
    miny = width;


    for (i = 0; i < 3; i++)
    { /*here we look for the point most to the left/right/up/down*/
        if (tri.point[i][Y] > maxx)
            maxx = tri.point[i][Y];

        if (tri.point[i][Y] < minx)
            minx = tri.point[i][Y];

        if (tri.point[i][X] > maxy)
            maxy = tri.point[i][X];

        if (tri.point[i][X] < miny)
            miny = tri.point[i][X];
    }

    for (p = 0; p < 3; p++)
    {
        l = (p + 1) % 3;

        sgn1 = tri.point[l][Y] - tri.point[p][Y]; /*analyze one of the points with the other ones (0,1) (1,2) (2,0)*/
        sgn2 = tri.point[l][X] - tri.point[p][X]; /*sgn1 Y sgn2 X ?*/

        if (sgn_int(sgn1) == 0)
        {                        /*the two points have the same Y coordinate*/
            i = tri.point[p][Y]; /*point A is p*/
            j = tri.point[p][X];
            sgn2 = sgn_int(sgn2); /*which point has the biggest X coordinate ? +1->l    -1->p*/
            do
            { /*change the values for those specific pixels included between the two points*/
                amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
                j += sgn2;                  /*either go left or right depending on which of the points is rightmost*/
            } while (j != tri.point[l][X]); /*continue until we reach point B (l)*/
        }
        else if (sgn_int(sgn2) == 0)
        { /*they have the same X coordinate*/
            i = tri.point[p][Y];
            j = tri.point[p][X];
            sgn1 = sgn_int(sgn1);
            do
            {
                amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
                i += sgn1;
            } while (i != tri.point[l][Y]);
        }
        else if (abs(sgn1) > abs(sgn2))
        { /*if obj1 != 0 and obj2 == 0 impossible because of previus condition?*/
            i = tri.point[p][0];
            j = tri.point[p][1];
            k = 0;
            do
            {
                amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
                i += sgn_int(sgn1);
                k += abs(sgn2);
                if ((abs(k) << 1) > abs(sgn1))
                {
                    j += sgn_int(sgn2);
                    k -= abs(sgn1);
                }
            } while ((i != tri.point[l][0]) || (j != tri.point[l][1]));
            amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
        }
        else
        { /*in case both coordinates of the points are different*/
            i = tri.point[p][Y];
            j = tri.point[p][X];
            k = 0;
            do
            {
                amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
                j += sgn_int(sgn2);
                k += abs(sgn1);
                if ((abs(k) << 1) > abs(sgn2))
                {
                    i += sgn_int(sgn1);
                    k -= abs(sgn2);
                }
            } while ((i != tri.point[l][0]) || (j != tri.point[l][1]));
            amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
        }
    }
    for (i = minx; i <= maxx; i++)
    {
        k = miny;

        while (amoeba->appearance[i][k].m >= 0)
            k++; 
        l = maxy;
        while (amoeba->appearance[i][l].m >= 0)
            l--;
            
        for (j = k; j <= l; j++)
        { /*finally copying the pixel values*/
            amoeba->appearance[i][j].m = 1 + abs(amoeba->appearance[i][j].m);
            amoeba->appearance[i][j].R += tri.R;
            amoeba->appearance[i][j].G += tri.G;
            amoeba->appearance[i][j].B += tri.B;
        }
    }
}

void delete_triangle(Amoeba *amoeba, int gen, int height, int width) /*removes triangle*/
{
    int i, j, k, l, p, maxx, minx, maxy, miny, sgn1, sgn2;
    Triangle tri;
    tri = amoeba->gene[gen];
    maxx = 0;
    minx = height;
    maxy = 0;
    miny = width;
    for (i = 0; i < 3; i++)
    {
        if (tri.point[i][0] > maxx)
            maxx = tri.point[i][0];
        if (tri.point[i][0] < minx)
            minx = tri.point[i][0];
        if (tri.point[i][1] > maxy)
            maxy = tri.point[i][1];
        if (tri.point[i][1] < miny)
            miny = tri.point[i][1];
    }
    for (p = 0; p < 3; p++)
    {
        l = (p + 1) % 3;
        sgn1 = tri.point[l][0] - tri.point[p][0];
        sgn2 = tri.point[l][1] - tri.point[p][1];
        if (sgn_int(sgn1) == 0)
        {
            i = tri.point[p][0];
            j = tri.point[p][1];
            sgn2 = sgn_int(sgn2);
            do
            {
                amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
                j += sgn2;
            } while (j != tri.point[l][1]);
        }
        else if (sgn_int(sgn2) == 0)
        {
            i = tri.point[p][0];
            j = tri.point[p][1];
            sgn1 = sgn_int(sgn1);
            do
            {
                amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
                i += sgn1;
            } while (i != tri.point[l][0]);
        }
        else if (abs(sgn1) > abs(sgn2))
        {
            i = tri.point[p][0];
            j = tri.point[p][1];
            k = 0;
            do
            {
                amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
                i += sgn_int(sgn1);
                k += abs(sgn2);
                if ((abs(k) << 1) > abs(sgn1))
                {
                    j += sgn_int(sgn2);
                    k -= abs(sgn1);
                }
            } while ((i != tri.point[l][0]) || (j != tri.point[l][1]));
            amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
        }
        else
        {
            i = tri.point[p][0];
            j = tri.point[p][1];
            k = 0;
            do
            {
                amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
                j += sgn_int(sgn2);
                k += abs(sgn1);
                if ((abs(k) << 1) > abs(sgn2))
                {
                    i += sgn_int(sgn1);
                    k -= abs(sgn2);
                }
            } while ((i != tri.point[l][0]) || (j != tri.point[l][1]));
            amoeba->appearance[i][j].m = -abs(amoeba->appearance[i][j].m);
        }
    }
    for (i = minx; i <= maxx; i++)
    {
        k = miny;
        while (amoeba->appearance[i][k].m >= 0)
            k++;
        l = maxy;
        while (amoeba->appearance[i][l].m >= 0)
            l--;
        for (j = k; j <= l; j++)
        {
            amoeba->appearance[i][j].m = abs(amoeba->appearance[i][j].m) - 1;
            amoeba->appearance[i][j].R -= tri.R;
            amoeba->appearance[i][j].G -= tri.G;
            amoeba->appearance[i][j].B -= tri.B;
        }
    }
}