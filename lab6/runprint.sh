#!/bin/bash

mkdir build
cd ./build
cmake ..
make
./main
cd ..

mv ./build/*.dat ./draw/
mv ./build/*aorazb.txt ./draw/

cd draw
gnuplot output.gp
