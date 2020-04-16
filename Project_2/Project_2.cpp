/******************************************************************************
**  Author:       Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         4-6-2020
**  Description:  Program 2 for OSU cs-475 Parallel Programming. The program
**                uses OpenMP and is a test of the performance of 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define _USE_MATH_DEFINES
#include <math.h>

// Setting the number of threads:
#ifndef NUMT
#define NUMT		1
#endif

// Setting the number of trials in the simulation:
#ifndef NUMNODES
#define NUMNODES	100
#endif

// How many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES	10
#endif

// Define the 
#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

float Height( int, int );

int main( int argc, char *argv[ ] )
{
    #ifndef _OPENMP
		fprintf(stderr, "No OpenMP support!\n");
		return 1;
	#endif

	//////////////////////////

	// the area of a single full-sized tile:

	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

	/////////////////////////



    return 0;
}


float Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r < 0. )
	        return 0.;
	float height = pow( 1. - xn - yn, 1./(float)N );
	return height;
}