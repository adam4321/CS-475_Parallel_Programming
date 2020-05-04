/******************************************************************************
**  Author:       Adam Wright
**  Email:        wrighada@oregonstate.edu
**  Date:         4-20-2020
**  Description:  Header file for Project 3 functions cs-475 spring 2020
******************************************************************************/

#ifndef PROJECT_3_FUNCTIONS_HPP
#define PROJECT_3_FUNCTIONS_HPP

// Functions run in parallel to simulate the grain-growing operation
void GrainDeer();
void Grain();
void Wolves();
void Watcher();

// Helper functions
void Calc_Weather(unsigned int *seed);
float SQR( float x );
float Ranf(unsigned int *, float, float);
int Ranf(unsigned int *, int, int);

#endif
