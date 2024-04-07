#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <math.h>
#include <algorithm>
#include <array>
#include <thread>
#include <barrier>

#include "helper.h"

void globalna(const double WG, std::ofstream &stream, std::ofstream &heatmap, std::ofstream &heatmapErr)
{
    // const double EPSILON = 1; -- nie wplywa na wynik
    const double DELTA = 0.1;
    const int NX = 150;
    const int NY = 100;

    const double V1 = 10;
    // const double V2 = 0; -- optymalizacja

    const double TOL = 1e-8;

    // Przygotuj macierz
    std::array<std::array<double, NY + 1>, NX + 1> VS;
    std::array<std::array<double, NY + 1>, NX + 1> VN;
    std::array<std::array<double, NY + 1>, NX + 1> ERR;

    for (int i = 0; i <= NX; i++)
    {
        for (int j = 0; j <= NY; j++)
        {
            VS[i][j] = 0.0;
        }
    }

    // Przygotuj warunki brzegowe
    for (int i = 0; i <= NX; i++)
    {
        VS[i][0] = V1;
    }

    // Wypełnij potencjał aby go potem nie liczyć co element
    std::array<std::array<double, NY + 1>, NX + 1> RHO;
    for (int i = 0; i <= NX; i++)
    {
        for (int j = 0; j <= NY; j++)
        {
            RHO[i][j] = gc(i * DELTA, j * DELTA);
        }
    }

    double oldS = 1;

    /*
        Trochę liczenia podziału pracy
    */

    const int thread_number = std::thread::hardware_concurrency()/4; // Po co i dlaczego nie wszystkie? Wątki powinny pracować jak najdłużej! (i w większej skali, do pozostałych można wsadzić dalsze rozwiązania!)
    std::vector<std::thread> threads;
    const int chunk = std::ceil(static_cast<float>(NX) / thread_number);
    std::barrier sync(thread_number);
    std::vector<double> partialS(thread_number, 0);

    /**
     * Co tu się dzieje?
     * Siatka jest dzielona na kawałki które nastepnie są przypisywane odpowiednim wątkom
     * Wątek liczy swoją część siatki, czekając na inne wątki w miejscach w których przejście do następnego kroku wymaga informacji z kawałka innego
     * Wątek o ID 0 posiada dodatkowe zadanie synchronizacji reszty i wykonywania operacji zapisu wyniku oraz sumowania S (z części policzonych przez inne wątki)
    */

    for (int t = 0; t < thread_number; t++)
    {

        int starting = t * chunk;
        int ending = (t + 1) * chunk;

        if (ending > NX)
            ending = NX;
        if (starting == 0)
            starting = 1;
        // std::cout << "Thread " << i << " " << starting << " " << ending << std::endl;

        bool done = false;

        threads.push_back(
            std::thread(

                [&VN, &VS, DELTA, &RHO, NX, &sync, WG, &partialS, t, &oldS, &stream, thread_number, &done, TOL, &ERR](int start, int end)
                    -> void
                {
                    for (int k = 0; k < 100000; k++) // Zabezpieczenie przed nieskończonym wywołaniem
                    {
                        // Policz VN
                        for (int i = start; i < end; i++)
                            for (int j = 1; j < NY; j++)
                            {
                                VN[i][j] = 0.25 * (VS[i + 1][j] + VS[i - 1][j] + VS[i][j + 1] + VS[i][j - 1] + DELTA * DELTA * RHO[i][j]);
                            }

                        // Trzeba poczekać na resztę
                        sync.arrive_and_wait();

                        // Warunki brzegowe aktualizujemy raz, ale po obliczeniu VN
                        if (t == 0)
                        {
                            for (int j = 1; j < NY; j++)
                            {
                                VN[0][j] = VN[1][j];
                                VN[NX][j] = VN[NX - 1][j];
                            }
                            sync.arrive_and_wait();
                        }
                        else
                        {
                            // Wątek musi poczekać na warunki brzegowe
                            sync.arrive_and_wait();
                        }

                        if (start == 1)
                            start = 0;
                        if (end == NX)
                            end = NX + 1;

                        for (int i = start; i < end; i++)
                        {
                            for (int j = 1; j < NY; j++)
                            {
                                VS[i][j] = (1 - WG) * VS[i][j] + WG * VN[i][j];
                            }
                        }

                        sync.arrive_and_wait();

                        // Trzeba policzyć S
                        if (end == NX + 1)
                            end = NX;

                        for (int i = start; i < end; i++)
                        {
                            for (int j = 0; j < NY; j++)
                            {
                                partialS[t] += DELTA * DELTA * (0.5 * std::pow((VS[i + 1][j] - VS[i][j]) / DELTA, 2) + 0.5 * std::pow((VS[i][j + 1] - VS[i][j]) / DELTA, 2) - RHO[i][j] * VS[i][j]);
                            }
                        }

                        sync.arrive_and_wait();

                        // Pierwszy wątek dostaje trochę dodatkowej pracy z synchronizacją i wypisywaniem

                        if (t == 0)
                        {

                            double newS = 0;
                            for (int i = 0; i < thread_number; i++)
                            {
                                newS += partialS[i];
                                partialS[i] = 0;
                            }
                            stream << k << " " << newS << std::endl;

                            // if (k % 5000 == 0)
                            //     std::cout << k << std::endl; // nadal żyję!

                            if (std::abs((newS - oldS) / oldS) < TOL)
                            {
                                done = true;
                            }

                            oldS = newS;
                        }

                        sync.arrive_and_wait();

                        // Koniec pracy, policz błąd dla swojego kawałka i wyjdź
                        if (done)
                        {
                            
                            if (start == 0)
                                start = 1;
                            for (int i = start; i < end; i++)
                            {
                                for (int j = 0; j < NY; j++)
                                {
                                    ERR[i][j] = (VS[i + 1][j] + VS[i - 1][j] + VS[i][j + 1] + VS[i][j - 1] - 4 * VS[i][j]) / (DELTA * DELTA) + RHO[i][j];
                                }
                            }

                            return;
                        }
                    }
                },
                starting, ending));
    }

    // Zbieramy wątki

    for (int i = 0; i < thread_number; i++)
    {
        threads[i].join();
    }
    threads.clear();

    // Trzeba jeszcze tylko policzyć błąd, ciągłe wywoływanie operatora << dla streamów jest wolniejsze od zapisania informacji do buforu i wysłania jej całej

    std::string errbuff = "";
    std::string vsbuff = "";

    for (int j = NY - 1; j > 0; j--)
    {
        for (int i = 1; i < NX; i++)
        {
            vsbuff += std::to_string(i * DELTA) + ' ' + std::to_string(j * DELTA) + ' ' + std::to_string(VS[i][j]) + "\n";
            errbuff += std::to_string(i * DELTA) + ' ' + std::to_string(j * DELTA) + ' ' + std::to_string(ERR[i][j]) + "\n";
            // heatmap << i * DELTA << ' ' << j * DELTA << ' ' << VS[i][j] << std::endl;
            // heatmapErr << i * DELTA << ' ' << j * DELTA << ' ' << ERR[i][j] << std::endl; // Konwolucja 2D operatora Laplasjanu i macierzy V - rho
        }
    }

    heatmap << vsbuff;
    heatmapErr << errbuff;
}