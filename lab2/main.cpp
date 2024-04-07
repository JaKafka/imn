#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <math.h>

using DoubleArrays = std::vector<std::vector<double>>;

struct Point
{
    Point(double a = 0, double b = 0) : t(a), y(b) {}
    double t;
    double y;
};

DoubleArrays pointToDoubles(std::vector<Point> &points)
{
    DoubleArrays output = {std::vector<double>(), std::vector<double>()};
    for (int i = 0; i < points.size(); i++)
    {
        output[0].push_back(points[i].t);
        output[1].push_back(points[i].y);
    }

    return output;
}

// Funkcja pomocnicza eksportująca wektor wektorów double do pliku csv oddzielanego średnikami
void exportGNUPlot(DoubleArrays &values, const std::string &name)
{
    std::ofstream output;

    output.open(name);

    int len = values[0].size();

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < values.size(); j++)
        {
            output << values[j][i];

            if (j != values.size() - 1)
            {
                output << ";";
            }
        }
        output << "\n";
    }

    output.close();
}

double f(double u)
{
    const double beta = 0.001;
    const double N = 500;
    const double ggamma = 0.1;

    return (beta * N - ggamma) * u - beta * u * u;
}

// Metoda picarda
void picard(std::vector<Point> &ut)
{
    const double beta = 0.001;
    const double N = 500;
    const double ggamma = 0.1;
    const double tmax = 100;
    const double dt = 0.1;
    const double u0 = 1;
    const double TOL = 10e-6;
    const double alpha = beta * N - ggamma;

    double t = 0;
    double u = u0;
    while (t < tmax)
    {
        // Zapisz wartość w obecnym węźle
        ut.push_back(Point(t, u));

        // Do obliczenia kolejnego węzłą potrzebujemy un+1

        double uiter = u;

        for (int i = 0; i < 21; i++)
        {
            double bak = uiter;
            uiter = u + dt * 0.5 * ((alpha * u - beta * u * u) + (alpha * uiter - beta * uiter * uiter));
            if (std::abs(uiter - bak) < TOL)
                break;
        }

        // Gdy mamy un+1, podstawiamy do funkcji

        u = u + dt * 0.5 * (f(u) + f(uiter));

        t += dt;
    }
}

// Metoda newtona
void newton(std::vector<Point> &ut)
{

    const double beta = 0.001;
    const double N = 500;
    const double ggamma = 0.1;
    const double tmax = 100;
    const double dt = 0.1;
    const double u0 = 1;
    const double TOL = 10e-6;
    const double alpha = beta * N - ggamma;

    double t = 0;
    double u = u0;
    while (t < tmax)
    {
        // Zapisz wartość w obecnym węźle
        ut.push_back(Point(t, u));

        // Do obliczenia kolejnego węzłą potrzebujemy un+1

        double uiter = u;

        for (int i = 0; i < 21; i++)
        {
            double bak = uiter;

            double top = uiter - u - dt * 0.5 * ((alpha * u - beta * u * u) + (alpha * uiter - beta * uiter * uiter));
            double bottom = 1 - dt * 0.5 * (alpha - 2 * beta * uiter);

            uiter = uiter - top / bottom;

            if (std::abs(uiter - bak) < TOL)
                break;
        }

        // Gdy mamy un+1, podstawiamy do funkcji

        u = u + dt * 0.5 * (f(u) + f(uiter));

        t += dt;
    }
}

// Metoda RK2
void rk2(std::vector<Point> &ut)
{

    const double beta = 0.001;
    const double N = 500;
    const double ggamma = 0.1;
    const double tmax = 100;
    const double dt = 0.1;
    const double u0 = 1;
    const double TOL = 10e-6;
    const double alpha = beta * N - ggamma;

    const double c1 = 0.5 - std::sqrt(3) / 6.0;
    const double c2 = 0.5 + std::sqrt(3) / 6.0;

    const double a11 = 0.25;
    const double a12 = 0.25 - std::sqrt(3) / 6.0;
    const double a21 = 0.25 + std::sqrt(3) / 6.0;
    const double a22 = 0.25;

    const double b1 = 0.5;
    const double b2 = 0.5;

    double t = 0;
    double u = u0;
    while (t < tmax)
    {
        // Zapisz wartość w obecnym węźle
        ut.push_back(Point(t, u));

        // Do obliczenia un+1 potrzebuje U1 i U2

        double U1 = u;
        double U2 = u;

        for (int i = 0; i < 21; i++)
        {

            double F1 = U1 - u - dt * (a11 * (alpha * U1 - beta * U1 * U1) + a12 * (alpha * U2 - beta * U2 * U2));

            double F2 = U2 - u - dt * (a21 * (alpha * U1 - beta * U1 * U1) + a22 * (alpha * U2 - beta * U2 * U2));

            double m11 = 1 - dt * a11 * (alpha - 2 * beta * U1);
            double m12 = -1 * dt * a12 * (alpha - 2 * beta * U2);
            double m21 = -1 * dt * a21 * (alpha - 2 * beta * U1);
            double m22 = 1 - dt * a22 * (alpha - 2 * beta * U2);

            double dU1 = (F2 * m12 - F1 * m22) / (m11 * m22 - m12 * m21);
            double dU2 = (F1 * m21 - F2 * m11) / (m11 * m22 - m12 * m21);

            U1 = U1 + dU1;
            U2 = U2 + dU2;
        }

        u = u + dt * (b1 * f(U1) + b2 * f(U2));

        t += dt;
    }
}

// Funkcja pomocnicza, w tym zadaniu tworzy także trzecią kolumnę v(t) = N - u(t)
void exportHelper(std::vector<Point> &punkty, const std::string &name)
{
    DoubleArrays temparray = pointToDoubles(punkty);
    temparray.push_back(std::vector<double>());

    for (int i = 0; i < temparray[0].size(); i++)
    {
        temparray[2].push_back(500 - temparray[1][i]);
    }

    exportGNUPlot(temparray, name);
}

int main(int argc, char **argv)
{

    std::vector<Point> picardpt;
    std::vector<Point> newtonpt;
    std::vector<Point> rk2pt;

    // Użyj metod do wygenerowania danych
    picard(picardpt);
    newton(newtonpt);
    rk2(rk2pt);

    // Funkcje pomocnicze eksportujące dane
    exportHelper(picardpt, "picard.csv");
    exportHelper(newtonpt, "newton.csv");
    exportHelper(rk2pt, "rk2.csv");

    return 0;
}