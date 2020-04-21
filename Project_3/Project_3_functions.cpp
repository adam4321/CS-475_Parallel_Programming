/******************************************************************************
**  Author:       Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         4-20-2020
**  Description:  Function definitions for Program 3 cs-475 spring 2020
******************************************************************************/

#include "Project_3_functions.hpp"
#include <iostream>
#include <stdlib.h>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


// Global system state variables
int	NowYear = 2020;		// 2020 - 2025
int	NowMonth = 0;		// 0 - 11

float	NowPrecip = 3.;		// inches of rain per month
float	NowTemp = 45.;		// temperature this month
float	NowHeight = 5.;		// grain height in inches
int	    NowNumDeer = 25;		// number of deer in the current population

unsigned int seed = 0;

// Required monthly simulation parameters
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


//* Functions run in parallel to simulate the grain-growing operation -------*/

void GrainDeer()
{
    int x=0;
    while (NowYear <= 2025)
    {
        x++;
        // Done Computing
        #pragma omp barrier
        x++;
        printf("GrainDeer %d ", x);
        // Done Assigning
        #pragma omp barrier
        // Done Printing
        #pragma omp barrier
    }
}


void Grain()
{
    int x=2;
    while (NowYear <= 2025)
    {
        x++;
        // Done Computing
        #pragma omp barrier
        x++;
        printf("Grain %d ", x);
        // Done Assigning
        #pragma omp barrier
        // Done Printing
        #pragma omp barrier
    }
}


void MyAgent()
{
    int x=4;
    while (NowYear <= 2025)
    {
        x++;
        // Done Computing
        #pragma omp barrier
        x++;
        printf("Myagent %d ", x);
        // Done Assigning
        #pragma omp barrier
        // Done Printing
        #pragma omp barrier
    }
}


void Watcher()
{
    while (NowYear <= 2025)
    {
        // Done Computing
        #pragma omp barrier
        // Done Assigning
        #pragma omp barrier

        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
        float temp = AVG_TEMP - AMP_TEMP * cos( ang );

        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )    { NowPrecip = 0.; }

        // Print the current state and update the month and year
        printf("Current Year: %d Current Month: %d\n", NowYear, NowMonth);

        NowMonth++;
        if (NowMonth == 12)
        {
            NowMonth = 0;
            NowYear++;
        }
        // Done Printing
        #pragma omp barrier
    }
}


/* Helper Functions ---------------------------------------------------------*/

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
