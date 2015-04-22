#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <cmath>
#include <boost/format.hpp>

#define MINARG 2

using namespace std;
using namespace boost;

int main(int argc, char *argv[]) {
    int i, num_steps;
    double pi, x, step, sum = 0.0;
    if(argc < MINARG) {
        cerr << "Usage: " << argv[0] << " num_steps\n";
        return EXIT_FAILURE;
    }
    num_steps = atoi(argv[1]);
    step = 1.0/num_steps;
#pragma omp parallel private(i,x)
{
#pragma omp for reduction(+:sum) schedule(static)
    for(i = 0; i < num_steps; i++) {
        x = (i+0.5) * step;
        sum = sum + 4.0/(1.0+x*x);
    }
}
    pi = step * sum;
    string formatted = str(format("Pi = %12.9f (expected %12.9f)\n") % pi % acos(-1.0));
    cout << formatted;
    return EXIT_SUCCESS;
}
