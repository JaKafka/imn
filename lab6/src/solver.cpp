#include "solver.h"

// Rzeczywiste rozwiazywanie problemu
void Solver::solve()
{
    this->initialize();

    pmgmres_ilu_cr(_N, nz_num, ia, ja, a, V, b, 500, 500, 1e-8, 1e-8);

    wypiszWymaluj(_name + "potencjal");
}

void Solver::print(std::string name)
{
    std::ofstream stream;
    stream.open(name + ".dat");
    for (int l = 0; l < _N; l++)
    {
        // Policzenie wspolrzednych
        int j = std::floor(static_cast<double>(l) / (_nx + 1.0));
        int i = l - j * (_nx + 1);
        stream << i * _delta << " " << j * _delta << " " << a[l] << std::endl;
    }
    stream.close();
}

double Solver::el(int l)
{
    int j = std::floor(static_cast<double>(l) / (_nx + 1.0));
    int i = l - j * (_nx + 1);
    if (i <= _nx / 2)
        return _eps1;
    return _eps2;
}

// Wszystkie potrzebne warianty a(l,l)
double Solver::allnxm1(int l)
{
    return el(l) / (_delta * _delta);
}

double Solver::allm1(int l)
{
    return el(l) / (_delta * _delta);
}

double Solver::all(int l)
{
    return -1 * (2 * el(l) + el(l + 1) + el(l + _nx + 1)) / (_delta * _delta);
}

double Solver::allp1(int l)
{
    return el(l + 1) / (_delta * _delta);
}

double Solver::allnxp1(int l)
{
    return el(l + _nx + 1) / (_delta * _delta);
}

void Solver::wypiszWymaluj(std::string name)
{
    std::ofstream stream;
    stream.open(name + ".dat");
    int lastj = 0;
    for (int l = 0; l < _N; l++)
    {
        int j = std::floor(static_cast<double>(l) / (_nx + 1.0));
        int i = l - j * (_nx + 1);
        if (lastj != j)
        {

            stream << std::endl;
        }
        lastj = j;

        stream << i * _delta << " " << j * _delta << " " << V[l] << std::endl;
    }
    stream.close();
}

// Tworzy tabele poczatkowa
void Solver::initialize()
{
    a = new double[5 * _N];
    ja = new int[5 * _N];
    ia = new int[_N + 1];
    b = new double[_N];
    V = new double[_N];

    const double xmax = _delta * _nx;
    const double ymax = _delta * _ny;
    const double omega = xmax / 10;

    for (int i = 0; i < _N + 1; i++)
        ia[i] = -1;

    int k = -1;

    for (int l = 0; l < _N; l++)
    {
        // Policzenie wspolrzednych
        int j = std::floor(static_cast<double>(l) / (_nx + 1.0));
        int i = l - j * (_nx + 1);

        // Wykryj brzeg
        int brzeg = 0;
        double vb = 0;

        if (i == 0)
        {
            brzeg = 1;
            vb = _V1;
        }

        if (j == _ny)
        {
            brzeg = 1;
            vb = _V2;
        }

        if (i == _nx)
        {
            brzeg = 1;
            vb = _V3;
        }

        if (j == 0)
        {
            brzeg = 1;
            vb = _V4;
        }

        // Wektor wyrazow wolnych

        if (_rho)
        {
            double x = i * _delta;
            double y = j * _delta;
            double rho1 = std::exp(-1 * (std::pow(x - 0.25 * xmax, 2) / (omega * omega)) - (std::pow(y - 0.5 * ymax, 2) / (omega * omega)));
            double rho2 = -1 * std::exp(-1 * (std::pow(x - 0.75 * xmax, 2) / (omega * omega)) - (std::pow(y - 0.5 * ymax, 2) / (omega * omega)));
            b[l] = -(rho1 + rho2);
        }
        else
        {
            b[l] = 0;
        }

        if (brzeg == 1)
        {
            b[l] = vb;
        }

        ia[l] = -1;

        // Lewa skrajna
        if (l - _nx - 1 >= 0 && brzeg == 0)
        {
            k++;
            if (ia[l] < 0)
                ia[l] = k;
            a[k] = allnxm1(l);
            ja[k] = l - _nx - 1;
        }

        // Poddiagonala
        if (l - 1 >= 0 && brzeg == 0)
        {
            k++;
            if (ia[l] < 0)
                ia[l] = k;
            a[k] = allm1(l);
            ja[k] = l - 1;
        }

        // Diagonala
        k++;
        if (ia[l] < 0)
            ia[l] = k;
        if (brzeg == 0)
        {
            a[k] = all(l);
        }
        else
        {
            a[k] = 1;
        }
        ja[k] = l;

        // Naddiagonala

        if (1 < _N - _nx - 1 && brzeg == 0)
        {
            k++;
            a[k] = allnxp1(l);
            ja[k] = l + 1;
        }

        // Prawa skretna przekatna

        if ((1 < _N - _nx - 1) && brzeg == 0)
        {
            k++;
            a[k] = allnxp1(l);
            ja[k] = l + _nx + 1;
        }
    }

    nz_num = k + 1;
    ia[_N] = nz_num;

    /*Wypisz informacje debugowania*/

    if (_debug)
    {
        std::ofstream stream;

        stream.open(_name + "aorazb.txt");

        stream << "# l\ti_l\tj_l\tb[l]\n";
        for (int l = 0; l < _N; l++)
        {

            int j = std::floor(static_cast<double>(l) / (_nx + 1.0));

            int i = l - j * (_nx + 1);
            stream << l << '\t' << i << '\t' << j << '\t' << b[l] << std::endl;
        }

        stream << "\n\n# k\ta[k]\n";

        for (int c = 0; c < nz_num; c++)
        {
            stream << c << '\t' << a[c] << std::endl;
        }

        stream.close();
    }
}
