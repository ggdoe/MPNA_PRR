# set term png size 600,400
set term png size 1200,800
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

unset key
set border 2
# set xtics ("A" 1, "B" 2) scale 0.0
set xtics nomirror
set ytics nomirror

set ylabel "temps (ms)"

set logscale y
# set yrange [0:100]


