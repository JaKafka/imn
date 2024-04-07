#!/bin/bash

mkdir build
cd ./build
cmake ..
make
./main
cd ..
mv ./build/*.csv ./
gnuplot wykresy.p