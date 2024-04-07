set output "s.png"
set term pngcairo size 500,500 enhanced

set title "S(it)"
set xlabel "it"
set ylabel "S"
set grid
plot "s16.dat" using 1:2 with line title "k=16", "s8.dat" using 1:2 with line title "k=8", "s4.dat" using 1:2 with line title "k=4", "s2.dat" using 1:2 with line title "k=2", "s1.dat" using 1:2 with line title "k=1"


set output "potencjal.png"
set term png size 2000,1000

set view map
set palette defined (-1 "blue", 0 "white", 1 "red")
set multiplot layout 3, 2

set size square
unset grid
set xrange [0:26.6]
set yrange [0:26.6]
set cbrange [-1:1]
set ylabel 'y'
set xlabel 'x'


set pm3d
unset surface

set title 'k = 16'
splot "s16graph.dat" using 1:2:3 notitle

set title 'k = 8'
splot "s8graph.dat" using 1:2:3 notitle

set title 'k = 4'
splot "s4graph.dat" using 1:2:3 notitle

set title 'k = 2'
splot "s2graph.dat" using 1:2:3 notitle

set title 'k = 1'
splot "s1graph.dat" using 1:2:3 notitle


unset multiplot
unset view

