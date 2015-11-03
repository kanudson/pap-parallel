#ifndef pap_parallel_h__
#define pap_parallel_h__

#include "stdafx.h"
#include "integrate.h"


double runParallel(IntegrateFunction f, double l, double r, int totalSteps);

double runParallelOMP(IntegrateFunction f, double l, double r, int totalSteps);
double runParallelPackage(IntegrateFunction f, double l, double r, int totalSteps);


template<typename RetVal, typename Fct, typename... Args>
std::chrono::milliseconds meassureTime(Fct function, RetVal& ret, Args... parameters)
{
    auto before = std::chrono::system_clock::now();
    ret = function(parameters...);
    auto after = std::chrono::system_clock::now();

    //  calculate duration
    auto durration = after - before;
    auto usedTime = std::chrono::duration_cast<std::chrono::milliseconds>(durration);
    return usedTime;
}

#endif // pap_parallel_h__
