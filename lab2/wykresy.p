
set term png

set datafile separator ';'

set xlabel 'czas t'
set ylabel 'populacja'

set output "picard.png"
set title 'Metoda Picarda'
plot "picard.csv" using 1:2 with lines lw 2 title 'u(t)', '' using 1:3 with lines lw 2 title 'z(t) = N-u(t)'

set output "newton.png"
set title 'Metoda Newtona'
plot "newton.csv" using 1:2 with lines lw 2 title 'u(t)', '' using 1:3 with lines lw 2 title 'z(t) = N-u(t)'

set output "rk2.png"
set title 'Niejawna metoda RK2'
plot "rk2.csv" using 1:2 with lines lw 2 title 'u(t)', '' using 1:3 with lines lw 2 title 'z(t) = N-u(t)'