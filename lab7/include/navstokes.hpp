#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

bool borders(const int nx, const int ny, const int i1, const int j1, const int i, const int j)
{
    if (i == 0)
        return true;
    if (j == ny)
        return true;
    if (j == 0)
        return true;
    if (i == nx)
        return true;

    if (i == i1 && j <= j1)
        return true;
    if (j == j1 && i <= i1)
        return true;

    return false;
}

void navStokes(const double delta, const double rho, const double mu, const int nx, const int ny, const int i1, const int j1, const int IT_MAX, const double Qwe, std::string nazwa)
{

    std::vector<std::vector<double>> psi(nx + 1, std::vector<double>(ny + 1, 0));
    std::vector<std::vector<double>> zeta(nx + 1, std::vector<double>(ny + 1, 0));

    const double Qwy = Qwe * (std::pow(ny * delta, 3) - std::pow(j1 * delta, 3) - 3 * j1 * delta * std::pow(ny * delta, 2) + 3 * std::pow(j1 * delta, 2) * ny * delta) / (std::pow(ny * delta, 3));

    // Warunki brzegowe dla psi

    // Brzeg A

    for (int j = j1; j <= ny; j++)
    {
        psi[0][j] = (Qwe / (2 * mu)) * (std::pow(delta * j, 3) / 3 - 0.5 * std::pow(j * delta, 2) * (j1 * delta + ny * delta) + j * delta * j1 * delta * ny * delta);
    }

    // Brzeg C

    for (int j = 0; j <= ny; j++)
    {
        psi[nx][j] = (Qwy / (2 * mu)) * (std::pow(delta * j, 3) / 3 - 0.5 * std::pow(j * delta, 2) * ny * delta) + (Qwe * std::pow(j1 * delta, 2) * (-delta * j1 + 3 * ny * delta)) / (12 * mu);
    }

    // Brzeg B

    for (int i = 1; i < nx; i++)
    {
        psi[i][ny] = psi[0][ny];
    }

    // Brzeg D

    for (int i = i1; i < nx; i++)
    {
        psi[i][0] = psi[0][j1];
    }

    // Brzeg E

    for (int j = 1; j <= j1; j++)
    {
        psi[i1][j] = psi[0][j1];
    }

    // Brzeg F

    for (int i = 1; i < i1; i++)
    {
        psi[i][j1] = psi[0][j1];
    }

    // Relaksacja NS

    double omega;

    for (int iter = 1; iter < IT_MAX; iter++)
    {
        if (iter < 2000)
        {
            omega = 0;
        }
        else
        {
            omega = 1;
        }

        for (int i = 1; i < nx; i++)
        {
            for (int j = 1; j < ny; j++)
            {
                if (!borders(nx, ny, i1, j1, i, j))
                {
                    psi[i][j] = 0.25 * (psi[i + 1][j] + psi[i - 1][j] + psi[i][j + 1] + psi[i][j - 1] - delta * delta * zeta[i][j]);
                    zeta[i][j] = 0.25 * (zeta[i + 1][j] + zeta[i - 1][j] + zeta[i][j + 1] + zeta[i][j - 1]) - (omega * rho / (16 * mu)) * ((psi[i][j + 1] - psi[i][j - 1]) * (zeta[i + 1][j] - zeta[i - 1][j]) - (psi[i + 1][j] - psi[i - 1][j]) * (zeta[i][j + 1] - zeta[i][j - 1]));
                }
            }
        }

        // WB ZETA

        // Brzeg A

        for (int j = j1; j <= ny; j++)
        {
            zeta[0][j] = (0.5 * Qwe / mu) * (2 * j * delta - j1 * delta - ny * delta);
        }

        // Brzeg C

        for (int j = 0; j <= ny; j++)
        {
            zeta[nx][j] = (0.5 * Qwy / mu) * (2 * j * delta - delta * ny);
        }

        // Brzeg B

        for (int i = 1; i < nx; i++)
        {
            zeta[i][ny] = (2 / (delta * delta)) * (psi[i][ny - 1] - psi[i][ny]);
        }

        // Brzeg D

        for (int i = i1; i < nx; i++)
        {
            zeta[i][0] = (2 / (delta * delta)) * (psi[i][1] - psi[i][0]);
        }

        // Brzeg E

        for (int j = 1; j < j1; j++)
        {
            zeta[i1][j] = (2 / (delta * delta)) * (psi[i1 + 1][j] - psi[i1][j]);
        }

        // Brzeg F

        for (int i = 1; i <= i1; i++)
        {
            zeta[i][j1] = (2 / (delta * delta)) * (psi[i][j1 + 1] - psi[i][j1]);
        }

        // Wierzchołek E/F

        zeta[i1][j1] = 0.5 * (zeta[i1 - 1][j1] + zeta[i1][j1 - 1]);

        double gamma = 0;
        int j2 = j1 + 2;
        for (int i = 1; i < nx; i++)
        {
            gamma += psi[i + 1][j2] + psi[i - 1][j2] + psi[i][j2 + 1] + psi[i][j2 - 1] - 4 * psi[i][j2] - delta * delta * zeta[i][j2];
        }

        std::cout << "Iter: " << iter << " gamma = " << gamma << std::endl;
    }

    // Wypisanie wyników

    std::ofstream outStream;
    outStream.open(nazwa);

    for (int i = 1; i < nx; i++)
    {
        for (int j = 1; j < ny; j++)
        {

            outStream << i * delta << " " << j * delta << " " << psi[i][j] << " " << zeta[i][j] << " " << (psi[i][j+1] - psi[i][j-1]) / (2 * delta) << " " << -(psi[i+1][j] - psi[i-1][j]) / (2 * delta) << std::endl;
        }
        outStream << std::endl;
    }

    outStream.close();

}
