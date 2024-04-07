#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <math.h>

struct Point {
    Point(double a = 0, double b = 0) : t(a), y(b) {}
    double t;
    double y;
};

void jawnyEuler(std::vector<Point> & output, double y_0, double lambda, double start, double stop, double dt)
{
    double yn = y_0;

    for (double i = start; i < stop + dt/4; i+=dt)
    {
        output.push_back(Point(i, yn));
        yn = yn + dt * lambda * yn;

        
    }
}

void rk2(std::vector<Point> & output, double y_0, double lambda, double start, double stop, double dt)
{
    double yn = y_0;

    for (double i = start; i < stop + dt/4; i+=dt)
    {
        output.push_back(Point(i, yn));
        
        double k1 = lambda * yn;
        double k2 = lambda * (yn + dt * k1);
        
        yn = yn + 0.5 * dt * (k1 + k2);
    }
}

void rk4(std::vector<Point> & output, double y_0, double lambda, double start, double stop, double dt)
{
    double yn = y_0;

    for (double i = start; i < stop + dt/4; i+=dt)
    {
        output.push_back(Point(i, yn));
        
        double k1 = lambda * yn;
        double k2 = lambda * (yn +  0.5 * dt * k1);
        double k3 = lambda * (yn +  0.5 * dt * k2);
        double k4 = lambda * (yn + dt *k3);
        
        yn = yn + (1.0/6.0) * dt * (k1 + 2 * k2 + 2 * k3 + k4);
    }
}
void error(std::vector<Point> & points, double lambda)
{
    for (double i = 0; i < points.size(); i++)
    {
        points[i] = Point(points[i].t, points[i].y - std::exp(lambda * points[i].t));
    }
}

void exportMatlab(std::ofstream & stream, const std::vector<Point> & points, const std::string & name)
{
    stream << name << "t = [";
    for (int i = 0; i < points.size(); i++)
    {
        stream << points[i].t;
        if (i != points.size() - 1) stream << ", ";
    }
    stream << "];\n";

    stream << name << "y = [";
    for (int i = 0; i < points.size(); i++)
    {
        stream << points[i].y;
        if (i != points.size() - 1) stream << ", ";
    }
    stream << "];\n";
}

double V(double t, double source_frequency)
{
    return 10 * std::sin(t * source_frequency);
}

void rlc(std::vector<Point> & qpoints, std::vector<Point> & ipoints, double source_frequency)
{

    const double pi = 3.14159;

    const double dt = 0.0001;
    const double R = 100;
    const double L = 0.1;
    const double C = 0.001;
    const double freq0 = 1.0 / std::sqrt(L * C);
    const double t0 = 2 * pi / freq0;

    double Q = 0;
    double I = 0;

    for (double t = 0; t < 4 * t0; t+=dt)
    {
        if (t > 4 * t0) break;
        qpoints.push_back(Point(t, Q));
        ipoints.push_back(Point(t, I));
        
        double k1q = I;
        double k1i = V(t, source_frequency* freq0) / L - Q / (L * C) - R * I / L;

        double k2q = I + 0.5 * dt * k1i;
        double k2i = V(t + 0.5 * dt, source_frequency* freq0) / L - (Q + 0.5 * dt * k1q) / (L * C) - R * (I + 0.5 * dt * k1i) / L;

        double k3q = I + 0.5 * dt * k2i;
        double k3i = V(t + 0.5 * dt, source_frequency* freq0) / L - (Q + 0.5 * dt * k2q) / (L * C) - R * (I + 0.5 * dt * k2i) / L;

        double k4q = I + dt * k3i;
        double k4i = V(t + dt, source_frequency * freq0) / L - (Q + dt * k3q) / (L * C) - R * (I + dt * k3i) / L;

        Q = Q + (dt/6) * (k1q + 2 * k2q + 2 * k3q + k4q);
        I = I + (dt/6) * (k1i + 2 * k2i + 2 * k3i + k4i);
    }

}

class GNUPlotExport {
    public:
        std::vector<std::vector<Point>> columns;
};

