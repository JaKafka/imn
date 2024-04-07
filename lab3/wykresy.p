
set term png size 2000,1000

set datafile separator ';'

set output "rk2.png"

set multiplot layout 2, 2

set ylabel 'x(t)'
set xlabel 't'
set title 'RK2 x(t)'
plot "rk22.csv" using 1:3 with lines lw 2 title 'TOL = 10^-2', "rk25.csv" using 1:3 with lines lw 2 title 'TOL = 10^-5'


set ylabel 'v(t)'
set xlabel 't'
set title 'RK2 v(t)'
plot "rk22.csv" using 1:4 with lines lw 2 title 'TOL = 10^-2', "rk25.csv" using 1:4 with lines lw 2 title 'TOL = 10^-5'
set ylabel 'dt(t)'
set xlabel 't'
set title 'RK2 dt(t)'
plot "rk22.csv" using 1:2 with lines lw 2 title 'TOL = 10^-2', "rk25.csv" using 1:2 with lines lw 2 title 'TOL = 10^-5'

set ylabel 'v(x)'
set xlabel 'x'
set title 'RK2 v(x)'
plot "rk22.csv" using 3:4 with lines lw 2 title 'TOL = 10^-2', "rk25.csv" using 3:4 with lines lw 2 title 'TOL = 10^-5'


unset multiplot



set output "trapezy.png"

set multiplot layout 2, 2

set ylabel 'x(t)'
set xlabel 't'
set title 'trapezy x(t)'
plot "trapezy2.csv" using 1:3 with lines lw 2 title 'TOL = 10^-2', "trapezy5.csv" using 1:3 with lines lw 2 title 'TOL = 10^-5'


set ylabel 'v(t)'
set xlabel 't'
set title 'trapezy v(t)'
plot "trapezy2.csv" using 1:4 with lines lw 2 title 'TOL = 10^-2', "trapezy5.csv" using 1:4 with lines lw 2 title 'TOL = 10^-5'

set ylabel 'dt(t)'
set xlabel 't'
set title 'trapezy dt(t)'
plot "trapezy2.csv" using 1:2 with lines lw 2 title 'TOL = 10^-2', "trapezy5.csv" using 1:2 with lines lw 2 title 'TOL = 10^-5'

set ylabel 'v(x)'
set xlabel 'x'
set title 'trapezy v(x)'
plot "trapezy2.csv" using 3:4 with lines lw 2 title 'TOL = 10^-2', "trapezy5.csv" using 3:4 with lines lw 2 title 'TOL = 10^-5'


unset multiplot