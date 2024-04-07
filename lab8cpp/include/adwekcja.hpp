#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

void wczytaj(std::string &linia, int &rdx, int &rdy, double &rdval)
{
    std::vector<std::string> separated;

    std::stringstream ss;
    ss.str(linia);
    std::string wartosc;

    std::getline(ss, wartosc, '\t');
    rdx = std::stoi(wartosc);

    std::getline(ss, wartosc, '\t');
    rdy = std::stoi(wartosc);

    std::getline(ss, wartosc, '\t');
    rdval = std::stod(wartosc);
}

void metoda(std::string nazwa, double dyfuzja, bool wypisuj)
{
    // Parametry
    const int nx = 400;
    const int ny = 90;
    const int i1 = 200;
    const int i2 = 210;
    const int j1 = 50;
    const double delta = 0.01;
    const double sigma = 10 * delta;
    const double xA = 0.45;
    const double yA = 0.45;


    // Macierze
    std::vector<std::vector<double>> u0 = std::vector<std::vector<double>>(nx + 1, std::vector<double>(ny + 1, 0));
    std::vector<std::vector<double>> u1 = std::vector<std::vector<double>>(nx + 1, std::vector<double>(ny + 1, 0));
    std::vector<std::vector<double>> uprev = std::vector<std::vector<double>>(nx + 1, std::vector<double>(ny + 1, 0));
    std::vector<std::vector<double>> psi = std::vector<std::vector<double>>(nx + 1, std::vector<double>(ny + 1, 0));
    std::vector<std::vector<double>> vx = std::vector<std::vector<double>>(nx + 1, std::vector<double>(ny + 1, 0));
    std::vector<std::vector<double>> vy = std::vector<std::vector<double>>(nx + 1, std::vector<double>(ny + 1, 0));

    // Wczytanie psi

    std::ifstream psifile("../dat/psi.dat");

    int rdx, rdy;
    double rdval;

    std::string linia;

    while (std::getline(psifile, linia))
    {
        wczytaj(linia, rdx, rdy, rdval);
        psi[rdx][rdy] = rdval;
    }

    // psi debug

    // for (int i = 0; i <= nx; i++)
    // {
    //     for (int j = 0; j <= ny; j++)
    //     {
    //         std::cout << i << ' ' << j << ' ' << psi[i][j] << std::endl;
    //     }
    // }

    // Pole prędkości

    for (int i = 1; i < nx; i++)
    {
        for (int j = 1; j < ny; j++)
        {
            vx[i][j] = (psi[i][j + 1] - psi[i][j - 1]) / (2 * delta);
            vy[i][j] = -(psi[i + 1][j] - psi[i - 1][j]) / (2 * delta);
        }
    }

    // Zastawka

    for (int i = i1; i <= i2; i++)
    {
        for (int j = 0; j <= j1; j++)
        {
            vx[i][j] = 0;
            vy[i][j] = 0;
        }
    }

    // Górne i dolne brzegi

    for (int i = 1; i < nx; i++)
    {
        vx[i][0] = 0;
        vy[i][ny] = 0;
    }

    // Lewe i prawe brzegi

    for (int j = 0; j <= ny; j++)
    {
        vx[0][j] = vx[1][j];
        vx[nx][j] = vx[nx - 1][j];
    }

    // Prędkość maksymalna i dt

    double vmax = -1;

    for (int i = 0; i <= nx; i++)
    {
        for (int j = 0; j <= ny; j++)
        {
            double lv = std::sqrt(vx[i][j] * vx[i][j] + vy[i][j] * vy[i][j]);

            if (lv > vmax)
            {
                vmax = lv;
            }
        }
    }

    double dt = delta / (4 * vmax);

    std::cout << "With dt=" << dt << " and vmax = " << vmax << std::endl;

    // Warunki początkowe

    for (int i = 0; i <= nx; i++)
    {
        for (int j = 0; j <= ny; j++)
        {
            u0[i][j] = (1 / (2 * 3.14 * sigma * sigma)) * std::exp(-1 * ((std::pow(i * delta - xA, 2) + std::pow(j * delta - yA, 2))) / (2 * sigma * sigma));
        }
    }

    // Prędkości

    if (wypisuj)
    {
        std::ofstream vxfile;
    std::ofstream vyfile;

    vxfile.open("vx.dat");
    vyfile.open("vy.dat");

    for (int i = 0; i <= nx; i++)
    {
        for (int j = 0; j <= ny; j++)
        {
            vxfile << i * delta << " " << j * delta << " " << vx[i][j] << std::endl;
            vyfile << i * delta << " " << j * delta << " " << vy[i][j] << std::endl;
        }
        vxfile << std::endl;
        vyfile << std::endl;
    }

    vxfile.close();
    vyfile.close();
    }

    

    int klatkan = 1;

    // Pliki wyjścia


    std::ofstream cstream;
    std::ofstream xstream;

    cstream.open( nazwa + "c.dat");
    xstream.open( nazwa + "xsr.dat");

    for (int iter = 1; iter <= 10000; iter++)
    {
        // Picard, nowy krok
        u1 = u0;
        uprev = u1;

        for (int k = 0; k < 20; k++)
        {
            for (int i = 0; i <= nx; i++)
            {
                for (int j = 1; j < ny; j++)
                {
                    // Ignoruj zastawkę
                    if (i >= i1 && i <= i2 && j <= j1)
                        continue;

                    if (i == 0) // lewy brzeg
                    {
                        double fterm = (u0[i + 1][j] - u0[nx][j]) / (2 * delta) + (uprev[i + 1][j] - uprev[nx][j]) / (2 * delta);
                        double sterm = (u0[i][j + 1] - u0[i][j - 1]) / (2 * delta) + (uprev[i][j + 1] - uprev[i][j - 1]) / (2 * delta);
                        double tterm = (u0[i + 1][j] + u0[nx][j] + u0[i][j + 1] + u0[i][j - 1] - 4 * u0[i][j]) / (delta * delta) +
                                       (uprev[i + 1][j] + uprev[nx][j] + uprev[i][j + 1] + uprev[i][j - 1]) / (delta * delta);
                        u1[i][j] = (1 / (1 + (2 * dyfuzja * dt) / (delta * delta))) * (u0[i][j] - 0.5 * dt * vx[i][j] * fterm - 0.5 * dt * vy[i][j] * sterm + 0.5 * dt * dyfuzja * tterm);
                    }
                    else if (i == nx) // prawy brzeg
                    {
                        double fterm = (u0[0][j] - u0[i - 1][j]) / (2 * delta) + (uprev[0][j] - uprev[i - 1][j]) / (2 * delta);
                        double sterm = (u0[i][j + 1] - u0[i][j - 1]) / (2 * delta) + (uprev[i][j + 1] - uprev[i][j - 1]) / (2 * delta);
                        double tterm = (u0[0][j] + u0[i - 1][j] + u0[i][j + 1] + u0[i][j - 1] - 4 * u0[i][j]) / (delta * delta) +
                                       (uprev[0][j] + uprev[i - 1][j] + uprev[i][j + 1] + uprev[i][j - 1]) / (delta * delta);
                        u1[i][j] = (1 / (1 + (2 * dyfuzja * dt) / (delta * delta))) * (u0[i][j] - 0.5 * dt * vx[i][j] * fterm - 0.5 * dt * vy[i][j] * sterm + 0.5 * dt * dyfuzja * tterm);
                    }
                    else
                    {
                        double fterm = (u0[i + 1][j] - u0[i - 1][j]) / (2 * delta) + (uprev[i + 1][j] - uprev[i - 1][j]) / (2 * delta);
                        double sterm = (u0[i][j + 1] - u0[i][j - 1]) / (2 * delta) + (uprev[i][j + 1] - uprev[i][j - 1]) / (2 * delta);
                        double tterm = (u0[i + 1][j] + u0[i - 1][j] + u0[i][j + 1] + u0[i][j - 1] - 4 * u0[i][j]) / (delta * delta) +
                                       (uprev[i + 1][j] + uprev[i - 1][j] + uprev[i][j + 1] + uprev[i][j - 1]) / (delta * delta);
                        u1[i][j] = (1 / (1 + (2 * dyfuzja * dt) / (delta * delta))) * (u0[i][j] - 0.5 * dt * vx[i][j] * fterm - 0.5 * dt * vy[i][j] * sterm + 0.5 * dt * dyfuzja * tterm);
                    }
                }
            }

            uprev = u1;
        }


        u0 = u1;

        // Zapis klatek animacji

        if (iter % 200 == 0)
        {
            std::ofstream klatka;
            klatka.open(std::string("../output/"+nazwa+"zad5_it=") + std::to_string(klatkan) + std::string(".txt"));

            for (int i = 0; i <= nx; i++)
            {
                for (int j = 0; j <= ny; j++)
                {
                    klatka << i * delta << " " << j * delta << " " << u0[i][j] << std::endl;
                }
                klatka << std::endl;
            }

            klatka.close();

            klatkan++;
        }

        // Całka gęstości

        if (iter % 20 == 0)
        {
            double ct = 0;

        for (int i = 0; i <= nx; i++)
        {
            for (int j = 0; j <= ny; j++)
            {
                ct += u0[i][j] * delta * delta;
            }
        }

        // Średnie położenie pakietu

        double xsr = 0;

        for (int i = 0; i <= nx; i++)
        {
            for (int j = 0; j <= ny; j++)
            {
                xsr += i * delta * u0[i][j] * delta * delta;
            }
        }

        cstream << iter * dt << " " << ct << std::endl;
        xstream << iter * dt << " " << xsr << std::endl;
        }

        if (iter % 10 == 0)
        {
            std::cout << "Iter: " << iter << std::endl;
        }

        
    }

    cstream.close();
    xstream.close();
}