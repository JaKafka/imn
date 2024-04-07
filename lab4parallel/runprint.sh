#!/bin/bash

# Kompilacja i uruchomienie
mkdir build
cd ./build
cmake ..
make
./main

# Tworzenie wykresow
cd ..
mkdir output
mv ./build/*.csv ./output/
mv ./build/*.dat ./output/
cp ./wykresy/* ./output/

cd ./output/
gnuplot rysowanie.gp
