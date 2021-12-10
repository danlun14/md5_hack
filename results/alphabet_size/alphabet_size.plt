set terminal png size 1000, 450 font 'Verdana, 10'
set output 'alphabet_size.png' 
set title "MPI MD5 hack"
set xlabel 'alphabet_size'
set ylabel 'Time (sec.)'

set key left top

set grid lc rgb  '#555555' lw 1 lt 4
set grid lc rgb '#555555' lw 1 lt 4

set xtics 6

plot 'alphabet_size.txt' using 1:2 with linespoints lw 1 lt rgb 'blue' title 'MD5 hack(word_lenght = 6, number of procecces = 8)',\
'alphabet_size.txt' using 1:3 with linespoints lw 1 lt rgb 'red' title 'MD5 hack(word_lenght = 6, number of procecces = 16)',\
'alphabet_size.txt' using 1:4 with linespoints lw 1 lt rgb 'green' title 'MD5 hack(word_lenght = 6, number of procecces = 24)',\
'alphabet_size.txt' using 1:5 with linespoints lw 1 lt rgb 'blue' title 'MD5 hack(word_lenght = 6, number of procecces = 32)',\
'alphabet_size.txt' using 1:6 with linespoints lw 1 lt rgb 'red' title 'MD5 hack(word_lenght = 6, number of procecces = 40)',\
'alphabet_size.txt' using 1:7 with linespoints lw 1 lt rgb 'green' title 'MD5 hack(word_lenght = 6, number of procecces = 48)',\
'alphabet_size.txt' using 1:6 with linespoints lw 1 lt rgb 'red' title 'MD5 hack(word_lenght = 6, number of procecces = 56)',\
'alphabet_size.txt' using 1:7 with linespoints lw 1 lt rgb 'green' title 'MD5 hack(word_lenght = 6, number of procecces = 64)'