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

configfile=system("cut --output-delimiter=' - ' -f2,5,6,7 'FILENAME.cfg'")

set title "Durée de calcul par itérations\n" . configfile
set grid

unset key
# set border 2
# set xtics ("mpi=1 - omp=1" 1, "mpi=1 - omp=8" 2, "mpi=8 - omp=16" 3) scale 0.0
set xtics ("1" 1, "2" 2, "4" 3, "6" 4, "8" 5, "10" 6, "12" 7, "14" 8, "16" 9) scale 0.0
# set xtics nomirror
# set ytics nomirror

set ylabel "Temps (ms)"
set xlabel "Nombre de thread OMP"

# set logscale y
# set yrange [0:4]


