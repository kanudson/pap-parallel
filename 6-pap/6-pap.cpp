// 6-pap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "pap-parallel.h"

#include "batchTest.h"


int main(int argc, char* argv[])
{
    double pi = 0, piOmp = 0, piPackage = 0;
    int numOfSteps;

    if (argc > 1)
        numOfSteps = atoi(argv[1]);
    else
        numOfSteps = INT32_MAX >> 2;

    auto usedTime_norm = meassureTime(runParallel, pi, quadrant, 0, 1, numOfSteps);
    printf("Normal\n");
    printf("%lf\n", pi);
    printf("%lli ms\n\n", usedTime_norm.count());

    auto usedTime_omp = meassureTime(runParallelOMP, piOmp, quadrant, 0, 1, numOfSteps);
    printf("OMP Parallel\n");
    printf("%lf\n", piOmp);
    printf("%lli ms\n", usedTime_omp.count());

    auto usedTime_pack = meassureTime(runParallelPackage, piPackage, quadrant, 0, 1, numOfSteps);
    printf("OMP Parallel\n");
    printf("%lf\n", piPackage * 4);
    printf("%lli ms\n", usedTime_pack.count());

    return EXIT_SUCCESS;
}


double runParallel(IntegrateFunction f, double l, double r, int totalSteps)
{
    double pi = integrate(f, 0, 1, totalSteps) * 4;
    return pi;
}


double runParallelOMP(IntegrateFunction f, double l, double r, int totalSteps)
{
    double pi = integrateParallel(f, 0, 1, totalSteps) * 4;
    return pi;
}


double runParallelPackage(IntegrateFunction f, double l, double r, int totalSteps)
{
    int maxThreads = omp_get_max_threads();
    std::vector<std::future<double>> futures;

    const int useSteps = totalSteps / maxThreads;
    const double range = (r - l) / (double)maxThreads;
    double curL, curR;

    curL = 0.0;
    curR = range;
    for (int i = 0; i < maxThreads; ++i)
    {
        auto future = std::async(std::launch::async, integrate, f, curL, curR, useSteps);
        futures.push_back(std::move(future));

        curL = curR;
        curR = curL + range;
    }

    double sumpi = 0.0;
    for (auto &future : futures)
    {
        sumpi += future.get();
    }

    return sumpi;
}

