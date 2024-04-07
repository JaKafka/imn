#pragma once

#include <vector>
#include <cmath>
#include <string>
#include <fstream>

using Siatka = std::vector<std::vector<double>>;

void printGrid(const Siatka &grid, std::string output, double delta, int k)
{
    std::ofstream stream;
    stream.open(output);

    if (k != 0)
    {
        for (int i = 0; i < grid.size(); i += k)
        {
            for (int j = 0; j < grid[0].size(); j += k)
            {

                stream << i * delta << " " << j * delta << " " << grid[i][j] << std::endl;
            }
            stream << std::endl;
        }
    }
    else
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {

                stream << i * delta << " " << j * delta << " " << grid[i][j] << std::endl;
            }
        }
    }

    stream.close();
}