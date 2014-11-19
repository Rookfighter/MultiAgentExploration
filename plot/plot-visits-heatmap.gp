# needs 'infile' variable set
# needs 'outfile' variable set

set title "Visits Heatmap"
set cblabel "visits"
set xlabel "x-pos"
set ylabel "y-pos"

set terminal png
set output outfile

plot infile using 1:2:3 title "visits-map" with image
