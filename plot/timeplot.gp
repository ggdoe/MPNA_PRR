set term png size 800,600
# set term png size 1200,800
set output "OUTPUT_time.png"
#
# Find number of points
#
filename="FILENAME.dat"
configfile=system("cut --output-delimiter=' - ' -f2,3,5,6,7 'FILENAME.cfg'")

stats filename nooutput
N = STATS_records

bin(x, s) = s*int(x/s)

set key box top left

set xrange [0:0.2]
set xlabel "Temps (ms)"

# Uniform
set title "Distribution du temps de calcul par itérations\n" . configfile
plot filename u (bin($2 / $1 / 1e3 ,0.002)):(3./N) smooth freq t 'en fréquence' w boxes fs solid 0.4, \
     '' u ($2 / $1 / 1e3):(1.) smooth cnorm t 'en cumulée' lw 2
# pause -1
