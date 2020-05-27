/******************************************************************************
**  Authors:      Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         5-26-2020
**  Description:  Program 7b for OSU cs-475 Parallel Programming. The program
**                uses        
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#ifndef THREAD_COUNT
#define THREAD_COUNT    1
#endif

#ifndef NUMTRIES
#define NUMTRIES      30	// Number of iterations
#endif

#ifndef OUTPUT_FILE
#define OUTPUT_FILE     "OMP_1_thread.txt"
#endif


int main()
{
    // Print error message and exit if OpenMP isn't supported
    #ifndef _OPENMP
        fprintf(stderr, "No OpenMP support!\n");
        return 1;
    #endif

    // Open the data file to retrieve the signal values
    FILE *fp = fopen( "signal.txt", "r" );

    if ( fp == NULL )
    {
        fprintf( stderr, "Cannot open file 'signal.txt'\n" );
        exit( 1 );
    }

    // The first line of the data file is the line count
    int Size;
    fscanf( fp, "%d", &Size );

    float *A =     new float[ 2*Size ];
    float *Sums  = new float[ 1*Size ];

    // Fill A array with the data values twice to simplify wrapping around
    for( int i = 0; i < Size; i++ )
    {
        fscanf( fp, "%f", &A[i] );
        A[i+Size] = A[i];		// duplicate the array
    }

    fclose( fp );


    // Set up the data processing
    double maxGigaMults = 0.;
    fp = fopen(OUTPUT_FILE, "w");
    omp_set_num_threads(THREAD_COUNT);

    // Autocorrelate the supplied signal numtries times 

    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

        #pragma omp parallel for default(none) shared(Size, A, Sums)
        for( int shift = 0; shift < Size; shift++ )
        {
            float sum = 0.;
            for( int i = 0; i < Size; i++ )
            {
                sum += A[i] * A[i + shift];
            }
            Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
        }

        double time1 = omp_get_wtime();
        double GigaMults = (double)(Size * Size) /(time1-time0)/1000000000.;

        // Prevent infinite values from entering data
        if (isfinite(GigaMults))
        {
            if (GigaMults > maxGigaMults)
            maxGigaMults = GigaMults;
        }
    }

    // Print GigaMultsPerSecond
    printf("OMP giga-ops / sec: %4.3lf\t", maxGigaMults);

    for (int i = 1; i < 513; i++)
    {
        fprintf(fp, "%d\t%.2lf\n", i, Sums[i]);
    }

    fclose( fp );

    return 0;
}

