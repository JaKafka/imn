mkdir -p build
cd build
cmake ..
make
./main
cd ..
mv ./build/*.dat ./output
cd output
gnuplot rysowanie.gp
cd ..
