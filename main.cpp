#include <iostream>
#include <random>
#include <gsl/gsl_randist.h>
#include "curve_fit.hpp"
#include "gnuplot.h"
#include <fstream>

// linspace from https://github.com/Eleobert/meth/blob/master/interpolators.hpp
template <typename Container>
auto linspace(typename Container::value_type a, typename Container::value_type b, size_t n)
{
    assert(b > a);
    assert(n > 1);

    Container res(n);
    const auto step = (b - a) / (n - 1);
    auto val = a;
    for(auto& e: res)
    {
        e = val;
        val += step;
    }
    return res;
}


double gaussian(double x, double a, double b, double c)
{
    const double z = (x - b) / c;
    return a * std::exp(-0.5 * z * z);
}

double Landau(double x, double c, double mu, double sigma)
{
   if (sigma <= 0) return 0;
   double den = c * gsl_ran_landau_pdf( (x-mu)/sigma );
   return den;
}

int main()
{

    GnuplotPipe gp;
    
    auto device = std::random_device();
    auto gen    = std::mt19937(device());

    auto xs = linspace<std::vector<double>>(0.0, 1.0, 300);
    auto ys = std::vector<double>(xs.size());

    double a = 5.0, b = 0.4, c = 0.15;

    for(size_t i = 0; i < xs.size(); i++)
    {
        auto y =  gaussian(xs[i], a, b, c);
        auto dist  = std::normal_distribution(0.0, 0.1 * y);
        ys[i] = y + dist(gen);
    }

    auto r = curve_fit(Landau, {1.0, 0.0, 1.0}, xs, ys);

    auto yy = std::vector<double>(xs.size());
    std::ofstream out;
    out.open("data.txt", std::ios::trunc);
    for(size_t i = 0; i < xs.size(); i++)
    {
        double y =  Landau(xs[i], r[0], r[1], r[2]);
        yy[i] = y;
        out << xs[i] << "," << ys[i] << "," << yy[i] << "\n";
    }
 
    out.close();
    gp.sendLine("set datafile separator \",\"");
    gp.sendLine("plot \"data.txt\" using 1:2 with lines, '' using 1:3 with lines");

    std::cout << "1\n";
    std::cout << "result: " << r[0] << ' ' << r[1] << ' ' << r[2] << '\n';
    std::cout << "error : " << r[0] - a << ' ' << r[1] - b << ' ' << r[2] - c << '\n';

}