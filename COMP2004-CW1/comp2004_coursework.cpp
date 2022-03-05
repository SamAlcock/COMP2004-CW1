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
    for (x = 0; x < N; x++)
        for (y = 0; y < M; y++)
            if (y >= C0 && y <= M - 1 - C0 && x >= C0 && x <= N - 1 - C0) {
                x_compute[x][y][0] = 0;

                x_compute[x][y][C0] = x_compute[x][y][C0 - 1] + input[x - 1][y] * Filter[1];
                x_compute[x][y][1 + C0] = x_compute[x][y][C0] + input[x][y] * Filter[0];
                x_compute[x][y][2 + C0] = x_compute[x][y][C0 + 1] + input[x + 1][y] * Filter[1];

                x_image[x][y] = x_compute[x][y][(2 * C0) + 1] / total;
            }
            else //this is for image border pixels only
                x_image[x][y] = 0;

    for (x = 0; x < N; x++)
        for (y = 0; y < M; y++)
            if (y >= C0 && y <= M - 1 - C0 && x >= C0 && x <= N - 1 - C0) {
                xy_compute[x][y][0] = 0;
                
                xy_compute[x][y][C0] = xy_compute[x][y][C0 - 1] + x_image[x][y - 1] * Filter[1];
                xy_compute[x][y][C0 + 1] = xy_compute[x][y][C0] + x_image[x][y] * Filter[0];
                xy_compute[x][y][C0 + 2] = xy_compute[x][y][C0 + 1] + x_image[x][y + 1] * Filter[1];

                xy_image[x][y] = xy_compute[x][y][(2 * C0) + 1] / total;
            }
            else
                xy_image[x][y] = 0;

    /*  Part B */
    for (x = 0; x < N; x++)
        for (y = 0; y < M; y++)
            if (x >= C0 && x <= N - 1 - C0 && y >= C0 && y <= M - 1 - C0) {
                char image = xy_image[x][y];
                diff_compute[x][y][0] = 0;
                diff_compute[x][y][1] = maximum(abs(xy_image[x + x_offset[0]][y + y_offset[0]] - image), diff_compute[x][y][0]);
                diff_compute[x][y][2] = maximum(abs(xy_image[x + x_offset[1]][y + y_offset[1]] - image), diff_compute[x][y][1]);
                diff_compute[x][y][3] = maximum(abs(xy_image[x + x_offset[2]][y + y_offset[2]] - image), diff_compute[x][y][2]);
                diff_compute[x][y][4] = maximum(abs(xy_image[x + x_offset[3]][y + y_offset[3]] - image), diff_compute[x][y][3]);
                diff_compute[x][y][5] = maximum(abs(xy_image[x + x_offset[4]][y + y_offset[4]] - image), diff_compute[x][y][4]);
                diff_compute[x][y][6] = maximum(abs(xy_image[x + x_offset[5]][y + y_offset[5]] - image), diff_compute[x][y][5]);
                diff_compute[x][y][7] = maximum(abs(xy_image[x + x_offset[6]][y + y_offset[6]] - image), diff_compute[x][y][6]);
                diff_compute[x][y][8] = maximum(abs(xy_image[x + x_offset[7]][y + y_offset[7]] - image), diff_compute[x][y][7]);

                edge_image[x][y] = diff_compute[x][y][C1];
            }
            else
                edge_image[x][y] = 0;

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

  total += Filter[1];
  total += Filter[0];
  total += Filter[1];
  
  for (x=0; x<N; x++)
    for (y=0; y<M; y++)
     if (x>=C0 && x<=N-1-C0 && y>=C0 && y<=M-1-C0) {
      x_compute[x][y][0]=0;

      
        x_compute[x][y][C0] = x_compute[x][y][C0-1]
           + input[x-1][y]*Filter[1];
        x_compute[x][y][C0+1] = x_compute[x][y][C0]
           + input[x][y]*Filter[0];
        x_compute[x][y][C0+2] = x_compute[x][y][C0+1]
           + input[x+1][y]*Filter[1];   
        
      x_image[x][y]= x_compute[x][y][(2*C0)+1]/total;
      }
     else
      x_image[x][y] = 0;

  for (x=0; x<N; x++)
    for (y=0; y<M; y++)
     if (x>=C0 && x<=N-1-C0 && y>=C0 && y<=M-1-C0) {
      xy_compute[x][y][0]=0;

        xy_compute[x][y][C0] = xy_compute[x][y][C0-1] +
            x_image[x][y-1]*Filter[1];
        xy_compute[x][y][C0+1] = xy_compute[x][y][C0] +
            x_image[x][y]*Filter[0];
        xy_compute[x][y][C0+2] = xy_compute[x][y][C0+1] +
            x_image[x][y+1]*Filter[1];
            
      xy_image[x][y]= xy_compute[x][y][(2*C0)+1]/total;
      }
     else
      xy_image[x][y] = 0;


 /*  ComputeEdges(g_image, c_image); */
  for (x=0; x<N; x++)
    for (y=0; y<M; y++)
     if (x>=C0 && x<=N-1-C0 && y>=C0 && y<=M-1-C0) {
      diff_compute[x][y][0] = 0;
      char image = xy_image[x][y];

        diff_compute[x][y][1] =
          maximum(abs(xy_image[x+x_offset[0]][y+y_offset[0]]
                    - image), diff_compute[x][y][0]);
        diff_compute[x][y][2] =
          maximum(abs(xy_image[x+x_offset[1]][y+y_offset[1]]
                    - image), diff_compute[x][y][1]);
        diff_compute[x][y][3] =
          maximum(abs(xy_image[x+x_offset[2]][y+y_offset[2]]
                    - image), diff_compute[x][y][2]);
        diff_compute[x][y][4] =
          maximum(abs(xy_image[x+x_offset[3]][y+y_offset[3]]
                    - image), diff_compute[x][y][3]);
        diff_compute[x][y][5] =
          maximum(abs(xy_image[x+x_offset[4]][y+y_offset[4]]
                    - image), diff_compute[x][y][4]);
        diff_compute[x][y][6] =
          maximum(abs(xy_image[x+x_offset[5]][y+y_offset[5]]
                    - image), diff_compute[x][y][5]);
        diff_compute[x][y][7] =
          maximum(abs(xy_image[x+x_offset[6]][y+y_offset[6]]
                    - image), diff_compute[x][y][6]);
        diff_compute[x][y][8] =
          maximum(abs(xy_image[x+x_offset[7]][y+y_offset[7]]
                    - image), diff_compute[x][y][7]);             
      edge_image[x][y] = diff_compute[x][y][C1];
      }
     else
      edge_image[x][y] = 0;
  
  /*  DetectRoots(c_image, out_image); */
  for (x=0; x<N; x++)
    for (y=0; y<M; y++)
     if (x>=C0 && x<=N-1-C0 && y>=C0 && y<=M-1-C0) {
        out_compute = 255;
        char edge_img = edge_image[x][y]; 
          if (edge_image[x+x_offset[0]][y+y_offset[0]] <
              edge_img) out_compute = 0;
          if (edge_image[x+x_offset[1]][y+y_offset[1]] <
              edge_img) out_compute = 0;
          if (edge_image[x+x_offset[2]][y+y_offset[2]] <
              edge_img) out_compute = 0;
          if (edge_image[x+x_offset[3]][y+y_offset[3]] <
              edge_img) out_compute = 0;
          if (edge_image[x+x_offset[4]][y+y_offset[4]] <
              edge_img) out_compute = 0;
          if (edge_image[x+x_offset[5]][y+y_offset[5]] <
              edge_img) out_compute = 0;
          if (edge_image[x+x_offset[6]][y+y_offset[6]] <
              edge_img) out_compute = 0;
          if (edge_image[x+x_offset[7]][y+y_offset[7]] <
              edge_img) out_compute = 0;
        if (output[x][y] != out_compute)
					return false;
        }
      else
          if (output[x][y] != 0)
					  return false;
		
					
					return true;
	}
	
