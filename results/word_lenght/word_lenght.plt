set terminal png size 1000, 450 font 'Verdana, 10'
set output 'word_lenght.png' 
set title "MPI MD5 hack"
set xlabel 'Word lenght'
set ylabel 'Time (sec.)'

set key left top

set grid lc rgb  '#555555' lw 1 lt 4
set grid lc rgb '#555555' lw 1 lt 4

set xtics 1

plot 'word_lenght.txt' using 1:2 with linespoints lw 1 lt rgb 'blue' title 'MD5 hack (alphabet size = 7, number of procecces =   8)',\
'word_lenght.txt' using 1:3 with linespoints lw 1 lt rgb 'red' title 'MD5 hack (alphabet size = 7, number of procecces = 16)',\
'word_lenght.txt' using 1:4 with linespoints lw 1 lt rgb 'green' title 'MD5 hack (alphabet size = 7, number of procecces = 24)',\
'word_lenght.txt' using 1:5 with linespoints lw 1 lt rgb 'blue' title 'MD5 hack (alphabet size = 7, number of procecces = 32)',\
'word_lenght.txt' using 1:6 with linespoints lw 1 lt rgb 'red' title 'MD5 hack (alphabet size = 7, number of procecces = 40)',\
'word_lenght.txt' using 1:7 with linespoints lw 1 lt rgb 'green' title 'MD5 hack (alphabet size = 7, number of procecces = 48)',\
'word_lenght.txt' using 1:8 with linespoints lw 1 lt rgb 'blue' title 'MD5 hack (alphabet size = 7, number of procecces = 56)',\
'word_lenght.txt' using 1:9 with linespoints lw 1 lt rgb 'red' title 'MD5 hack (alphabet size = 7, number of procecces = 64)'