
set term png size 2000,1000

set output "wykresyS.png"


set multiplot layout 1, 2

set logscale x
set xrange [1:100000]
set title 'Relaksacja globalna S(iteracja)'
set ylabel 'S(iteracja)'
set xlabel 'iteracja'
plot "rg06s.csv" using 1:2  with lines lw 2 title 'w=0.6', 'rg1s.csv' using 1:2  with lines lw 2 title 'w=1'
set title 'Relaksacja lokalna S(iteracja)'
plot "rl1s.csv" using 1:2  with lines lw 2 title 'w=1', "rl14s.csv" using 1:2  with lines lw 2 title 'w=1.4', "rl18s.csv" using 1:2  with lines lw 2 title 'w=1.8', "rl19s.csv" using 1:2  with lines lw 2 title 'w=1.9'
unset logscale x
unset multiplot

set output "potencjal.png"
set term png size 2000,1000

set view map
load 'parula.pal'
set multiplot layout 1, 2
set xrange [0:15]
set ylabel 'y'
set xlabel 'x'
set title 'Relaksacja globalna w=0.6  V(x,y)'
splot "rg06h.dat" using 1:2:3 with image
set title 'Relaksacja globalna w=1 V(x,y)'
splot "rg1h.dat" using 1:2:3 with image
unset multiplot
unset view


set output "blad.png"
set term png size 2000,1000

set view map
load 'parula.pal'
set multiplot layout 1, 2
set xrange [0:15]
set ylabel 'y'
set xlabel 'x'
set title 'Relaksacja globalna w=0.6 (blad)'
splot "rg06err.dat" using 1:2:3 with image
set title 'Relaksacja globalna w=1 (blad)'
splot "rg1err.dat" using 1:2:3 with image
unset multiplot
unset view