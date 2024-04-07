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

double ff(double v) { return v; }
double gg(double x, double v, double alpha) { return alpha * (1 - x * x) * v - x; }

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

// xn+1, vn+1
std::pair<double, double> trapezy(double xn, double vn, double dt, double alpha)
{
    double dx, dv;
    double xn1 = xn, vn1 = vn;

    do
    {

        // Policz F i G
        const double F = xn1 - xn - dt * 0.5 * (ff(vn) + ff(vn1));
        const double G = vn1 - vn - dt * 0.5 * (gg(xn, vn, alpha) + gg(xn1, vn1, alpha));

        // Policz aij
        const double a11 = 1;
        const double a12 = -0.5 * dt;
        const double a21 = -0.5 * dt * (-2 * alpha * xn1 * vn1 - 1);
        const double a22 = 1 - 0.5 * dt * alpha * (1 - xn1 * xn1);

        // Policz dx i dv
        dx = (-F * a22 - (-G) * a12) / (a11 * a22 - a12 * a21);
        dv = (a11 * (-G) - a21 * (-F)) / (a11 * a22 - a12 * a21);

        xn1 = xn1 + dx;
        vn1 = vn1 + dv;

    } while (std::abs(dx) < 10e-10 && std::abs(dv) < 10e-10);

    return std::pair<double,double>(xn1, vn1);
}


std::pair<double, double> rk2(double xn, double vn, double dt, double alpha)
{
    double k1x = vn;
    double k1v = alpha * (1.0 - xn * xn) * vn - xn;

    double k2x = vn + dt * k1v;
    double k2v = alpha * (1.0 - (xn + dt * k1x) * (xn + dt * k1x) ) * (vn + dt * k1v) - (xn + dt * k1x);

    return std::pair<double,double>(xn + dt * 0.5 * (k1x + k2x), vn + dt * 0.5 * (k1v + k2v));
}

void kkc(std::pair<double, double> (*schemat)(double, double, double, double), const double TOL, std::ofstream & stream)
{
    double t = 0, dt = 1, xn = 0.01, vn = 0;

    const double alpha = 5, tmax = 40, S = 0.75;

    do
    {
        // 2 kroki
        std::pair<double,double> dok2 = schemat(xn, vn, dt, alpha);
        dok2 = schemat(dok2.first, dok2.second, dt, alpha);

        // 1 duzy krok
        std::pair<double,double> dok1 = schemat(xn, vn, dt * 2, alpha);

        // Ex i Ev
        const double Ex = (dok2.first - dok1.first) / 3.0;
        const double Ev = (dok2.second - dok1.second) / 3.0;

        if (std::max(std::abs(Ex), std::abs(Ev)) < TOL)
        {
            t = t + 2 * dt;
            xn = dok2.first;
            vn = dok2.second;
            stream << t << ';' << dt << ';' << xn << ';' << vn << '\n';
        }

        dt = std::cbrt((S * TOL) / std::max(std::abs(Ex), std::abs(Ev))) * dt;

    } while (t < tmax);
    
}

int main(int argc, char **argv)
{

    std::ofstream output;

    // RK2 -- 10^-2
    output.open("rk22.csv");
    kkc(rk2,  1e-2, output);
    output.close();

    // RK2 -- 10^-5
    output.open("rk25.csv");
    kkc(rk2,  1e-5, output);
    output.close();

    // Trapezy -- 10^-2
    output.open("trapezy2.csv");
    kkc(trapezy,  1e-2, output);
    output.close();

    // Trapezy -- 10^-5
    output.open("trapezy5.csv");
    kkc(trapezy,  1e-5, output);
    output.close();

    return 0;
}