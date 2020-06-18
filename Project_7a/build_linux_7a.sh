#!/bin/bash

###############################################################################
##  Author:       Adam Wright
##  Date:         6/17/2020
##  Course:       OSU cs-475
##  Project:      Project 7a
##  Description:  Script to compile and run project 7a with OpenCL and OpenGL
##                on Linux.
###############################################################################

# Build the particle simulation
g++ Project_7a.cpp -o Project_7a -DGL_GLEXT_PROTOTYPES -lm -fopenmp -w

# Run the particle simulation
./Project_7a
