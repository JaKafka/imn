#pragma once

#include <cmath>

double gc(double x, double y)
{
    const int NX = 150;
    const int NY = 100;
    const double DELTA = 0.1;
    const double XMAX = DELTA * NX;
    const double YMAX = DELTA * NY;

    double u1 = std::pow(x - 0.35 * XMAX , 2);
    double u2 = std::pow(y - 0.5 * YMAX, 2);
    double u3 = std::pow(x - 0.65 * XMAX, 2);

    double g1 = std::exp(-1 * u1 / (0.01 * XMAX * XMAX) - u2 / (0.01 * YMAX * YMAX));
    double g2 = std::exp(-1 * u3 / (0.01 * XMAX * XMAX) - u2 / (0.01 * YMAX * YMAX));

    return g1 - g2;
}
