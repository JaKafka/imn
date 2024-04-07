#include "adwekcja.hpp"

#include <thread>

int main()
{
    std::jthread t1(metoda,"d0", 0, true);

    std::jthread t2(metoda,"d01", 0.1, false);
    
    return 0;
}