# set term png size 600,400
set term png size 1200,800
set output "OUTPUT_time.png"
#
# Find number of points
#
filename="FILENAME"

stats filename nooutput
N = STATS_records

bin(x, s) = s*int(x/s)

set key box top left

# set xrange [0:1000]

# Uniform
set title "Uniform Distribution N=" . N
plot filename u (bin($2 / $1,5)):(10./N) smooth freq t 'smooth frequency' w boxes fs solid 0.4, \
     '' u 1:(1.) smooth cnorm t 'smooth cnorm' lw 2
# pause -1
