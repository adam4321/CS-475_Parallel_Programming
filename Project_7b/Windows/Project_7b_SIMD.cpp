/******************************************************************************
**  Authors:      Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         5-26-2020
**  Description:  Program 7b for OSU cs-475 Parallel Programming. The program
**                uses SIMD to test multi-threading an autocorrelation
**                which can find a signal wave within seemingly random data      
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <xmmintrin.h>


// Setting the number of SIMD parallel floats
#define SSE_WIDTH		4

// Setting the number of iterations
#define NUMTRIES	    100

#define THREAD_COUNT    4

float SimdMulSum( float *a, float *b, int len );


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
    omp_set_num_threads(THREAD_COUNT);
    int NUM_ELEMENTS_PER_CORE = Size / THREAD_COUNT;
    
    // Run NUMTRIES tests and find the max performance
    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

        // Double for loop where function call is inner loop
        #pragma omp parallel
        {
            int first = omp_get_thread_num() * NUM_ELEMENTS_PER_CORE;

            for( int shift = 0; shift < Size; shift++ )
            {
                float sum = 0.;
                Sums[shift] = SimdMulSum(&A[first], &A[first+shift], NUM_ELEMENTS_PER_CORE);
                Sums[shift] = sum;
            }
        }
        
        double time1 = omp_get_wtime();
        double GigaMults = (double)(Size * Size) /(time1-time0)/1000000000.;

        // Prevent infinite values from entering the data
        if (isfinite(GigaMults))
        {
            if (GigaMults > maxGigaMults)
            maxGigaMults = GigaMults;
        }
    }

    // Print GigaMultsPerSecond
    printf(" SIMD giga-ops / sec: %4.3lf\t\n", maxGigaMults);

    return 0;
}


// Function to multiply and reduce an array to a sum with SIMD
float SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}
