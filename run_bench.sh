#!/bin/bash

module purge
module load intel/20.0.4/gcc-4.8.5 intel-mkl/2020.4.304/intel-20.0.4.304 intel-mpi/2019.9.304/intel-20.0.4.304
module load gcc/11.2.0/gcc-4.8.5 openmpi/4.1.1/gcc-11.2.0 openblas/0.3.8/gcc-9.2.0

while IFS=' ' read -r -u "$fd_num" cc omp_thread mpi_node filename m epsilon nb_rep freq max_it
do
	export OMP_NUM_THREADS=$omp_thread

	if [[ ${cc:0:1} == "#" ]]; then 
		continue
	fi

	if [[ $mpi_node -lt 1 ]] || [[ $omp_thread -lt 1 ]]; then
		echo "Error : either mpi_node or omp_thread is lower than 1. Check \"run_config.cfg\"."
		exit 1
	elif [[ $mpi_node -eq 1 ]] && [[ $omp_thread -eq 1 ]]; then
		execution="seq"
		exe="prr_$cc"
	else
		execution="par"
		exe="multiprr_$cc"
	fi

	path="logs"/"${filename%.*}_${execution}_${cc}_$(date "+%F-%T")"
	mkdir -p $path

	args="--m $m --max_it $max_it --file matrices/$filename --epsilon $epsilon --nb_rep $nb_rep --freq $freq"
	echo $cc $execution
	srun --ntasks=$mpi_node --cpus-per-task=$omp_thread ./$exe $args > /dev/null
	mv result.dat $path/${execution}_${cc}_result_$m.dat
	mv config.dat $path/config_$m.cfg
done {fd_num}<run_config.cfg
