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


// Functions run in parallel to simulate the grain-growing operation

void GrainDeer()
{
    printf("hello graindeer\n");
}


void Grain()
{
    printf("hello grain\n");
}


void Watcher()
{
    printf("hello watcher\n");
}


void MyAgent()
{
    printf("hello myagent\n");
}


// Helper Functions:

// To choose a random number between two floats
float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float)( rand_r( seedp ) );              // 0 - RAND_MAX

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
   <http://www.gnu.org/licenses/>.  */

/* This algorithm is mentioned in the ISO C standard, here extended
   for 32 bits.  */
int
rand_r (unsigned int *seed)
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
