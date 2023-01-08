#!/bin/bash
module load intel/20.0.4/gcc-4.8.5 intel-mkl/2020.4.304/intel-20.0.4.304 intel-mpi/2019.9.304/intel-20.0.4.304
module load gcc/11.2.0/gcc-4.8.5 openmpi/4.1.1/gcc-11.2.0 openblas/0.3.8/gcc-9.2.0

ntasks=8
cpt=2
#omp_num_th=2

filename=mat7x7.txt
epsilon=1e-2
freq=100
m=5
max_it=0
nb_rep=3

path=logs/$(date +%s)
# path=logs/
mkdir -p $path

for i in $m
do
	echo "step : m = $i"
	args="--m $i --max_it $max_it --file $filename --epsilon $epsilon --nb_rep $nb_rep --freq $freq"
	rm -f result.dat
  
	srun --cpus-per-task=$cpt ./prr_gcc $args > /dev/null
	mv result.dat $path/gcc_result_$m.dat

	srun --cpus-per-task=$cpt ./prr_icc $args > /dev/null
	mv result.dat $path/icc_result_$m.dat

	srun --ntasks=$ntasks --cpus-per-task=$cpt ./multiprr_gcc $args > /dev/null
	mv result.dat $path/multi_gcc_result_$m.dat

	srun --ntasks=$ntasks --cpus-per-task=$cpt ./multiprr_icc $args > /dev/null
	mv result.dat $path/multi_icc_result_$m.dat

	mv config.dat $path/config_$m.dat
done

