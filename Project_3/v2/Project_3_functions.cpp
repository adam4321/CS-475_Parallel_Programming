/******************************************************************************
**  Author:       Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         4-20-2020
**  Description:  Function definitions for Program 3 cs-475 spring 2020
******************************************************************************/

#include <stdio.h>

#define _CRT_RAND_S
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "Project_3_functions.hpp"


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


/* Functions run in parallel to simulate the grain-growing operation --------*/

// Function that represents the deer population over time in the simulation
void GrainDeer()
{
    int grain_deer;

    while (NowYear <= 2025)
    {
        grain_deer = NowNumDeer;

        // If deer overrun the grain, then they die
        if ((float)grain_deer > NowHeight)
        {
            grain_deer--;
        }
        // If the height is greater than the deer, then they reproduce
        else if ((float)grain_deer < NowHeight)
        {
            grain_deer++;
        }

        // Done Computing
        #pragma omp barrier

        // Update the global deer count
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
        
        // Use sine-wave functions to create cyclical values 
        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

        // Create the current grain height
        grain_height += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		grain_height -= (float) NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        // Don't allow a negative grain height
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

        // Once there is a group of deer, one will be captured
        if (NowNumDeer > 3)
        {
            deer_captured++;
        }

        // After 3 deer are captured, a new wolf is born
        if (deer_captured > 3)
        {
            num_wolves++;
            deer_captured = 0;
        }

        // If the deer population drops, then the wolf population will also drop
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
    float grain_height_c;

    while (NowYear <= 2025)
    {
        // Done Computing
        #pragma omp barrier
        // Done Assigning
        #pragma omp barrier

        // Conver temperature and precipitation to metric locally
        precip_c = NowPrecip * 2.54;
        temp_c = (5./9.) * (NowTemp - 32);
        grain_height_c = NowHeight * 2.54;

        // Print the current state and update the month and year
        printf("%d\t%d\t%.2f\t%.2f\t%.2f\t%d\t%d\n", NowYear, NowMonth + 1, 
        precip_c, temp_c, grain_height_c, NowNumDeer, NowNumWolves);

        // Update the current month and year
        NowMonth++;
        if (NowMonth == 12)
        {
            NowMonth = 0;
            NowYear++;
        }

        // Call the randomization functions to set up the next month
        Calc_Weather(&seed);

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


// To calculate the current weather
void Calc_Weather(unsigned int *seed)
{
    // Create sine wave to simulate seasons
    double ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    // Calculate seasonal temperatures
    double temp = AVG_TEMP - AMP_TEMP * cos( ang );

    NowTemp = temp + Ranf( seed, -RANDOM_TEMP, RANDOM_TEMP );

    double precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( seed,  -RANDOM_PRECIP, RANDOM_PRECIP );

    if( NowPrecip < 0. )
    {
        NowPrecip = 0.;
    }
}


// To choose a random number between two floats
float Ranf( unsigned int *seedp,  float low, float high )
{
    float r = (float)( rand_s( seedp ) );              // 0 - RAND_MAX

    return(   low  +  r * ( high - low ) / (double)RAND_MAX   );
}


// To choose a random number between two ints
int Ranf( unsigned int *seedp, int ilow, int ihigh )
{
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;

    return (int)(  Ranf(seedp, low,high) );
}
