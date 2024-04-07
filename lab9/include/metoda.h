#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

const int nx = 40;
const int ny = 40;
const int N = (nx + 1) * (ny + 1);
const double DELTA = 1;
const double dt = 1;

const double TA = 40;
const double TB = 0;
const double TC = 30;
const double TD = 0;
const double kB = 0.1;
const double kD = 0.6;
const int IT_MAX = 2000;

int lij(int i, int j)
{
    return i + j * (nx + 1);
}

void wypiszWektor(std::string nazwapliku, gsl_vector *v, gsl_vector * d)
{

    std::ofstream stream;
    stream.open(nazwapliku + ".dat");

    for (int i = 0; i <= nx; i++)
    {
        for (int j = 0; j <= ny; j++)
        {

            if (i > 0 && i < nx && j > 0 && j < ny)
            {
                int l = lij(i, j);
                double vc = gsl_vector_get(v, l);
                if (isnan(vc))
                {
                    vc = 0.0;
                }


                stream << i * DELTA << " " << j * DELTA << " " << 0 << " " << vc << std::endl;
            } else
            {
                int l = lij(i, j);
                double test = gsl_vector_get(v, l);
                if (isnan(test))
                {
                    test = 0.0;
                }
                stream << i * DELTA << " " << j * DELTA << " " << 0 << " " << test << std::endl;
            }


        }
        stream << std::endl;
    }

    stream.close();
}



void metoda()
{

    // A
    // B
    //  wek c
    //  wek T

    // LU A

    gsl_matrix *A = gsl_matrix_calloc(N, N);
    gsl_matrix *B = gsl_matrix_calloc(N, N);

    gsl_vector *c = gsl_vector_calloc(N);  // alokacja z zerowaniem
    gsl_vector *vt = gsl_vector_calloc(N); // alokacja z zerowaniem

    // Wnetrze
    for (int i = 1; i < nx; i++)
    {
        for (int j = 1; j < ny; j++)
        {
            int l = lij(i, j);
            gsl_matrix_set(A, l, l, -2 * dt / (DELTA * DELTA) - 1);

            gsl_matrix_set(A, l, l - nx - 1, dt / (2 * DELTA * DELTA));
            gsl_matrix_set(A, l, l - 1, dt / (2 * DELTA * DELTA));
            gsl_matrix_set(A, l, l + 1, dt / (2 * DELTA * DELTA));
            gsl_matrix_set(A, l, l + nx + 1, dt / (2 * DELTA * DELTA));

            gsl_matrix_set(B, l, l, 2 * dt / (DELTA * DELTA) - 1);

            gsl_matrix_set(B, l, l - nx - 1, -dt * (2 * DELTA * DELTA));
            gsl_matrix_set(B, l, l - 1, -dt * (2 * DELTA * DELTA));
            gsl_matrix_set(B, l, l + nx + 1, -dt * (2 * DELTA * DELTA));
            gsl_matrix_set(B, l, l + 1, -dt * (2 * DELTA * DELTA));
        }
    }

    // WB Dirichlet lewy prawy
    for (int j = 0; j <= ny; j++)
    {
        int li1 = lij(0, j);
        int li2 = lij(nx, j);

        gsl_matrix_set(A, li1, li1, 1);
        gsl_matrix_set(A, li2, li2, 1);
        gsl_matrix_set(B, li2, li2, 1);
        gsl_matrix_set(B, li1, li1, 1);
        // gsl_vector_set(c, li1, 0);
        // gsl_vector_set(c, li2, 0);
    }

    // WB Neumann gorny
    for (int i = 1; i < nx; i++)
    {
        int l = lij(i, ny);

        gsl_matrix_set(A, l, l - nx - 1, -1 / (kB * DELTA));
        gsl_matrix_set(A, l, l, 1 + (1 / (kB * DELTA)));
        gsl_vector_set(c, l, TB);
        // for (int j = 0; j <= ny; j++)
        // {
        //     gsl_matrix_set(B, l, j, 0);
        // }
    }

    // WB Neumann dolny

    for (int i = 1; i < nx; i++)
    {
        int l = lij(i, 0);
        gsl_matrix_set(A, l, l, 1 + (1 / (kD * DELTA)));
        gsl_matrix_set(A, l, l + nx + 1, -1 / (kD * DELTA));
        gsl_vector_set(c, l, TD);
        // for (int j = 0; j <= ny; j++)
        // {
        //     gsl_matrix_set(B, l, j, 0);
        // }
    }

    // Warunki poczatkowe

    for (int j = 0; j <= ny; j++)
    {
        int l = lij(0, j);
        gsl_vector_set(vt, l, TA);
        l = lij(nx, j);
        gsl_vector_set(vt, l, TC);
    }

    // wypiszMacierz("matA", A);

    gsl_permutation *p = gsl_permutation_calloc(N);

    int sig;
    gsl_linalg_LU_decomp(A, p, &sig);

    gsl_vector *d = gsl_vector_calloc(N);

    for (int it = 0; it <= IT_MAX; it++)
    {
        gsl_blas_dgemv(CblasNoTrans, 1.0, B, vt, 0.0, d);
        gsl_blas_daxpy(1.0, c, d);

        gsl_linalg_LU_solve(A, p, d, vt);

        if (it == 100 || it == 200 || it == 500 || it == 1000 || it == 2000)
        {
            wypiszWektor(std::to_string(it), vt, d);
        }
    }

    gsl_vector_free(d);

    gsl_matrix_free(A);
    gsl_matrix_free(B);
    gsl_vector_free(c);
    gsl_vector_free(vt);
    gsl_permutation_free(p);
}