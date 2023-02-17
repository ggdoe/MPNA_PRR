set term png size 800,600
# set term png size 1200,800
set output "OUTPUT"
#
# Find number of points
#
# filename="FILENAME"

# stats filename nooutput
# N = STATS_records

set style fill solid 0.5 border -1
set style boxplot outliers pointtype 7
set style data boxplot
set boxwidth  0.5
set pointsize 0.5

configfile=system("cut --output-delimiter=' - ' -f5,6,7 'FILENAME.cfg'")

set title "Boxplot nombre d'itérations\n" . configfile
set grid

unset key
# set border 2
set xtics ("mpi=1 - omp=1" 1, "mpi=1 - omp=8" 2, "mpi=8 - omp=16" 3) scale 0.0
# set xtics nomirror
# set ytics nomirror

set ylabel "Nombre d'itérations"

set logscale y
# set yrange [0:100]


