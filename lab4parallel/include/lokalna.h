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

void lokalna(const double WL, std::ofstream &stream /*, std::ofstream &heatmap*/)
{

    const double DELTA = 0.1;
    const int NX = 150;
    const int NY = 100;
    // const double EPSILON = 1; -- 1, uzywana do mnozenia i dzielenia nie wplywa na wynik
    const double V1 = 10;
    // const double V2 = 0; -- optymalizacja
    const double TOL = 1e-8;

    // Przygotuj macierz
    std::array<std::array<double, NY + 1>, NX + 1> V;

    for (int i = 0; i <= NX; i++)
    {
        for (int j = 1; j <= NY; j++)
        {
            V[i][j] = 0.0;
        }
    }

    // Przygotuj warunki brzegowe
    for (int i = 0; i <= NX; i++)
    {
        V[i][0] = V1;
        // V[i][NY] = V2; -- optymalizacja
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

    const int thread_number = std::thread::hardware_concurrency() / 4; // Po co i dlaczego nie wszystkie? Wątki powinny pracować jak najdłużej! (i w większej skali, do pozostałych można wsadzić dalsze rozwiązania!)
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

                [&V, DELTA, &RHO, NX, &sync, WL, &partialS, t, &oldS, &stream, thread_number, &done, TOL](int start, int end)
                    -> void
                {
                    for (int k = 0; k < 100000; k++) // Zabezpieczenie przed nieskończonym wywołaniem
                    {
                        // Policz VN
                        for (int i = start; i < end; i++)
                            for (int j = 1; j < NY; j++)
                            {
                                V[i][j] = (1.0 - WL) * V[i][j] + 0.25 * WL * (V[i + 1][j] + V[i - 1][j] + V[i][j + 1] + V[i][j - 1] + DELTA * DELTA * RHO[i][j]);
                            }

                        // Trzeba poczekać na resztę
                        sync.arrive_and_wait();

                        // Warunki brzegowe aktualizujemy raz, ale po obliczeniu VN
                        if (t == 0)
                        {
                            for (int j = 1; j < NY; j++)
                            {
                                V[0][j] = V[1][j];
                                V[NX][j] = V[NX - 1][j];
                            }
                            sync.arrive_and_wait();
                        }
                        else
                        {
                            // Wątek musi poczekać na warunki brzegowe
                            sync.arrive_and_wait();
                        }

                        // Trzeba policzyć S

                        for (int i = start; i < end; i++)
                        {
                            for (int j = 0; j < NY; j++)
                            {
                                partialS[t] += DELTA * DELTA * (0.5 * std::pow((V[i + 1][j] - V[i][j]) / DELTA, 2) + 0.5 * std::pow((V[i][j + 1] - V[i][j]) / DELTA, 2) - RHO[i][j] * V[i][j]);
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

                        // Koniec pracy, tutaj błąd niepotrzebny
                        if (done)
                        {
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
}