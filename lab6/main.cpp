#include <iostream>

#include "solver.h"

int main(int argc, char **argv)
{
    {
        // Wypisuje tablice a i b do pliku --> readme.txt
        Solver test(4, 4, 10, -10, 10, -10, 0.1, 1, 1, "testowy", true, false);
        test.solve();
    }

    {
        // zadanie 5 50x50
        Solver zad5a(50, 50, 10, -10, 10, -10, 0.1, 1, 1, "5a", false, false);
        zad5a.solve();
    }

    {
        // Zadanie 5 100x100
        Solver zad5b(100, 100, 10, -10, 10, -10, 0.1, 1, 1, "5b", false, false);
        zad5b.solve();
    }

    {
        // Zadanie 5 200x200
        Solver zad5c(200, 200, 10, -10, 10, -10, 0.1, 1, 1, "5c", false, false);
        zad5c.solve();
    }

    {
        // Zadanie 6 e1 = 1 e2 = 1
        Solver zad6a(100, 100, 0, 0, 0, 0, 0.1, 1, 1, "6a", false, true);
        zad6a.solve();
    }

    {
        // Zadanie 6 e1 = 1 e2 = 2
        Solver zad6b(100, 100, 0, 0, 0, 0, 0.1, 1, 2, "6b", false, true);
        zad6b.solve();
    }

    {
        // Zadanie 6 e1 = 1 e2 = 10
        Solver zad6c(100, 100, 0, 0, 0, 0, 0.1, 1, 10, "6c", false, true);
        zad6c.solve();
    }

    return 0;
}