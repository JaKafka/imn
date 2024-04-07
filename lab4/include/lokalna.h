#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <math.h>
#include <algorithm>

#include "helper.h"

void lokalna(const double WL, std::ofstream &stream, std::ofstream &iteracje)
{

    const double DELTA = 0.1;
    const int NX = 150;
    const int NY = 100;
    const double EPSILON = 1;
    const double V1 = 10;
    const double V2 = 0;
    const double TOL = 1e-8;

    const double XMAX = DELTA * NX;
    const double YMAX = DELTA * NY;

    // Przygotuj macierz
    std::array<std::array<double, NY + 1>, NX + 1> V;
    std::array<std::array<double, NY + 1>, NX + 1> VTEMP;

    double SO = 1;

    for (int i = 0; i <= NX; i++)
    {
        for (int j = 0; j <= NY; j++)
        {
            V[i][j] = 0.0;
        }
    }

    // Przygotuj warunki brzegowe
    for (int i = 0; i <= NX; i++)
    {
        V[i][0] = V1;
        V[i][NY] = V2;
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

    // Iteracja aż do TOL
    for (int k = 0; k < 100000; k++) // Zabezpieczenie przed nieskończonym wykonaniem
    {
        for (int i = 1; i < NX; i++)
        {
            for (int j = 1; j < NY; j++)
            {
                V[i][j] = (1.0 - WL) * V[i][j] + 0.25 * WL * (V[i + 1][j] + V[i - 1][j] + V[i][j + 1] + V[i][j - 1] +  DELTA * DELTA * RHO[i][j]);
            }
        }

        // Przeniesienie warunków brzegowych Neumanna
        for (int j = 1; j < NY; j++)
        {
            V[0][j] = V[1][j];
            V[NX][j] = V[NX - 1][j];
        }

        if (k % 1000 == 0)
            std::cout << k << std::endl;

        double newS = 0;
        // Trzeba policzyć S
        for (int i = 0; i < NX; i++)
        {
            for (int j = 0; j < NY; j++)
            {
                 newS += DELTA * DELTA * (0.5 * std::pow((V[i + 1][j] - V[i][j]) / DELTA, 2) + 0.5 * std::pow((V[i][j + 1] - V[i][j]) / DELTA, 2) - RHO[i][j] * V[i][j]);
            }
        }

        if (std::abs((newS - oldS) / oldS) < TOL)
        {
            iteracje << "Lokalna dla " << WL << " iteracje " << k << std::endl;
            break;
        }

        oldS = newS;

        stream << k << " " << oldS << std::endl;
    }

    // for (int j = NY; j >= 0; j--)
    // {
    //     for (int i = 0; i <= NX; i++)
    //     {
    //         heatmap << i * DELTA << ' ' << j * DELTA << ' ' << V[i][j] << std::endl;
    //     }
    // }
}