#!/bin/bash
#SBATCH --job-name=run_seq
#SBATCH --output=%x_output.txt
#SBATCH --error=%x_error.txt
#SBATCH --ntasks=1
#SBATCH --time=01:00:00
#SBATCH --partition=cpu_short

# execution
./prr_gcc mat30x30.txt seq 1
./prr_icc mat30x30.txt seq 1
