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


int main()
{
    // Print error message and exit if OpenMP isn't supported
    #ifndef _OPENMP
        fprintf(stderr, "No OpenMP support!\n");
        return 1;
    #endif

    FILE *fp = fopen( "signal.txt", "r" );

    if ( fp == NULL )
    {
        fprintf( stderr, "Cannot open file 'signal.txt'\n" );
        exit( 1 );
    }

    int Size;
    fscanf( fp, "%d", &Size );

    float *A =     new float[ 2*Size ];
    float *Sums  = new float[ 1*Size ];

    for( int i = 0; i < Size; i++ )
    {
        fscanf( fp, "%f", &A[i] );
        A[i+Size] = A[i];		// duplicate the array
    }

    fclose( fp );

    // Autocorrelate the supplied signal
    for( int shift = 0; shift < Size; shift++ )
    {
        float sum = 0.;
        for( int i = 0; i < Size; i++ )
        {
            sum += A[i] * A[i + shift];
        }
        Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
    }

    return 0;
}

