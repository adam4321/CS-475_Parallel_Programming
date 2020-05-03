/******************************************************************************
**  Author:       Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         4-20-2020
**  Description:  Program 3 for OSU cs-475 Parallel Programming. The program
**                uses OpenMP and is a simulation of 3 actors and a watcher in
**                an environment. It uses functional decomposition to have 4
**                synchronized threads executing the actors.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


/* Global Variables ---------------------------------------------------------*/

// Global system state variables
int	NowYear = 2020;		// 2020 - 2025
int	NowMonth = 0;		// 0 - 11

float	NowPrecip = 3.;		    // inches of rain per month
float	NowTemp = 45.;		    // temperature this month
float	NowHeight = 3.;		    // grain height in inches
int	    NowNumDeer = 1;		    // number of deer in the current population
int     NowNumWolves = 1;       // number of wolves (MY AGENT)

// Global monthly simulation variables
const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =	1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			    2.0;	// plus or minus noise

const float AVG_TEMP =		60.0;	// average
const float AMP_TEMP =		20.0;	// plus or minus
const float RANDOM_TEMP =	10.0;	// plus or minus noise

const float MIDTEMP =		40.0;
const float MIDPRECIP =		10.0;


/* Function Declarations ----------------------------------------------------*/

// Functions run in parallel to simulate the grain-growing operation
void GrainDeer();
void Grain();
void Watcher();
void Wolves();

// Helper functions
float SQR( float x );
float Ranf(unsigned int *, float, float);
int Ranf(unsigned int *, int, int);
int local_rand_r(unsigned int* seed);


/* Main Function ------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    #ifndef _OPENMP
        fprintf(stderr, "No OpenMP support!\n");
        return 1;
    #endif


    // Seed for the local_rand_r function
    unsigned int seed = 0;

    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
    {
        NowPrecip = 0.;
    }

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
            Wolves();	// your own
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


/* Function Definitions -----------------------------------------------------*/

// Function that represents the deer population over time in the simulation
void GrainDeer()
{
    int grain_deer;

    while (NowYear <= 2025)
    {
        grain_deer = NowNumDeer;

        if ((float)grain_deer > NowHeight)
        {
            grain_deer--;
        }
        else if ((float)grain_deer < NowHeight)
        {
            grain_deer++;
        }

        // Done Computing
        #pragma omp barrier

        NowNumDeer = grain_deer;

        // Done Assigning
        #pragma omp barrier
        // Done Printing
        #pragma omp barrier
    }
}


// Function that represents the grain height over time in the simulation
void Grain()
{
    float grain_height;
    float tempFactor;
    float precipFactor;

    while (NowYear <= 2025)
    {
        grain_height = NowHeight;
        
        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

        grain_height += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		grain_height -= (float) NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        if (grain_height < 0.)
        {
            grain_height = 0;
        }

        // Done Computing
        #pragma omp barrier
        
        NowHeight = grain_height;

        // Done Assigning
        #pragma omp barrier
        // Done Printing
        #pragma omp barrier
    }
}


// Function which is my agent which represents a monster who removes deer from the simulation
void Wolves()
{
    int num_wolves;
    int deer_captured = 0;

    while (NowYear <= 2025)
    {
        bool new_wolf = false;
        num_wolves = NowNumWolves;

        if (NowNumDeer > 3)
        {
            deer_captured++;
        }

        if (deer_captured > 3)
        {
            num_wolves++;
            deer_captured = 0;
        }

        if (num_wolves > 1 && num_wolves > NowNumDeer)
        {
            num_wolves--;
        }


        // Done Computing
        #pragma omp barrier

        // Update the global number of wolves
        NowNumWolves = num_wolves;

        // Done Assigning
        #pragma omp barrier
        // Done Printing
        #pragma omp barrier
    }
}


// Function which watches the other threads and gathers and prints the current state of the simulation
void Watcher()
{
    // Seed for the local_rand_r function
    unsigned int seed = 0;

    float precip_c;
    float temp_c;

    while (NowYear <= 2025)
    {
        // Done Computing
        #pragma omp barrier
        // Done Assigning
        #pragma omp barrier

        // Conver temperature and precipitation to metric locally
        precip_c = NowPrecip * 2.54;
        temp_c = (5./9.) * (NowTemp - 32);

        // Print the current state and update the month and year
        printf(" %d\t%d\t%.2f\t%.2f\t%.2f\t%d\t%d\n", NowYear, NowMonth + 1, 
        precip_c, temp_c, NowHeight, NowNumDeer, NowNumWolves);

        NowMonth++;
        if (NowMonth == 12)
        {
            NowMonth = 0;
            NowYear++;
        }


        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
        {
            NowPrecip = 0.;
        }

        // Done Printing
        #pragma omp barrier
    }
}


/* Helper Functions ---------------------------------------------------------*/

// Function to return the square of a float
float SQR( float x )
{
    return x * x;
}


// To choose a random number between two floats
float Ranf( unsigned int *seedp,  float low, float high )
{
    float r = (float)( local_rand_r( seedp ) );              // 0 - RAND_MAX

    return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


// To choose a random number between two ints
int Ranf( unsigned int *seedp, int ilow, int ihigh )
{
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;

    return (int)(  Ranf(seedp, low,high) );
}


/* Reentrant random function from POSIX.1c.
   Copyright (C) 1996-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1996.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.

   This algorithm is mentioned in the ISO C standard, here extended
   for 32 bits.
*/
int local_rand_r (unsigned int *seed)
{
  unsigned int next = *seed;
  int result;
  next *= 1103515245;
  next += 12345;
  result = (unsigned int) (next / 65536) % 2048;
  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;
  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;
  *seed = next;
  return result;
}
