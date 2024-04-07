#include "navstokes.hpp"

int main()
{
    navStokes(0.01, 1, 1, 200, 90, 50, 55, 20000, -1000, "wyn.dat");
    navStokes(0.01, 1, 1, 200, 90, 50, 55, 20000, -4000, "wyn_Qm4000.dat");
    navStokes(0.01, 1, 1, 200, 90, 50, 55, 20000, 4000, "wyn_Q4000.dat");

    return 0;
}