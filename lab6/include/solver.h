#pragma once

#include "mgmres.h"

#include <string>
#include <fstream>
#include <cmath>

class Solver
{
public:
    Solver(int nx, int ny, double V1, double V2, double V3, double V4, double delta, double eps1, double eps2, std::string name, bool debug, bool rho)
    {
        _nx = nx;
        _ny = ny;
        _V1 = V1;
        _V2 = V2;
        _V3 = V3;
        _V4 = V4;
        _delta = delta;
        
        _eps1 = eps1;
        _eps2 = eps2;

        a = nullptr;
        ja = nullptr;
        ia = nullptr;
        b = nullptr;
        V = nullptr;

        _N = (_nx + 1) * (_ny + 1);

        _name = name;
        _debug = debug;
        _rho = rho;
    }

    // Rzeczywiste rozwiazywanie problemu
    void solve();

    void print(std::string name);

    void wypiszWymaluj(std::string name);

    ~Solver()
    {
        if (a)
        {
            delete[] a;
            a = nullptr;
        }

        if (ja)
        {
            delete[] ja;
            ja = nullptr;
        }

        if (ia)
        {
            delete[] ia;
            ia = nullptr;
        }

        if (b)
        {
            delete[] b;
            b = nullptr;
        }

        if (V)
        {
            delete[] V;
            V = nullptr;
        }
    }

private:
    double el(int l);

    // Wszystkie potrzebne warianty a(l,l)
    double allnxm1(int l);
    double allm1(int l);

    double all(int l);

    double allp1(int l);

    double allnxp1(int l);

    // Tworzy tabele poczatkowa
    void initialize();

    int _nx;
    int _ny;
    double _V1;
    double _V2;
    double _V3;
    double _V4;
    double _delta;
    double _eps1;
    double _eps2;
    int _N;
    double *a;
    int *ja;
    int *ia;
    double *b;
    double *V;
    int nz_num;
    std::string _name;
    bool _debug;
    bool _rho;
};