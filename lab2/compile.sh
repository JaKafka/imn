#!/bin/bash
mkdir build
cd build
cmake ..
make
echo "wykonywanie programu..."
./main

echo "przenosze wygenerowane pliki csv katalog wyzej..."
mv ./*.csv ../
cd ..

echo "generuje pliki .png z wykresami..."
gnuplot wykresy.p