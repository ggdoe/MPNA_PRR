#!/bin/sh

# --> module à charger :
module load intel/20.0.4/gcc-4.8.5 intel-mkl/2020.4.304/intel-20.0.4.304 intel-mpi/2019.9.304/intel-20.0.4.304
module load gcc/11.2.0/gcc-4.8.5 openmpi/4.1.1/gcc-11.2.0 openblas/0.3.8/gcc-9.2.0

FILES="src/*.c"
ICC_FLAGS=-Wall -std=c11 -flto -O3 -mkl -DMKL
GCC_FLAGS=-flto -O3 -lm -lopenblas 
#-no-multibyte-chars

echo prr_icc
icc $ICC_FLAGS $FILES -o prr_icc 
echo mpi_icc
mpiicc $ICC_FLAGS -DMULTIPRR $FILES -o multiprr_icc 

echo prr_gcc
gcc $GCC_FLAGS $FILES -o prr_gcc
echo mpi_gcc
mpicc $GCC_FLAGS -DMULTIPRR $FILES -o multiprr_gcc

