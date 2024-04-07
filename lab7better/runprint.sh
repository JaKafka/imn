gfortran -O3 navier.f90 -o main.out
mkdir -p bin
mv main.out ./bin/
cd ./bin
./main.out
cd ..
mkdir -p output
mv ./bin/*.dat ./output/
cd output
gnuplot ../rysowanie.gp