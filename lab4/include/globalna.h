#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <math.h>
#include <algorithm>

#include "helper.h"

void globalna(const double WG, std::ofstream &stream, std::ofstream &heatmap, std::ofstream &heatmapErr , std::ofstream &iteracje)
{
    const double EPSILON = 1;
    const double DELTA = 0.1;
    const int NX = 150;
    const int NY = 100;

    const double V1 = 10;
    const double V2 = 0;

    const double XMAX = DELTA * NX;
    const double YMAX = DELTA * NY;

    const double TOL = 1e-8;

    // Przygotuj macierz
    std::array<std::array<double, NY + 1>, NX + 1> VS;
    std::array<std::array<double, NY + 1>, NX + 1> VN;

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
        VS[i][NY] = V2;
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

    for (int k = 0; k < 100000; k++) // Zabezpieczenie przed nieskończonym wykonaniem
    {
        for (int i = 1; i < NX; i++)
        {
            for (int j = 1; j < NY; j++)
            {
                VN[i][j] = 0.25 * (VS[i + 1][j] + VS[i - 1][j] + VS[i][j + 1] + VS[i][j - 1] + DELTA * DELTA * RHO[i][j]);
            }
        }

        for (int j = 1; j < NY; j++)
        {
            VN[0][j] = VN[1][j];
            VN[NX][j] = VN[NX - 1][j];
        }

        for (int i = 0; i <= NX; i++)
        {
            for (int j = 1; j < NY; j++)
            {
                VS[i][j] = (1 - WG) * VS[i][j] + WG * VN[i][j];
            }
        }

        // Czy skrypt żyje
        if (k % 1000 == 0)
            std::cout << k << std::endl;

        double newS = 0;
        // Trzeba policzyć S
        for (int i = 0; i < NX; i++)
        {
            for (int j = 0; j < NY; j++)
            {
                newS += DELTA * DELTA * (0.5 * std::pow((VS[i + 1][j] - VS[i][j]) / DELTA, 2) + 0.5 * std::pow((VS[i][j + 1] - VS[i][j]) / DELTA, 2) - RHO[i][j] * VS[i][j]);
            }
        }

        if (std::abs((newS - oldS) / oldS) < TOL)
        {
            iteracje << "Globalna dla " << WG << " iteracje: " << k << std::endl;
            break;
        }

        oldS = newS;

        stream << k << " " << newS << std::endl;
    }

    

    for (int j = NY - 1; j > 0; j--)
    {
        for (int i = 1; i < NX; i++)
        {
            heatmap << i * DELTA << ' ' << j * DELTA << ' ' << VS[i][j] << std::endl;
            heatmapErr << i * DELTA << ' ' << j * DELTA << ' ' << (VS[i + 1][j] + VS[i - 1][j] + VS[i][j + 1] + VS[i][j - 1] - 4 * VS[i][j]) / (DELTA * DELTA) + RHO[i][j] << std::endl; // Konwolucja 2D operatora Laplasjanu i macierzy V
        }
    }
}