# needs 'infile-1' variable set
# needs 'infile-2' variable set
# needs 'infile-4' variable set
# needs 'infile-8' variable set
# needs 'outfile' variable set

set title "Time until Coverage reached"
set xlabel "minutes"
set ylabel "coverage"

set terminal png
set output outfile

# data in file is in usec, so scale to minutes
plot infile-1 using ($2/60000000):($1) title "1 robot" with linespoints, \
     infile-2 using ($2/60000000):($1) title "2 robots" with linespoints, \
     infile-4 using ($2/60000000):($1) title "4 robots" with linespoints, \
     infile-8 using ($2/60000000):($1) title "8 robots" with linespoints
