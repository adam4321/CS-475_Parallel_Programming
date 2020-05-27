/******************************************************************************
**  Authors:      Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         5-26-2020
**  Description:  Program 7b for OSU cs-475 Parallel Programming. The program
**                uses        
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main()
{
    // Print error message and exit if OpenMP isn't supported
    #ifndef _OPENMP
        fprintf(stderr, "No OpenMP support!\n");
        return 1;
    #endif



    return 0;
}