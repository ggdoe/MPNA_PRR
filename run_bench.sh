#!/bin/bash

module load intel/20.0.4/gcc-4.8.5 intel-mkl/2020.4.304/intel-20.0.4.304 intel-mpi/2019.9.304/intel-20.0.4.304
module load gcc/11.2.0/gcc-4.8.5 openmpi/4.1.1/gcc-11.2.0 openblas/0.3.8/gcc-9.2.0

#export OMP_PLACES=threads
declare -i counter=1
path="logs/$(date "+%F-%T")"
mkdir -p $path

while IFS=' ' read -r -u "$fd_num" cc mpi_node omp_thread filename m epsilon nb_rep freq max_it
do
	if [[ -z $cc ]] || [[ ${cc:0:1} == "#" ]]; then 
		continue
	fi

	export OMP_NUM_THREADS=$omp_thread
	export MKL_NUM_THREADS=$omp_thread

	if [[ $mpi_node -lt 1 ]] || [[ $omp_thread -lt 1 ]]; then
		echo "Error : either mpi_node or omp_thread is lower than 1. Check \"run_config.cfg\"."
		exit 1
	elif [[ $mpi_node -eq 1 ]]; then
		exe="prr_$cc"
	else
		exe="multiprr_$cc"
	fi

	args="--m $m --max_it $max_it --file matrices/$filename --epsilon $epsilon --nb_rep $nb_rep --freq $freq"
	echo $cc mpi=$mpi_node omp=$omp_thread $filename

	srun --ntasks=$mpi_node --cpus-per-task=$omp_thread ./$exe $args > /dev/null
	mv result.dat $path/${counter}_${cc}_mpi-${mpi_node}_omp-${omp_thread}.dat
	mv config.dat $path/${counter}_${cc}_mpi-${mpi_node}_omp-${omp_thread}.cfg
	counter+=1
done {fd_num}<run_config.cfg
