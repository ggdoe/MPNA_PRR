#!/bin/bash

path=${1%*/}
if [ -z $path ]; then 
    echo "no path given"
    exit 1
fi
echo $path

box_str="plot"
declare -i COUNTER=1

for i in $path/*.dat; do
    echo processing $i
    sed "s#FILENAME#$i#;s#OUTPUT#${i%.*}#" freqplot.gp | gnuplot
    sed "s#FILENAME#$i#;s#OUTPUT#${i%.*}#" timeplot.gp | gnuplot
    box_str="$box_str \"<awk '\$1<500{print}' $i\" using ($COUNTER):(\$2 / 1e3),"
    COUNTER+=1
done

cat boxplot.gp - <<< $box_str | sed "s#OUTPUT#$path/boxplot.png#" | gnuplot
