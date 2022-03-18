/*
------------------DR VASILIOS KELEFOURAS-----------------------------------------------------
------------------COMP2004 ------------------------------------------------------------------
------------------EMBEDDED PROGRAMMING AND THE INTERNET OF THINGS-------------------------------------------------
------------------UNIVERSITY OF PLYMOUTH, SCHOOL OF ENGINEERING, COMPUTING AND MATHEMATICS---
*/

// --------------- COMP2004 Coursework - Set of Exercises -------------------

/*
In this element of assessment you need to reduce the execution time of the ineffcient_routine(). 
This is an image processing routine written in an inefficient way.
First, read the code and understand what it does. Then, try to simplify the code (reduce its complexity), by eliminating redundant operations. Last, apply code optimizations such as scalar replacement, register blocking, loop merge and others. 

*/

//Initial time Debug- 21ms
//Initial time Release - 19ms

//Methods used: Loop unrolling, scalar replacement, strength reduction

#include "mbed.h"
#include <cstdio>


using namespace std::chrono;


#include "string.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


#define N 80 //image size 
#define M 80 //image size 

void inefficient_routine();
void initialization();
bool compare_images();


//the following variables never change their values. You could take this into advantage
const int C0=1;
const int C1=8;
const int x_offset[C1]={1,1,1,0,0,-1,-1,-1};
const int y_offset[C1]={1,0,-1,1,-1,1,0,-1};
const int Filter[]={99,68,35,10};

unsigned char input[N][M];
unsigned char output[N][M];
unsigned char x_image[N][M];
unsigned char xy_image[N][M];
unsigned char edge_image[N][M];
unsigned char diff_compute[N][M][C1+1];
unsigned short x_compute[N][M][(2 * C0) + 2];
unsigned short xy_compute[N][M][(2*C0)+2];
	
unsigned char maximum(unsigned char a, unsigned char b);





Timer timer;

int main() {
  

  printf("\n\r Programmed started \n\r");
	initialization();
	
	timer.start();
    
//YOU WILL OPTIMIZE THE FOLLOWING function
  inefficient_routine();
//------------------------------------------------------------------------------
	
 timer.stop();
printf("\nThe time taken was %llu msecs\n",duration_cast<milliseconds>(timer.elapsed_time()).count());

	bool outcome=compare_images();
	
	if (outcome==true)
		printf("\n\n\r -----  output is correct -----\n\r");
	else 
		printf("\n\n\r ----- output is INcorrect -----\n\r");

	return 0;
}

void initialization(){
	int i,j;
	
	for (i=0;i<N;i++)
	 for (j=0;j<M;j++)
	  input[i][j]=rand()%255;
	
	for (i=0;i<N;i++)
	   for (j=0;j<M;j++)
	    output[i][j]=0;
	
}


unsigned char maximum(unsigned char a, unsigned char b)
{
    return a > b ? a : b;
}


