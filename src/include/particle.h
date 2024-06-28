#ifndef PARTICLE_H
#define PARTICLE_H

#include "p_cats.h"
#include "node.h"
#include "space.h"
#include <memory>

class Space;

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
    Space *parent;

    std::vector<Node> getChildren() override;

    Points getCentreOfCharge() override;

    /// @brief Function to check if the space is an external node.
    /// @return True if the space is an external node, false otherwise.
    bool isExternalNode() override;

    /// @brief Polymporphic function to calculate the midpoint of the particle.
    /// @return Midpoint of the particle.
    Point midpoint() override;

    /// @brief Constructor for Particle.
    /// @param alias Alias/Name of the particle.
    /// @param position Position of the particle in m.
    /// @param velocity Velocity of the particle in m/s.
    /// @param bForce Magnetic force acting on the particle in N.
    /// @param eForce Electric force acting on the particle in N.
    /// @param mass Mass of the particle in kg.
    /// @param charge Charge of the particle in C.
    Particle(std::string alias,
             double mass,
             Charge charge,
             Point position = Point(),
             Velocity velocity = Velocity(),
             Force bForce = Force(),
             Force eForce = Force());

    Particle();

    /// @brief Function to calculate the position and velocity of the particle in the next time step.
    /// @param alias Alias/Name of the particle.
    /// @param pa Position of particle a.
    /// @param qa Charge of particle a.
    /// @param va Velocity of particle a.
    /// @param massa Mass of particle
    /// @param pb Position of particle b.
    /// @param qb Charge of particle b.
    /// @param vb Velocity of particle b.
    /// @param dt Time step in seconds.
    /// @return The particle in the next time step.
    Particle particleInNextTimeStep(std::string alias, Point pa, Charge qa, Velocity va, double massa, Points pb, Charge qb, double dt, Velocity vb = Velocity(0, 0, 0)); // add velocity qb in the future

    /// @brief Function to calculate the electric force between two particles.
    /// @param pa Position of particle a.
    /// @param qa Charge of particle a.
    /// @param pb Position of particle b.
    /// @param qb Charge of particle b.
    /// @return The electric force acting on particle a.
    Force coulombLaw(Point pa, Charge qa, Points pb, Charge qb);

    /// @brief Function to calculate the magnetic field between two particles.
    /// @param pa Position of particle a.
    /// @param va Velocity of particle a.
    /// @param pb Position of particle b.
    /// @param qb Charge of particle b.
    /// @param vb Velocity of particle b.
    /// @return The magnetic field acting on particle a.
    Field biotSavart(Point pa, Velocity va, Points pb, Charge qb, Velocity vb);

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
    Velocity updateVelocity(Point pa, Charge qa, Velocity va, double massa, Points pb, Charge qb, Velocity vb, double dt);
};

#endif