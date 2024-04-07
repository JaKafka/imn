set output "potencjal.png"
set term png size 1000,1000



set grid
set pm3d
unset surface
set view map

set ylabel 'y'
set xlabel 'x'

set palette defined (-10 "blue", 0 "white", 10 "red")
set size ratio -1

set multiplot layout 2,3

set cbrange [-10:10]


set title 'V(x,y) nx=ny=50 \e_1=1, \e_2 = 1'
splot "5apotencjal.dat" using 1:2:3 w pm3d notitle


set title 'V(x,y) nx=ny=100'

splot "5bpotencjal.dat" using 1:2:3 w pm3d notitle

set title 'V(x,y) nx=ny=200'
splot "5cpotencjal.dat" using 1:2:3 w pm3d notitle

set palette defined (-0.8 "blue", 0 "white", 0.8 "red")
set cbrange [-0.8:0.8]

set title 'V(x,y) nx=ny=100, e_1=1, e_2=1'
splot "6apotencjal.dat" using 1:2:3 w pm3d notitle

set title 'V(x,y) nx=ny=100, ne_1=1, e_2=2'
splot "6bpotencjal.dat" using 1:2:3 w pm3d notitle

set title 'V(x,y) nx=ny=100, e_1=1, e_2=10'
splot "6cpotencjal.dat" using 1:2:3 w pm3d notitle



unset multiplot
unset view

