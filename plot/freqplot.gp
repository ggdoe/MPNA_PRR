set term x11 size 1200,800
#
# Find number of points
#
filename="1.dat"

stats filename nooutput
N = STATS_records

bin(x, s) = s*int(x/s)

set key box top left

# Uniform
set title "Uniform Distribution"
plot filename u (bin($1,5)):(10./N) smooth freq t 'smooth frequency' w boxes fs solid 0.4, \
     '' u 1:(1.) smooth cnorm t 'smooth cnorm' lw 2
pause -1

