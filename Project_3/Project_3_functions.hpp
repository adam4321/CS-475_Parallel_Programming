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
void Watcher();
void MyAgent();

// Helper functions
float Ranf(unsigned int *, float, float);
int Ranf(unsigned int *, int, int);
int rand_r(unsigned int* seed);

#endif