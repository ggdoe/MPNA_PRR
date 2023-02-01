#!/bin/bash
#SBATCH --ntasks=$2
#SBATCH --cpus-per-task=$3

module load intel/20.0.4/gcc-4.8.5 intel-mkl/2020.4.304/intel-20.0.4.304 intel-mpi/2019.9.304/intel-20.0.4.304
module load gcc/11.2.0/gcc-4.8.5 openmpi/4.1.1/gcc-11.2.0 openblas/0.3.8/gcc-9.2.0

while IFS=' ' read -r -u "$fd_num" cc execution m max_it filename epsilon nb_rep freq
do
	if [[ ${cc:0:1} == "#" ]]; then 
		continue
	fi

	if [ "$execution" == "seq" ]; then
		exe="prr_$cc"
	elif [ "$execution" == "par" ]; then
		exe="multiprr_$cc"
	else
		echo "Error : execution has to be \"seq\" or \"par\". Check \"run_config.csv\"."
		exit 1
	fi

	mat_name="$(basename -- "$filename")"
	mat_name="${mat_name%.*}"
	path="logs"/"${mat_name}_${execution}_${cc}_$(date "+%F-%T")"
	mkdir -p $path

	args="--m $m --max_it $max_it --file $filename --epsilon $epsilon --nb_rep $nb_rep --freq $freq"
	srun ./$exe $args > /dev/null
	mv result.dat $path/${execution}_${cc}_result_$m.dat
	mv config.dat $path/config_$m.cfg
done {fd_num}< <(tail -n +2 run_config.cfg)
