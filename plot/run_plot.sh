#!/bin/bash

maxit=600

path=${1%*/}
if [ -z $path ]; then 
    echo "no path given"
    exit 1
fi
echo $path

box_str="plot"
box_str_mean="plot"
declare -i COUNTER=1

for i in $path/*.dat; do
    echo processing $i
    sed "s#FILENAME#${i%.*}#;s#OUTPUT#${i%.*}#" freqplot.gp | gnuplot
    sed "s#FILENAME#${i%.*}#;s#OUTPUT#${i%.*}#" timeplot.gp | gnuplot
    box_str="$box_str \"<awk '\$1<$maxit{print}' $i\" using ($COUNTER):(\$2 / 1e6),"
    box_str_mean="$box_str_mean \"<awk '\$1<$maxit{print}' $i\" using ($COUNTER):(\$2 / 1e3 / \$1),"
    COUNTER+=1
done

cat boxplot.gp - <<< $box_str | sed "s#FILENAME#${i%.*}#;s#OUTPUT#$path/boxplot.png#" | gnuplot
cat boxplot_mean.gp - <<< $box_str_mean | sed "s#FILENAME#${i%.*}#;s#OUTPUT#$path/boxplot_mean.png#" | gnuplot
