set term png size 800,600
# set term png size 1200,800
set output "OUTPUT_iter.png"
#
# Find number of points
#
filename="FILENAME.dat"
configfile=system("cut --output-delimiter=' - ' -f2,3,5,6,7 'FILENAME.cfg'")

maxit=system("cut -f10 'FILENAME.cfg' | sed 's/.*=//'")

stats filename nooutput
N = STATS_records

bin(x, s) = s*int(x/s)

set key box top left

set xrange [0:maxit]
set xlabel "Nombre d'itérations"

# Uniform
set title "Distribution du nombre d'itérations\n" . configfile
plot filename u (bin($1,30)):(10./N) smooth freq t 'en fréquence' w boxes fs solid 0.4, \
     '' u 1:(1.) smooth cnorm t 'en cumulée' lw 2
# pause -1
