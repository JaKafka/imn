mkdir -p output
mkdir -p build
cd build
cmake ..
make
./main
cd ..

mkdir -p wyniki_wykresy
gnuplot animacja.gp
gnuplot wykresy.gp