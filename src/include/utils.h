#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include "particle.h"

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

/// @brief Function to calculate the interactions of other particles on particle, p.
/// @param p Particle to calculate interactions on.
/// @param n Node to calculate interactions from.
/// @param theta The Barnes-Hut criterion.
/// @param dt Time step.
/// @param newTree The new tree to store the interactions.
Particle* Interact(Particle p, Node *n, double theta, double dt);

#endif