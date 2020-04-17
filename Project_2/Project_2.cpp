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

// Setting the number of nodes per side in the simulation:
#ifndef NUMNODES
#define NUMNODES	1000
#endif

// How many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES	10
#endif

// Define the exponent of the n-sphere
#define N 4

// Define the dimensions of the superquadratic
#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

// Provided function for calculating the height at a given point (iu, iv)
float Height(int, int);


int main(int argc, char* argv[])
{
#ifndef _OPENMP
    fprintf(stderr, "No OpenMP support!\n");
    return 1;
#endif

    // Set the number of threads to use in the for-loop
    omp_set_num_threads(NUMT);

    // Get ready to record the maximum performance
    float maxPerformance = 0.;      // Must be declared outside the NUMTRIES loop
    float volume;              // must be declared outside the NUMTRIES loop

    // The area of a single full-sized tile:
    float fullTileArea = (((XMAX - XMIN) / (float)(NUMNODES - 1)) *
        ((YMAX - YMIN) / (float)(NUMNODES - 1)));

    // Sum up the weighted heights into the variable "volume"
    // using an OpenMP for loop and a reduction:
    for (int i = 0; i < NUMTRIES; i++)
    {
        volume = 0.;

        // Start the calculation timer
        double time0 = omp_get_wtime();

        #pragma omp parallel for default(none) reduction(+:volume)
        for (int i = 0; i < NUMNODES * NUMNODES; i++)
        {
            int iu = i % NUMNODES;
            int iv = i / NUMNODES;
            float z = Height(iu, iv);

            // Calculate area from current height sample
            double tile_area = (z * 2) * fullTileArea;

            // Add current value to volume with 1/4 for corners and 1/2 for edges
            if (iu == 0 || iu == NUMNODES - 1)
            {
                tile_area /= 2;
            }
            if (iv == 0 || iv == NUMNODES - 1)
            {
                tile_area /= 2;
            }

            // Add to the total volume
            volume += tile_area;
        }

        // Stop timer
        double time1 = omp_get_wtime();

        // Calculate the operations / sec of this run
        double megaHeightsPerSecond = ((double)NUMNODES * (double)NUMNODES)
            / (time1 - time0) / 1000000.;

        // Prevent a divide by 0 infinite result from poluting the data
        if (isfinite(megaHeightsPerSecond))
        {
            if (megaHeightsPerSecond > maxPerformance)
                maxPerformance = megaHeightsPerSecond;
        }
    }

    printf("Volume: %lf\n", volume);

    // Print out the maximum performance value of the trials set 
    printf("%.2f\t", maxPerformance);

    return 0;
}


float Height(int iu, int iv)	// iu,iv = 0 .. NUMNODES-1
{
    float x = -1. + 2. * (float)iu / (float)(NUMNODES - 1);	// -1. to +1.
    float y = -1. + 2. * (float)iv / (float)(NUMNODES - 1);	// -1. to +1.

    float xn = pow(fabs(x), (double)N);
    float yn = pow(fabs(y), (double)N);
    float r = 1. - xn - yn;
    if (r < 0.)
        return 0.;
    float height = pow(1. - xn - yn, 1. / (float)N);
    return height;
}
