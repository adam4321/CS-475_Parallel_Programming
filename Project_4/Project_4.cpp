/******************************************************************************
**  Authors:      Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         5-4-2020
**  Description:  Program 4 for OSU cs-475 Parallel Programming. The program
**                uses SIMD to speed up the processing of an array while only
**                using a single core.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <xmmintrin.h>

#define SSE_WIDTH		4

// Setting the array size
#ifndef SIZE
#define SIZE   80000
#endif

// Setting the number of iterations
#ifndef NUMTRIES
#define NUMTRIES	1000
#endif

// Setting the number of threads
#ifndef NUMT
#define NUMT		4
#endif

// Set the 
#define NUM_ELEMENTS_PER_CORE   SIZE/NUMT

// Arrays for the test
float A[SIZE];
float B[SIZE];

// Variables for the test
float arr_sum;
float simd_sum;
double Arr_Flops = 0.;
double Simd_Flops = 0.;
double speedup;


/* FUNCTION DECLARATIONS ----------------------------------------------------*/

float SimdMulSum(float *a, float *b, int len);
float ArrayMulSum(float *a, float *b, int len);


/* MAIN ---------------------------------------------------------------------*/

int main(int argc, char* argv[])
{
    #ifndef _OPENMP
		fprintf(stderr, "No OpenMP support!\n");
		return 1;
	#endif


    // Inialize the arrays
    for (int i = 0; i < SIZE; i++)
    {
        A[i] = 1.;
        B[i] = 2.;
    }


    /********************************************
    **  Test standard array multiplication
    ********************************************/
    
    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

        arr_sum = ArrayMulSum(A, B, SIZE);

        double time1 = omp_get_wtime();
        double megaMults = (double)SIZE / (time1 - time0) / 1000000.;

        // Prevent infinite values from entering data
        if (isfinite(megaMults))
        {
            if (megaMults > Arr_Flops)
            Arr_Flops = megaMults;
        }
    }

    printf("Standard Peak Performance = %8.2lf MegaFLOPS/Sec\nArr sum: %.2lf\n", Arr_Flops, arr_sum);


    /******************************************
    **  Test SIMD array multiplication
    ******************************************/

    Simd_Flops = 0.;

    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

        arr_sum = SimdMulSum(A, B, SIZE);

        double time1 = omp_get_wtime();
        double megaMults = (double)SIZE / (time1 - time0) / 1000000.;

        // Prevent infinite values from entering data
        if (isfinite(megaMults))
        {
            if (megaMults > Simd_Flops)
            Simd_Flops = megaMults;
        }
    }

    printf("SIMD Peak Performance = %8.2lf MegaFLOPS/Sec\nArr sum: %.2lf\n", Simd_Flops, arr_sum);

    // Calculate the speedup
    speedup = Simd_Flops / Arr_Flops;

    printf("SIMD Speedup: %lf\n", speedup);


    /********************************************************
    **  Test the SIMD and multi-core array multiplication
    ********************************************************/

    Simd_Flops = 0.;
    omp_set_num_threads(NUMT);

    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

        #pragma omp parallel
        {
            int first = omp_get_thread_num() * NUM_ELEMENTS_PER_CORE;
            arr_sum = SimdMulSum(&A[first], &B[first], NUM_ELEMENTS_PER_CORE);
        }

        double time1 = omp_get_wtime();
        double megaMults = (double)SIZE / (time1 - time0) / 1000000.;

        // Prevent infinite values from entering data
        if (isfinite(megaMults))
        {
            if (megaMults > Simd_Flops)
            Simd_Flops = megaMults;
        }
    }

    printf("SIMD Peak Performance = %8.2lf MegaFLOPS/Sec\nArr sum: %.2lf\n", Simd_Flops, arr_sum);

    // Calculate the speedup
    speedup = Simd_Flops / Arr_Flops;

    printf("SIMD Speedup: %lf\n", speedup);

    return 0;
}


/* FUNCTION DEFINITIONS -----------------------------------------------------*/

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


// Function to multiply and reduce an array to a sum
float ArrayMulSum(float *a, float *b, int len)
{
	double sum = 0;
	
	for(int i = 0; i < len; i++)
	{
		sum += a[i] * b[i];
	}

    return sum;
}
