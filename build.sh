#!/bin/sh

# --> module à charger :
module load intel/20.0.4/gcc-4.8.5 intel-mkl/2020.4.304/intel-20.0.4.304 intel-mpi/2019.9.304/intel-20.0.4.304
module load gcc/11.2.0/gcc-4.8.5 openmpi/4.1.1/gcc-11.2.0 openblas/0.3.8/gcc-9.2.0

FILES="main.c calcul_residu.c projection.c prr.c resolution_sev.c retour_espace_depart.c tools.c"

icc -Wall -std=c11 -flto -O3 -mkl -DMKL $FILES -o prr_icc -no-multibyte-chars
mpiicc -Wall -std=c11 -flto -O3 -mkl -DMKL -DMULTIPRR $FILES -o multiprr_icc -no-multibyte-chars

gcc -flto -O3 -lm -lopenblas $FILES -o prr_gcc
mpicc -flto -O3 -lm -lopenblas -DMULTIPRR $FILES -o multiprr_gcc

