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
double integrateInline(double l, double r, int steps)
{
    int i;
    double step_size = (r - l) / (double)steps;
    double sum = 0.0;

    for (i = 0; i < steps; i++)
    {
        //  added the left bound to the sum
        //  allows passing of arbitry bounds to the function
        double x = l + i * step_size;
        sum += step_size * (sqrt(1 -( x * x)));
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
double integrateSimdInline(double l, double r, int steps)
{
    int i;
    const double step_size = (r - l) / (double)steps;

    double sum = 0.0;
    __m256d sumvec = _mm256_setzero_pd();

    const __m256d multis = _mm256_set_pd(1, 2, 3, 4);
    const __m256d ones = _mm256_set1_pd(1);
    const __m256d links = _mm256_set1_pd(l);
    const __m256d stepsvec = _mm256_set1_pd(step_size);

    //_mm256_mul_pd()
    const __m256d idiffs = _mm256_set_pd(0, 1, 2, 3);

    __m256d xwerte;
    for (i = 0; i < steps; i += 4)
    {
        xwerte = _mm256_set1_pd(static_cast<double>(i));
        xwerte = _mm256_add_pd(xwerte, idiffs);
        xwerte = _mm256_mul_pd(xwerte, stepsvec);
        xwerte = _mm256_add_pd(xwerte, links);
        __m256d currentvalues = xwerte;             //  load current X value for this iteration

        //  actuall calculation
        currentvalues = _mm256_mul_pd(currentvalues, currentvalues);
        currentvalues = _mm256_sub_pd(ones, currentvalues);
        currentvalues = _mm256_sqrt_pd(currentvalues);
        currentvalues = _mm256_mul_pd(currentvalues, stepsvec);

        //  sum with sumvector
        sumvec = _mm256_add_pd(sumvec, currentvalues);
    }

    //  create total sum from vector
    sum = sumvec.m256d_f64[0] + sumvec.m256d_f64[1] +
        sumvec.m256d_f64[2] + sumvec.m256d_f64[3];

    return sum;
}
