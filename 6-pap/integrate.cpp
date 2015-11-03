#include "stdafx.h"
#include "integrate.h"


/**
* Determine the area under the given function subject to the given
* interval. Numerical integration is used.
*
* @param f the function for which the area shall be determined
* @param l the left bounds
* @param r the right bound
* @param steps the number of steps used for the numerical integration.
*  The more steps the better is the approximation.
* @return the approximated area under f.
*/
double integrate(IntegrateFunction f, double l, double r, int steps)
{
    int i;
    double step_size = (r - l) / (double)steps;
    double sum = 0.0;

    for (i = 0; i < steps; i++)
    {
        //  added the left bound to the sum
        //  allows passing of arbitry bounds to the function
        double x = l + (i + 0.5)*step_size;
        sum += step_size * f(x);
    }
    return sum;
}


/**
* Determine the area under the given function subject to the given
* interval. Numerical integration is used.
*
* @param f the function for which the area shall be determined
* @param l the left bounds
* @param r the right bound
* @param steps the number of steps used for the numerical integration.
*  The more steps the better is the approximation.
* @return the approximated area under f.
*/
double integrateOMP(IntegrateFunction f, double l, double r, int steps)
{
    int i;
    double step_size = (r - l) / (double)steps;
    double sum = 0.0;

#pragma omp parallel for reduction(+ : sum)
    for (i = 0; i < steps; i++)
    {
        double x = (i + 0.5) * step_size;
        sum += step_size * f(x);
    }
    return sum;
}
