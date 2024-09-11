#include "include/utils.h"
#include <chrono>
#include <iostream>

Timer::Timer()
{
    startTime = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    end();
}

void Timer::end()
{
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;

    float ms = duration.count();

    std::cout << "Elapsed Time: " << ms << "ms" << std::endl
              << std::endl;
}