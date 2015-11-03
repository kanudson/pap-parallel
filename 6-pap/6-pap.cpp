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
        numOfSteps = INT32_MAX >> 3;

    uint8_t flags = RUN_PACKAGE | RUN_ASYNC;

    MeassureTimeWithCores(4, numOfSteps, flags);
    MeassureTimeWithCores(8, numOfSteps, flags);
    MeassureTimeWithCores(16, numOfSteps, flags);

    return EXIT_SUCCESS;
}

void MeassureTimeWithCores(const uint8_t cores, const uint32_t numOfSteps, uint8_t flags)
{
    if (cores > 32)
        throw "wtf is wrong with you?";

    omp_set_num_threads(cores);
    println("running on %i cores", cores);

    double pi, piOmp, piAsync, piPackage;

    if (RUN_NORMAL & flags)
    {
        auto usedTime_norm = meassureTime(runOnce, pi, quadrant, 0, 1, numOfSteps);
        println("Normal\n");
        println("%lf", pi);
        println("%lli ms\n", usedTime_norm.count());
    }

    if (RUN_OMP & flags)
    {
        auto usedTime_omp = meassureTime(runParallelOMP, piOmp, quadrant, 0, 1, numOfSteps);
        println("OMP Parallel");
        println("%lf", piOmp);
        println("%lli ms\n", usedTime_omp.count());
    }

    if (RUN_ASYNC & flags)
    {
        auto usedTime_async = meassureTime(runParallelFuture, piAsync, cores, quadrant, 0, 1, numOfSteps);
        println("C++ Async + Futures");
        println("%lf", piAsync * 4);
        println("%lli ms\n", usedTime_async.count());
    }

    if (RUN_PACKAGE & flags)
    {
        auto usedTime_pack = meassureTime(runParallelPackage, piPackage, cores, quadrant, 0, 1, numOfSteps);
        println("C++ Package + Thread + Future");
        println("%lf", piPackage * 4);
        println("%lli ms\n", usedTime_pack.count());
    }

    println("");
}


double runOnce(IntegrateFunction f, double l, double r, int totalSteps)
{
    double pi = integrate(f, 0, 1, totalSteps) * 4;
    return pi;
}


double runParallelOMP(IntegrateFunction f, double l, double r, int totalSteps)
{
    double pi = integrateOMP(f, 0, 1, totalSteps) * 4;
    return pi;
}


double runParallelFuture(const uint8_t maxThreads, IntegrateFunction f, double l, double r, int totalSteps)
{
    std::vector<std::future<double>> futures(maxThreads);

    const int useSteps = totalSteps / maxThreads;
    const double range = (r - l) / (double)maxThreads;
    double curL, curR;

    curL = 0.0;
    curR = range;
    for (int i = 0; i < maxThreads; ++i)
    {
        auto future = std::async(std::launch::async, integrate, f, curL, curR, useSteps);
        futures[i] = std::move(future);

        curL = curR;
        curR = curL + range;
    }

    double sum = 0.0;
    for (auto &future : futures)
    {
        sum += future.get();
    }

    return sum;
}


double runParallelPackage(const uint8_t maxThreads, IntegrateFunction f, double l, double r, int totalSteps)
{
    std::vector<std::future<double>> futures(maxThreads);

    const int useSteps = totalSteps / maxThreads;
    const double range = (r - l) / (double)maxThreads;
    double curL, curR;

    curL = 0.0;
    curR = range;
    for (int i = 0; i < maxThreads; ++i)
    {
        std::packaged_task<double(IntegrateFunction, double, double, int)> pack(integrate);
        futures[i] = pack.get_future();

        std::thread thr(std::move(pack), f, curL, curR, useSteps);
        thr.detach();

        curL = curR;
        curR += range;
    }

    //  get results
    double sum = 0.0;
    for (auto &future : futures)
    {
        sum += future.get();
    }

    return sum;
}
