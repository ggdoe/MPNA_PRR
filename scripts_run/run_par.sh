#!/bin/bash
#SBATCH --job-name=run_par
#SBATCH --output=%x_output.txt
#SBATCH --error=%x_error.txt
#SBATCH --ntasks=20 #MPI
#SBATCH --cpus-per-task=20 #OpenMP
#SBATCH --time=01:00:00
#SBATCH --partition=cpu_short

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK} 
export OMP_PLACES=cores

# execution
srun ./prr mat30x30.txt par ${SLURM_CPUS_PER_TASK}
