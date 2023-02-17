set term png size 800,600
# set term png size 1200,800
set output "stats_iteration.png"
#
# Find number of points
#
filename="stats_iteration.data"

# stats filename nooutput
# N = STATS_records

# set style fill solid 0.5 border -1
# set style boxplot outliers pointtype 7
# set style data boxplot
# set boxwidth  0.5
# set pointsize 0.5

configfile=system("cut --output-delimiter=' - ' -f5,6,7 16_icc_mpi-16_omp-16.cfg")

set title "Durée de calcul par itérations\n" . configfile
set grid

# unset key
# set border 2
# set xtics ("mpi=1 - omp=1" 1, "mpi=1 - omp=8" 2, "mpi=8 - omp=16" 3) scale 0.0
# set xtics nomirror
# set ytics nomirror

set ylabel "temps (s)"

plot filename u 1:($2/1e3):($3/1e3):($4/1e3) w errorlines t ""
# pause -1

# set logscale y
# set yrange [0:100]


