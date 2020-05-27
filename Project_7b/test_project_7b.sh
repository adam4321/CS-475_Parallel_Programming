#!/bin/bash

###############################################################################
##  Author:       Adam Wright
##  Date:         5/26/2020
##  Course:       OSU cs-475
##  Project:      Project 7b
##  Description:  Script to compile and run project 7b with OpenMP, SIMD and 
##                OpenCL while running an autocorrelation of random values.
###############################################################################

#SBATCH -J Project_7b
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o Project_7b.out
#SBATCH -e Project_7b.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=wrighada@oregonstate.edu

OUTPUT_FILE="Project_7_performance.txt"

# Clear the test files
> $OUTPUT_FILE

# Message to user while results are gathered
printf "\nGathering the parallel comparison results...\n\n"

# Compile and run the 1 thread test
g++ -o Project_7b_1thread Project_7b_openMp.cpp -O3 -lm -fopenmp
./Project_7b_1thread >> $OUTPUT_FILE
printf "\n" >> $OUTPUT_FILE

# Compile and run the 4 thread openMP test
g++ -o Project_7b_4thread Project_7b_openMp.cpp -O3 -lm -fopenmp -D THREAD_COUNT=4
./Project_7b_4thread >> $OUTPUT_FILE
printf "\n" >> $OUTPUT_FILE

# # Compile and run the openCL version
# g++ -o Project_7b_openCL Project_7b_openCL.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
# ./Project_7b_openCL >> $OUTPUT_FILE
    
printf "\n" >> $OUTPUT_FILE


# Print a description of the axes to the terminal
printf "        ***************************************************\n"
printf "        ** 1 thread vs openMP 6 thread vs SIMD vs openCL **\n"
printf "        ***************************************************\n\n"
printf "           NMB(Array Size) vs. GigaMults / sec.\n\n"

# Print the multiplication file contents to the terminal
cat $OUTPUT_FILE
printf "\n"