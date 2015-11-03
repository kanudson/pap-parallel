#ifndef batchTest_h__
#define batchTest_h__


#include "stdafx.h"



typedef std::pair<std::chrono::milliseconds, std::chrono::milliseconds> TimerResult;


template<typename Fct, typename... Args>
TimerResult RunTest(const uint32_t count, Fct function, Args... parameters)
{
    std::chrono::milliseconds time_taken = std::chrono::milliseconds::zero();
    std::chrono::milliseconds average_time = std::chrono::milliseconds::zero();

    for (uint32_t i = 0; i < count; ++i)
    {
        auto before = std::chrono::system_clock::now();
        function(parameters...);
        auto after = std::chrono::system_clock::now();

        auto dur = after - before;
        time_taken += std::chrono::duration_cast<std::chrono::milliseconds>(dur);
    }

    average_time = time_taken / count;
    return { time_taken, average_time };
}



#endif // batchTest_h__
