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
    void Interact(Particle *p, Node *n, double theta, double dt, Space newTree);
};