int main(int argc, char** argv)
{

    /* Metoda jawna Eulera */

    std::vector<Point> data1;
    std::vector<Point> data2;
    std::vector<Point> data3;

    std::thread t1 = std::thread{jawnyEuler, std::ref(data1), 1, -1, 0, 5, 0.01};
    std::thread t2 = std::thread{jawnyEuler, std::ref(data2), 1, -1, 0, 5, 0.1};
    std::thread t3 = std::thread{jawnyEuler, std::ref(data3), 1, -1, 0, 5, 1};

    t1.join();
    t2.join();
    t3.join();
    
    std::ofstream output;

    output.open("out1.m");

    exportMatlab(output, data1, "euler1");
    exportMatlab(output, data2, "euler2");
    exportMatlab(output, data3, "euler3");

    t1 = std::thread{error, std::ref(data1), -1};
    t2 = std::thread{error, std::ref(data2), -1};
    t3 = std::thread{error, std::ref(data3), -1};

    t1.join();
    t2.join();
    t3.join();

    exportMatlab(output, data1, "euler1err");
    exportMatlab(output, data2, "euler2err");
    exportMatlab(output, data3, "euler3err");

    /* Metoda RK2 */

    data1.clear();
    data2.clear();
    data3.clear();

    t1 = std::thread{rk2, std::ref(data1), 1, -1, 0, 5, 0.01};
    t2 = std::thread{rk2, std::ref(data2), 1, -1, 0, 5, 0.1};
    t3 = std::thread{rk2, std::ref(data3), 1, -1, 0, 5, 1};

    t1.join();
    t2.join();
    t3.join();
    
    exportMatlab(output, data1, "rk2_1");
    exportMatlab(output, data2, "rk2_2");
    exportMatlab(output, data3, "rk2_3");

    t1 = std::thread{error, std::ref(data1), -1};
    t2 = std::thread{error, std::ref(data2), -1};
    t3 = std::thread{error, std::ref(data3), -1};

    t1.join();
    t2.join();
    t3.join();

    exportMatlab(output, data1, "rk2_1err");
    exportMatlab(output, data2, "rk2_2err");
    exportMatlab(output, data3, "rk2_3err");

    /* Metoda RK4 */

    data1.clear();
    data2.clear();
    data3.clear();

    t1 = std::thread{rk4, std::ref(data1), 1, -1, 0, 5, 0.01};
    t2 = std::thread{rk4, std::ref(data2), 1, -1, 0, 5, 0.1};
    t3 = std::thread{rk4, std::ref(data3), 1, -1, 0, 5, 1};

    t1.join();
    t2.join();
    t3.join();
    
    exportMatlab(output, data1, "rk4_1");
    exportMatlab(output, data2, "rk4_2");
    exportMatlab(output, data3, "rk4_3");

    t1 = std::thread{error, std::ref(data1), -1};
    t2 = std::thread{error, std::ref(data2), -1};
    t3 = std::thread{error, std::ref(data3), -1};

    t1.join();
    t2.join();
    t3.join();

    exportMatlab(output, data1, "rk4_1err");
    exportMatlab(output, data2, "rk4_2err");
    exportMatlab(output, data3, "rk4_3err");

    data1.clear();
    data2.clear();
    data3.clear();


    /* Metoda RK4 w obwodzie RLC */

    std::vector<Point> qpoints;
    std::vector<Point> ipoints;

    std::vector<Point> qpoints1;
    std::vector<Point> ipoints1;

    std::vector<Point> qpoints2;
    std::vector<Point> ipoints2;

    std::vector<Point> qpoints3;
    std::vector<Point> ipoints3;

    rlc(qpoints, ipoints, 0.5);

    std::thread t4;

    t1 = std::thread{rlc, std::ref(qpoints), std::ref(ipoints), 0.5};
    t2 = std::thread{rlc, std::ref(qpoints1), std::ref(ipoints1), 0.8};
    t3 = std::thread{rlc, std::ref(qpoints2), std::ref(ipoints2), 1.0};
    t4 = std::thread{rlc, std::ref(qpoints3), std::ref(ipoints3), 1.2};

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    exportMatlab(output, qpoints, "rk4_rlcq");
    exportMatlab(output, ipoints, "rk4_rlci");

    exportMatlab(output, qpoints1, "rk4_rlcq1");
    exportMatlab(output, ipoints1, "rk4_rlci1");

    exportMatlab(output, qpoints2, "rk4_rlcq2");
    exportMatlab(output, ipoints2, "rk4_rlci2");

    exportMatlab(output, qpoints3, "rk4_rlcq3");
    exportMatlab(output, ipoints3, "rk4_rlci3");


    output.close();


    return 0;
}