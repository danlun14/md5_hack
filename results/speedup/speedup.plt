set terminal png size 1000, 450 font 'Verdana, 10'
set output 'speedup.png' 
set title "MPI MD5 hack "
set xlabel 'Number of processes'
set ylabel 'Speedup'

set key left top

set grid lc rgb  '#555555' lw 1 lt 4
set grid lc rgb '#555555' lw 1 lt 4

set xrange [8:64]
set yrange [7:64]


set xtics 8
set ytics 8

plot 'speedup.txt' using 1:2 with linespoints lw 1 lt rgb 'blue' title 'MD5 hack(alphabet size=6, word lenght=10)', \
 'speedup.txt' using 1:3 with linespoints lw 1 lt rgb 'red' title 'MD5 hack(alphabet size=12, word lenght=8)', \
 'speedup.txt' using 1:4 with linespoints lw 1 lt rgb 'green' title 'MD5 hack(alphabet size=37, word lenght=5)', \
 'speedup.txt' using 1:1 with linespoints lw 1 lt rgb 'black' title 'Linear speedup'