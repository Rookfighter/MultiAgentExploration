# needs 'infile' variable set
# needs 'outfile' variable set

set title "Time until Coverage reached"
set xlabel "coverage"
set ylabel "minutes"

set terminal png
set output outfile

# data in file is in usec, so scale to minutes
plot infile using ($1):($2/60000000) title "time-coverage" with linespoints
