
set output "wyniki.png"
set term png size 2000,1000

set view map
set multiplot layout 2,1

unset grid

#set xrange [-1:26]
#set yrange [-1:26]
#set cbrange [-1:1]

set ylabel 'y'
set xlabel 'x'

set title 'vx(x,y)'
splot "vx.dat" using 1:2:3 with image notitle

set title 'vy(x,y)'
splot "vy.dat" using 1:2:3 with image notitle


unset multiplot
unset view
