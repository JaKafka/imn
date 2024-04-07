reset
set term png size 3000, 6000 enhanced font "Helvetica, 32" lw 2


set lmargin  1.5
set bmargin  1.5
set rmargin  1.5
set tmargin  1.0

set size ratio 0.5


set out "out.png"

set multiplot layout 6,1
set xlabel "t"
set ylabel "E"
set title "E(t)"
plot "Ev00.dat" with lines title "\alpha = 0, \beta = 0", "Ev001.dat" with lines title "\alpha = 0, \beta = 0.1", "Ev01.dat" with lines title "\alpha = 0, \beta = 1"


#------------------------------------------------------------------------

set grid
set view map
unset surface
set pm3d

#------------------------------------------------------------------------
set title "u(x,t) \alpha = 0, \beta = 0"
set xlabel "t"
set ylabel "x"

set palette defined (-1 "blue", 0 "white", 1 "red")
set title "u(x,t)  \alpha = 0 \beta = 0"
splot [1:50][1:16] "v00.dat" u 2:1:3 notitle
set title "u(x,t)  \alpha = 0 \beta = 0.1"
splot [1:50][1:16] "v001.dat" u 2:1:3 notitle
set title "u(x,t) \alpha = 0 \beta = 1"
splot [1:50][1:16] "v01.dat" u 2:1:3 notitle

unset grid
unset view
unset pm3d

set xlabel "E"
set title "E(t) \alpha = 1 \beta = 1 z wymuszeniem"
plot "Ev11.dat" with lines

set xlabel "x"

set grid
set view map
unset surface
set pm3d
set title "u(x,t)  \alpha = 1 \beta = 1 z wymuszeniem"
splot [1:50][1:16] "v11.dat" u 2:1:3 notitle





unset multiplot
reset
