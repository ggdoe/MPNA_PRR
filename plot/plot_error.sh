#!/bin/bash

maxit=600

path=${1%*/}
if [ -z $path ]; then 
    echo "no path given"
    exit 1
fi
echo $path

# for i in *.dat; 
# do 
#     cut -f 2 ${i%.*}.cfg | sed "s/.*=//" | tr '\n' ' ' ; 
#     cat $i | gnuplot -e 'stats "-" u ($2/$1) name "a" nooutput; print a_mean, a_lo_quartile, a_up_quartile'; 
# done &> stats_iteration.data