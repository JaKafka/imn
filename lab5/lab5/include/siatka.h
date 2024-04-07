#pragma once

#include "helper.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

void zageszczanie(double delta, int nx, int ny, double TOL)
{
    const double xmax = delta * nx;
    const double ymax = delta * ny;

    Siatka V = Siatka(nx + 1, std::vector<double>(ny + 1, 0.0));

    double sOld = 1;

    
    std::ofstream iterStream;
    iterStream.open("ITERACJE.txt");
    for (int i = 0; i < nx; i++)
    {
        V[i][ny] = -1 * std::sin(2 * 3.14 * i * delta / xmax); // VB2
        V[i][0] = std::sin(2 * 3.14 * i * delta / xmax);       // VB4
    }

    for (int j = 0; j < ny; j++)
    {
        V[0][j] = std::sin(3.14 * j * delta / ymax);  // VB1
        V[nx][j] = std::sin(3.14 * j * delta / ymax); // VB 3
    }

    // printGrid(V, "starting.csv", delta, 0);

    int k = 16;

    std::ofstream sFile;
    sFile.open("s16.dat");

    for (int iter = 0; iter < 50000; iter++)
    {
        // Policzenie wartości
        for (int i = k; i <= nx - k; i += k)
        {
            for (int j = k; j <= ny - k; j += k)
            {
                V[i][j] = 0.25 * (V[i + k][j] + V[i - k][j] + V[i][j + k] + V[i][j - k]);
            }
        }

        // Policzenie S

        double s = 0;

        for (int i = 0; i <= nx - k; i += k)
        {
            for (int j = 0; j <= ny - k; j += k)
            {
                const double first = (V[i + k][j] - V[i][j]) / (2 * k * delta) + (V[i + k][j + k] - V[i][j + k]) / (2 * k * delta);
                const double second = (V[i][j + k] - V[i][j]) / (2 * k * delta) + (V[i + k][j + k] - V[i + k][j]) / (2 * k * delta);
                s += std::pow(k * delta, 2) * 0.5 * (std::pow(first, 2) + std::pow(second, 2));
            }
        }

        sFile << iter << " " << s << std::endl;

        // Warunek stopu / zagęszczenia
        if (std::abs((s - sOld) / sOld) < TOL)
        {
            iterStream << "Koniec iteracji dla k = " << k << ": " << iter << std::endl;
            // Zapisz uzyskaną siatkę
            printGrid(V, "s" + std::to_string(k) + "graph.dat", delta, k);

            // Zagęszczenie
            for (int i = 0; i <= nx - k; i += k)
            {
                for (int j = 0; j <= ny - k; j += k)
                {
                    const int k2 = k / 2;

                    if (i + k2 != nx && j + k != ny)
                        V[i + k2][j + k] = 0.5 * (V[i][j + k] + V[i + k][j + k]);

                    if (j != 0)
                        V[i + k2][j] = 0.5 * (V[i][j] + V[i + k][j]);

                    if (i + k != nx && j + k2 != ny)
                        V[i + k][j + k2] = 0.5 * (V[i + k][j] + V[i + k][j + k]);

                    if (i + k2 != nx && j + k2 != ny)
                        V[i + k2][j + k2] = 0.25 * (V[i][j] + V[i + k][j] + V[i][j + k] + V[i + k][j + k]);

                    if (i != 0)
                        V[i][j + k2] = 0.5 * (V[i][j] + V[i][j + k]);
                }
            }

            // Czasem zagęszczanie psuje warunki brzegowe
            // Można to rozwiązać tworząc łańcuszek ifów, albo po prostu je resetując
            // for (int j = 0; j <= ny; j++)
            // {
            //     V[0][j] = std::sin(3.14 * j * delta / ymax);  // VB1
            //     V[nx][j] = std::sin(3.14 * j * delta / ymax); // VB 3
            // }

            // for (int i = 1; i < nx; i++)
            // {
            //     V[i][ny] = -1 * std::sin(2 * 3.14 * i * delta / xmax); // VB2
            //     V[i][0] = std::sin(2 * 3.14 * i * delta / xmax);       // VB4
            // }

            k = k / 2;
            if (k == 0)
            {
                sFile.close();

                
                
                std::cout << "Max Iter: " << iter << std::endl;
                iterStream << "Max Iter: " << iter << std::endl;
                iterStream.close();
                break;
            }
            else
            {
                sFile.close();
                sFile.open("s" + std::to_string(k) + ".dat");
            }
        }

        sOld = s;
    }

    return;
}