#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

inline int kronecker(int a, int b)
{
    if (a == b)
        return 1;
    return 0;
}

inline double af(int x, int t, double dt, int tmax, int xF)
{
    return std::cos((50 * t * dt) / (tmax * dt)) * kronecker(x, xF);
}

void verlet(const double alpha, const double beta, std::string name, bool wymuszenie)
{
    const int nx = 150;
    const int nt = 1000;
    const double delta = 0.1;
    const double dt = 0.05;
    const double xA = 7.5;
    const double omega = 0.5;

    const double xF = 2.5;
    const int xFint = 25;

    // Inicjalizacja tablic
    std::vector<double> u0 = std::vector<double>(nx + 1, 0);
    std::vector<double> u = std::vector<double>(nx + 1, 0);
    std::vector<double> v = std::vector<double>(nx + 1, 0);
    std::vector<double> vp = std::vector<double>(nx + 1, 0);
    std::vector<double> a = std::vector<double>(nx + 1, 0);

    // WB spełnione - tablice wypełnione zerami na początku, teraz wypełniam 1 do nx - 1

    for (int i = 1; i < nx; i++)
    {
        if (!wymuszenie)
        {
            u[i] = std::exp(-1 * std::pow(i * delta - xA, 2) / (2 * omega * omega));
        }
    }

    u0 = u;

    for (int i = 1; i < nx; i++)
    {
        if (!wymuszenie)
        {
            a[i] = (u[i + 1] - 2 * u[i] + u[i - 1]) / (delta * delta) - beta * (u[i] - u0[i]) / dt;
        }
    }

    std::ofstream xstream;
    xstream.open(name);

    std::ofstream estream;
    estream.open("E" + name);

    for (int n = 0; n <= nt; n++)
    {
        // std::cout << n << " " << u[nx] << " " << v[nx] << std::endl;

        // vp = v + dt/2 a
        for (int i = 1; i < nx; i++)
        {
            vp[i] = v[i] + 0.5 * dt * a[i];
        }

        for (int i = 0; i < nx; i++)
        {
            u0[i] = u[i];
        }

        // u = u + dt vp
        for (int i = 1; i < nx; i++)
        {
            u[i] = u0[i] + dt * vp[i];
        }

        // Duży wzór
        for (int i = 1; i < nx; i++)
        {
            if (!wymuszenie)
            {
                a[i] = (u[i + 1] - 2 * u[i] + u[i - 1]) / (delta * delta) - beta * (u[i] - u0[i]) / dt;
            } else
            {
                a[i] = (u[i + 1] - 2 * u[i] + u[i - 1]) / (delta * delta) - beta * (u[i] - u0[i]) / dt + alpha * af(i, n, dt, nt, xFint);
            }
        }

        // v = vp + dt/2 a
        for (int i = 1; i < nx; i++)
        {
            v[i] = vp[i] + 0.5 * dt * a[i];
        }

        double En = (delta / 4) * ( std::pow((u[1] - u[0])/delta, 2) + std::pow((u[nx] - u[nx - 1]) / delta, 2));

        double sum = 0;

        for (int i = 1; i < nx; i++)
        {
            sum += std::pow(v[i], 2) + std::pow((u[i+1] - u[i - 1]) / (2 * delta), 2);
        }

        En = En + 0.5 * delta * sum;

        estream << n * dt << " " << En << std::endl;

        // Wypisz u

        for (int i = 0; i <= nx; i++)
        {
            xstream << i * delta << " " << n * dt << " " << u[i] << std::endl;
        }
        xstream << std::endl;
    }

    xstream.close();
    estream.close();
}