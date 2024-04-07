set term png size 4000,2000
set output "./wyniki_wykresy/wykresy.png"

set multiplot layout 2, 2

set ylabel 'y'
set xlabel 'x'

set view map

set title 'vx(x,y)'
splot "./build/vx.dat" using 1:2:3 with image notitle

set title 'vy(x,y)'
splot "./build/vy.dat" using 1:2:3 with image notitle

unset view

set ylabel 'c(t)'
set xlabel 't'

set title 'c(t)'

plot "./build/d0c.dat" with lines lw 2 t "c(D=0)", "./build/d01c.dat" with lines lw 2 t "c(D=0.1)"

set ylabel 'xs(t)'
set xlabel 't'

set title 'xsr(t)'

plot "./build/d0xsr.dat" with lines lw 2 t "xsr(D=0)", "./build/d01xsr.dat" with lines lw 2 t "xsr(D=0)"
unset multiplot


