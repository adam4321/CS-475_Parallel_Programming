/******************************************************************************
**  Author:       Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         4-20-2020
**  Description:  Program 3 for OSU cs-475 Parallel Programming. The program
**                uses OpenMP and is a 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "Project_3_functions.hpp"


int main(int argc, char *argv[])
{
    #ifndef _OPENMP
        fprintf(stderr, "No OpenMP support!\n");
        return 1;
    #endif

    // Set the number of threads to use
    omp_set_num_threads( 4 );	// same as # of sections
    

    // Functional decomposition spread over 4 threads
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            GrainDeer();
        }

        #pragma omp section
        {
            Grain();
        }

        #pragma omp section
        {
            MyAgent();	// your own
        }

        #pragma omp section
        {
            Watcher();
        }
    }
    // implied barrier -- all functions must return in order
    // to allow any of them to get past here

    return 0;
}
