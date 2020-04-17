/******************************************************************************
**  Author:       Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         4-16-2020
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

// Define the exponent of the n-sphere
#define N 4

// Define the 
#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

// Provided function for calculating the height at a given point (iu, iv)
float Height( int, int );


int main( int argc, char *argv[ ] )
{
    #ifndef _OPENMP
		fprintf(stderr, "No OpenMP support!\n");
		return 1;
	#endif


	omp_set_num_threads(NUMT);	// set the number of threads to use in the for-loop:`

	// get ready to record the maximum performance and the probability:
	float maxPerformance = 0.;      // must be declared outside the NUMTRIES loop
	float currentProb = 0.;              // must be declared outside the NUMTRIES loop

	// The area of a single full-sized tile:
	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

	/////////////////////////

    for (int i = 0; i < NUMTRIES; i++)
    {
        double time0 = omp_get_wtime();

        #pragma omp parallel for default(none) . . .
        for( int i = 0; i < NUMNODES*NUMNODES; i++ )
        {
            int iu = i % NUMNODES;
            int iv = i / NUMNODES;
            float z = Height( iu, iv );

            ////////////////////////
        }


        double time1 = omp_get_wtime();
		double megaHeightsPerSecond = (double)NUMNODES / (time1 - time0) / 1000000.;

        // Prevent a divide by 0 infinite result from poluting the data
        if (isfinite(megaHeightsPerSecond))
        {  
            if (megaHeightsPerSecond > maxPerformance)
                maxPerformance = megaHeightsPerSecond;
        }
    }

    // Print out the maximum performance value of the trials set 
    printf("%.2f\t", maxPerformance);

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