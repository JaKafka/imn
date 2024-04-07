#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <math.h>
#include <algorithm>

#include "lokalna.h"
#include "globalna.h"

#include <chrono>

#include <thread>

int main(int argc, char **argv)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    std::ofstream sOut1;
    std::ofstream heatmapOut1;
    std::ofstream heatmapErr1;

    std::ofstream sOut2;
    std::ofstream heatmapOut2;
    std::ofstream heatmapErr2;

    std::ofstream sOut3;

    sOut1.open("rg06s.csv");
    heatmapOut1.open("rg06h.dat");
    heatmapErr1.open("rg06err.dat");

    sOut2.open("rg1s.csv");
    heatmapOut2.open("rg1h.dat");
    heatmapErr2.open("rg1err.dat");

    sOut3.open("rl1s.csv");

    std::ofstream iteracje;
    iteracje.open("iteracje.txt");

    // Relaksacja globalna wg = 0.6
    std::thread t1 = std::thread(globalna, 0.6, std::ref(sOut1), std::ref(heatmapOut1), std::ref(heatmapErr1));

    // Relaksacja globalna wg = 1
    std::thread t2 = std::thread(globalna, 1, std::ref(sOut2), std::ref(heatmapOut2), std::ref(heatmapErr2));

    // Relaksacja lokalna WL = 1
    std::thread t3 = std::thread(lokalna, 1, std::ref(sOut3));

    t1.join();

    sOut1.close();
    heatmapOut1.close();
    heatmapErr1.close();

    sOut1.open("rl14s.csv");

    // Relaksacja lokalna WL = 1.4
    t1 = std::thread(lokalna, 1.4, std::ref(sOut1));

    t2.join();

    sOut2.close();
    heatmapOut2.close();
    heatmapErr2.close();

    sOut2.open("rl18s.csv");

    // Relaksacja lokalna WL = 1.8
    t2 = std::thread(lokalna, 1.8, std::ref(sOut2));

    t3.join();
    sOut3.close();

    sOut3.open("rl19s.csv");

    // Relaksacja lokalna WL = 1.9
    t3 = std::thread(lokalna, 1.9, std::ref(sOut3));

    t1.join();
    sOut1.close();

    t2.join();
    sOut2.close();

    t3.join();
    sOut3.close();

    iteracje.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    std::cout << time / std::chrono::milliseconds(1) << "ms to run.\n";

    return 0;
}