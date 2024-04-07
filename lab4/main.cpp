#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <math.h>
#include <algorithm>
#include <chrono>

#include "lokalna.h"
#include "globalna.h"

int main(int argc, char **argv)
{
    auto start_time = std::chrono::high_resolution_clock::now();


    std::ofstream sOut;
    std::ofstream heatmapOut;
    std::ofstream heatmapErr;
    std::ofstream iteracje;
    iteracje.open("iteracje.txt");
    sOut.open("rg06s.csv");
    heatmapOut.open("rg06h.dat");
    heatmapErr.open("rg06err.dat");

    // Zadanie - relaksacja globalna dla 0.6 i 1
    globalna(0.6, sOut, heatmapOut, heatmapErr, iteracje);

    sOut.close();
    heatmapOut.close();
    heatmapErr.close();

    sOut.open("rg1s.csv");
    heatmapOut.open("rg1h.dat");
    heatmapErr.open("rg1err.dat");

    globalna(1, sOut, heatmapOut, heatmapErr, iteracje);

    sOut.close();
    heatmapOut.close();
    heatmapErr.close();

    // Zadanie - relaksacja lokalna dla 1, 1.4, 1.8, 1.9
    sOut.open("rl1s.csv");
    lokalna(1, sOut, iteracje);
    sOut.close();

    sOut.open("rl14s.csv");
    lokalna(1.4, sOut, iteracje);
    sOut.close();

    sOut.open("rl18s.csv");
    lokalna(1.8, sOut, iteracje);
    sOut.close();

    sOut.open("rl19s.csv");
    lokalna(1.9, sOut, iteracje);
    sOut.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    std::cout << time / std::chrono::milliseconds(1) << "ms to run.\n";
    iteracje.close();


    return 0;
}