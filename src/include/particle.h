#include "p_cats.h"
#include "node.h"

/// @brief A structure to store a particle node in the tree.
///
/// A structure that stores a particle node in the tree. This node has a position, velocity, and forces acting on it.
class Particle : public Node
{
public:
    /// @brief Alias/Name of the particle.
    std::string alias;
    /// @brief Mass of the particle in kg.
    double mass;
    /// @brief Position of the particle in m.
    Point position;
    /// @brief Velocity of the particle in m/s.
    Velocity velocity;
    /// @brief Magnetic Force acting on the particle in N.
    Force bForce;
    /// @brief Electric force acting on the particle in N.
    Force eForce;

    /// @brief Constructor for Particle.
    /// @param alias Alias/Name of the particle.
    /// @param position Position of the particle in m.
    /// @param velocity Velocity of the particle in m/s.
    /// @param bForce Magnetic force acting on the particle in N.
    /// @param eForce Electric force acting on the particle in N.
    /// @param mass Mass of the particle in kg.
    /// @param charge Charge of the particle in C.
    Particle(std::string alias, Point position, Velocity velocity, Force bForce, Force eForce, double mass, Charge charge);

    /// @brief Function to update the position of the particle.
    ///
    /// @param pa Position of particle a.
    /// @param qa Charge of particle a.
    /// @param va Velocity of particle a.
    /// @param massa Mass of particle
    /// @param pb Position of particle b.
    /// @param qb Charge of particle b.
    /// @param vb Velocity of particle b.
    /// @param dt Time step in seconds.
    /// @return The new position of the particle.
    Point updatePosition(Point pa, Charge qa, Velocity va, double massa, Point pb, Charge qb, Velocity vb, double dt);

    /// @brief Function to calculate the electric force between two particles.
    /// @param pa Position of particle a.
    /// @param qa Charge of particle a.
    /// @param pb Position of particle b.
    /// @param qb Charge of particle b.
    /// @return The electric force acting on particle a.
    Force coulombLaw(Point pa, Charge qa, Point pb, Charge qb);

    /// @brief Function to calculate the magnetic field between two particles.
    /// @param pa Position of particle a.
    /// @param va Velocity of particle a.
    /// @param pb Position of particle b.
    /// @param qb Charge of particle b.
    /// @param vb Velocity of particle b.
    /// @return The magnetic field acting on particle a.
    Field biotSavart(Point pa, Velocity va, Point pb, Charge qb, Velocity vb);

    /// @brief Function to update the velocity of the particle.
    /// @param pa Position of particle a.
    /// @param qa Charge of particle a.
    /// @param va Velocity of particle a.
    /// @param massa Mass of particle
    /// @param pb Position of particle b.
    /// @param qb Charge of particle b.
    /// @param vb Velocity of particle b.
    /// @param dt Time step in seconds.
    /// @return The new velocity of the particle.
    Velocity updateVelocity(Point pa, Charge qa, Velocity va, double massa, Point pb, Charge qb, Velocity vb, double dt);
};