#include <cmath>
#include "p_cats.h"
#include "particle.h"

class Interactions
{
public:
    /// @brief Function to calculate the interactions of other particles on particle, p.
    /// @param p Particle to calculate interactions on.
    /// @param n Node to calculate interactions from.
    /// @param theta The Barnes-Hut criterion.
    /// @param dt The time step.
    /// @param newTree The new tree to insert the new particle into.
    void Interact(Particle p, Node n, double theta, double dt, Space newTree);
};