#ifndef pap_parallel_h__
#define pap_parallel_h__

#include "stdafx.h"
#include "integrate.h"


enum TEST_CASE_SELECT
{
    RUN_NORMAL = 1,
    RUN_OMP = 2,
    RUN_ASYNC = 4,
    RUN_PACKAGE = 8,
};

constexpr int RUN_ALL_TEST = RUN_NORMAL | RUN_OMP | RUN_ASYNC | RUN_PACKAGE;


double runOnce(IntegrateFunction f, double l, double r, int totalSteps);
double runParallelOMP(IntegrateFunction f, double l, double r, int totalSteps);
double runParallelFuture(const uint8_t cores, IntegrateFunction f, double l, double r, int totalSteps);
double runParallelPackage(const uint8_t cores, IntegrateFunction f, double l, double r, int totalSteps);


void MeassureTimeWithCores(const uint8_t cores, const uint32_t numOfSteps, uint8_t flags = RUN_ALL_TEST);


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


template<typename... Args>
void println(char const * const str, Args... parameters)
{
    std::string ausgabe(str);
    ausgabe.append("\n");
    printf(ausgabe.c_str(), parameters...);
}

#endif // pap_parallel_h__
