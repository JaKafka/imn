#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <math.h>
#include <algorithm>
#include <chrono>


#include "struna.h"

int main(int argc, char **argv)
{
    verlet(0,0,"v00.dat", false);
    verlet(0,0.1,"v001.dat", false);
    verlet(0,1,"v01.dat", false);

    verlet(1,1,"v11.dat", true);

    return 0;
}