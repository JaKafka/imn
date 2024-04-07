
reset
set term gif size 800,300 animate delay 10
set output "./wyniki_wykresy/animd00.gif"
n=50    #liczba klatek
set view map # widok z gory
set size ratio -1
set cbr [0:]

set title 'u(x,y) w czasie, D=0'

do for [i=1:n] {
  file = sprintf("output/d0zad5_it=%i.txt",i)
  splot file u 1:2:3 w pm3d  title sprintf("t=%i",i)
}

reset
set term gif size 800,300 animate delay 10
set output "./wyniki_wykresy/animd01.gif"
n=50    #liczba klatek
set view map # widok z gory
set size ratio -1
set cbr [0:]

set title 'u(x,y) w czasie, D=0.1'

do for [i=1:n] {
  file = sprintf("output/d01zad5_it=%i.txt",i)
  splot file u 1:2:3 w pm3d  title sprintf("t=%i",i)
} 