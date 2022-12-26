#!/bin/bash
#SBATCH --job-name=run_par
#SBATCH --output=%x_output.txt
#SBATCH --error=%x_error.txt
#SBATCH --ntasks=20 #MPI
#SBATCH --cpus-per-task=20 #OpenMP
#SBATCH --time=01:00:00
#SBATCH --partition=cpu_short

# export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK} 
# export OMP_PLACES=cores

# execution
# srun ./prr mat30x30.txt par ${SLURM_CPUS_PER_TASK}

############################


#   GCC
# srun --ntasks=48 --cpus-per-task=1 ./multiprr_gcc

#   ICC : 
# srun --ntasks=48 --cpus-per-task=1 ./multiprr_icc
#   ou bien :
# localisation mpirun pour icc :
# icc_mpirun="/gpfs/softs/spack_0.17/opt/spack/linux-centos7-cascadelake/intel-20.0.4.304/intel-mpi-2019.9.304-d4pbusne53goxmkp4drzqim2rbogfmit/compilers_and_libraries_2020.4.304/linux/mpi/intel64/bin/mpirun"
# icc_mpirun -np 48 ./multiprr_icc
