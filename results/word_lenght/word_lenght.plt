set terminal png size 1000, 450 font 'Verdana, 14'
set output 'word_lenght.png' 
set title "MPI MD5 hack"
set xlabel 'Длина слова'
set ylabel 'Время (сек.)'

set key left top

set grid lc rgb  '#555555' lw 1 lt 4
set grid lc rgb '#555555' lw 1 lt 4

set xtics 1

plot 'word_lenght.txt' using 1:2 with linespoints lw 1 lt rgb 'blue' title 'p =   8',\
'word_lenght.txt' using 1:3 with linespoints lw 1 lt rgb 'red' title 'p = 16',\
'word_lenght.txt' using 1:4 with linespoints lw 1 lt rgb 'green' title 'p = 24',\
'word_lenght.txt' using 1:5 with linespoints lw 1 lt rgb 'blue' title 'p = 32',\
'word_lenght.txt' using 1:6 with linespoints lw 1 lt rgb 'red' title 'p = 40',\
'word_lenght.txt' using 1:7 with linespoints lw 1 lt rgb 'green' title 'p = 48',\
'word_lenght.txt' using 1:8 with linespoints lw 1 lt rgb 'blue' title 'p = 56',\
'word_lenght.txt' using 1:9 with linespoints lw 1 lt rgb 'red' title 'p = 64'