#ifndef UTILS_H
#define UTILS_H

#include <chrono>

class Timer
{
public:
    Timer();
    ~Timer();

    void end();

private:
    std::chrono::_V2::system_clock::time_point startTime;
    std::chrono::_V2::system_clock::time_point endTime;
    std::chrono::duration<float, std::milli> duration = endTime - startTime;
};

#endif