/******************************************************************************
**  Author:       Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         3-29-2020
**  Description:  Program 0 for OSU cs-475 Parallel Programming. The program is
**                a test of the performance of array multiplication in a for
**                loop with 1 thread vs. 4 threads. It uses OpenMP.
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMT	        4   // Thread count
#define SIZE       	16384	// Array size
#define NUMTRIES      100	// Number of iterations

float A[SIZE];
float B[SIZE];
float C[SIZE];

int main()
{
    #ifndef _OPENMP
        fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
        return 1;
    #endif

    // Inialize the arrays
    for (int i = 0; i < SIZE; i++)
    {
        A[i] = 1.;
        B[i] = 2.;
    }

    omp_set_num_threads(NUMT);
    fprintf(stderr, "Using %d threads\n", NUMT);

    double maxMegaMults = 0.;

    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

        #pragma omp parallel for
        for (int i = 0; i < SIZE; i++)
        {
            C[i] = A[i] * B[i];
        }

        double time1 = omp_get_wtime();
        double megaMults = (double)SIZE / (time1 - time0) / 1000000.;
        if (megaMults > maxMegaMults)
            maxMegaMults = megaMults;
    }

    printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);

    // NOTE: %lf stands for "long float", which is how printf prints a "double"
    //        %d stands for "decimal integer", not "double"

    return 0;
}