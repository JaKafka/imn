#!/bin/bash

mkdir build
cd ./build
cmake ..
make
./main
cd ..
mv ./build/*.csv ./
mv ./build/*.dat ./
mv ./build/iteracje.txt ./
gnuplot wykres.gp