void inefficient_routine()
{
    unsigned char out_compute;
    int x, y, k;
    const unsigned short int total = 235;

    /*  Part A */
    for (x = 0; x < 80; x++)
        for (y = 0; y < 80; y++)
            if (y >= 1 && y <= 78 && x >= 1 && x <= 78) {
                x_compute[x][y][0] = 0;

                x_compute[x][y][1] = x_compute[x][y][0] + input[x - 1][y] * 68;
                x_compute[x][y][2] = x_compute[x][y][1] + input[x][y] * 99;
                x_compute[x][y][3] = x_compute[x][y][2] + input[x + 1][y] * 68;

                x_image[x][y] = x_compute[x][y][3] / total;
            }
            else //this is for image border pixels only
                x_image[x][y] = 0;

    for (x = 0; x < 80; x++)
        for (y = 0; y < 80; y++)
            if (y >= 1 && y <= 78 && x >= 1 && x <= 78) {
                xy_compute[x][y][0] = 0;
                
                xy_compute[x][y][1] = xy_compute[x][y][0] + x_image[x][y - 1] * 68;
                xy_compute[x][y][2] = xy_compute[x][y][1] + x_image[x][y] * 99;
                xy_compute[x][y][3] = xy_compute[x][y][2] + x_image[x][y + 1] * 68;

                xy_image[x][y] = xy_compute[x][y][3] / total;
            }
            else
                xy_image[x][y] = 0;

    /*  Part B */
    unsigned char image;
    unsigned char edge;
    for (x = 0; x < 80; x++)
        for (y = 0; y < 80; y+=4)
            if (x != 0 || y != 0 || x != 79 || y != 79){
                image = xy_image[x][y];
                edge = edge_image[x][y];
                
                edge = 0;
                edge = maximum(abs(xy_image[x + 1][y + 1] - image), edge);
                edge = maximum(abs(xy_image[x + 1][y] - image), edge);
                edge = maximum(abs(xy_image[x + 1][y - 1] - image), edge);
                edge = maximum(abs(xy_image[x][y + 1] - image), edge);
                edge = maximum(abs(xy_image[x][y - 1] - image), edge);
                edge = maximum(abs(xy_image[x - 1][y + 1] - image), edge);
                edge = maximum(abs(xy_image[x - 1][y] - image), edge);
                edge_image[x][y] = maximum(abs(xy_image[x - 1][y - 1] - image), edge);

                image = xy_image[x][y + 1];
                edge = edge_image[x][y + 1];
                
                edge = 0;
                edge = maximum(abs(xy_image[x + 1][y + 2] - image), edge);
                edge = maximum(abs(xy_image[x + 1][y + 1] - image), edge);
                edge = maximum(abs(xy_image[x + 1][y] - image), edge);
                edge = maximum(abs(xy_image[x][y + 2] - image), edge);
                edge = maximum(abs(xy_image[x][y] - image), edge);
                edge = maximum(abs(xy_image[x - 1][y + 2] - image), edge);
                edge = maximum(abs(xy_image[x - 1][y + 1] - image), edge);
                edge_image[x][y + 1] = maximum(abs(xy_image[x - 1][y] - image), edge);
                
                image = xy_image[x][y + 2];
                edge = edge_image[x][y + 2];
                
                edge = 0;
                edge = maximum(abs(xy_image[x + 1][y + 3] - image), edge);
                edge = maximum(abs(xy_image[x + 1][y + 2] - image), edge);
                edge = maximum(abs(xy_image[x + 1][y + 1] - image), edge);
                edge = maximum(abs(xy_image[x][y + 3] - image), edge);
                edge = maximum(abs(xy_image[x][y + 1] - image), edge);
                edge = maximum(abs(xy_image[x - 1][y + 3] - image), edge);
                edge = maximum(abs(xy_image[x - 1][y + 2] - image), edge);
                edge_image[x][y + 2] = maximum(abs(xy_image[x - 1][y + 1] - image), edge);
                
                image = xy_image[x][y + 3];
                edge = edge_image[x][y + 3];
                
                edge = 0;
                edge = maximum(abs(xy_image[x + 1][y + 4] - image), edge);
                edge = maximum(abs(xy_image[x + 1][y + 3] - image), edge);
                edge = maximum(abs(xy_image[x + 1][y + 2] - image), edge);
                edge = maximum(abs(xy_image[x][y + 4] - image), edge);
                edge = maximum(abs(xy_image[x][y + 2] - image), edge);
                edge = maximum(abs(xy_image[x - 1][y + 4] - image), edge);
                edge = maximum(abs(xy_image[x - 1][y + 3] - image), edge);
                edge_image[x][y + 3] = maximum(abs(xy_image[x - 1][y + 2] - image), edge);



            }
            
            
            
            
    /* Part C */
    for (x = 0; x < N; x++)
        for (y = 0; y < M; y++)
            if (x >= C0 && x <= N - 1 - C0 && y >= C0 && y <= M - 1 - C0) {
                out_compute = 255;
                k = 0;
                //C1 = 8
                if (edge_image[x + x_offset[0]][y + y_offset[0]] < edge_image[x][y] || edge_image[x + x_offset[1]][y + y_offset[1]] < edge_image[x][y] || edge_image[x + x_offset[2]][y + y_offset[2]] < edge_image[x][y] || edge_image[x + x_offset[3]][y + y_offset[3]] < edge_image[x][y] || edge_image[x + x_offset[4]][y + y_offset[4]] < edge_image[x][y] || edge_image[x + x_offset[5]][y + y_offset[5]] < edge_image[x][y] || edge_image[x + x_offset[6]][y + y_offset[6]] < edge_image[x][y] || edge_image[x + x_offset[7]][y + y_offset[7]] < edge_image[x][y])  
                    out_compute = 0;

                output[x][y] = out_compute;
            }
            else
                output[x][y] = 0;
}


//returns false/true, when the output image is incorrect/correct, respectively
bool compare_images(){
	
    unsigned char out_compute;
  int x,y,k;
  unsigned short total=0;


/* start layer 2 code */

  /*  GaussBlur(in_image, g_image); */
  for (k=-C0; k<=C0; ++k)  total += Filter[abs(k)];

  for (x=0; x<N; x++)
    for (y=0; y<M; y++)
     if (x>=C0 && x<=N-1-C0 && y>=C0 && y<=M-1-C0) {
      x_compute[x][y][0]=0;
      for (k=-C0; k<=C0; ++k)
        x_compute[x][y][C0+k+1] = x_compute[x][y][C0+k]
           + input[x+k][y]*Filter[abs(k)];
      x_image[x][y]= x_compute[x][y][(2*C0)+1]/total;
      }
     else
      x_image[x][y] = 0;

  for (x=0; x<N; x++)
    for (y=0; y<M; y++)
     if (x>=C0 && x<=N-1-C0 && y>=C0 && y<=M-1-C0) {
      xy_compute[x][y][0]=0;
      for (k=-C0; k<=C0; ++k)
        xy_compute[x][y][C0+k+1] = xy_compute[x][y][C0+k] +
           x_image[x][y+k]*Filter[abs(k)];
      xy_image[x][y]= xy_compute[x][y][(2*C0)+1]/total;
      }
     else
      xy_image[x][y] = 0;


 /*  ComputeEdges(g_image, c_image); */
  for (x=0; x<N; x++)
    for (y=0; y<M; y++)
     if (x>=C0 && x<=N-1-C0 && y>=C0 && y<=M-1-C0) {
      diff_compute[x][y][0] = 0;
      for (k=0; k<=C1-1; ++k)
        diff_compute[x][y][k+1] =
          maximum(abs(xy_image[x+x_offset[k]][y+y_offset[k]]
                    - xy_image[x][y]), diff_compute[x][y][k]);
      edge_image[x][y] = diff_compute[x][y][C1];
      }
     else
      edge_image[x][y] = 0;
  
  /*  DetectRoots(c_image, out_image); */
  for (x=0; x<N; x++)
    for (y=0; y<M; y++)
     if (x>=C0 && x<=N-1-C0 && y>=C0 && y<=M-1-C0) {
        out_compute = 255; 
        k = 0;
        while ((out_compute == 255) && (k <= C1-1)) {
          if (edge_image[x+x_offset[k]][y+y_offset[k]] <
              edge_image[x][y]) out_compute = 0;
          ++k; }
        if (output[x][y] != out_compute)
					return false;
        }
     else
          if (output[x][y] != 0)
					  return false;
		
					
					return true;

	}
	
