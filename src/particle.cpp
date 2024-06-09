#include "include/p_cats.h"

Particle::Particle(std::string alias, Point position, Velocity velocity, Force bForce, Force eForce, double mass, Charge charge) : Node(charge)
{
    this->alias = alias;
    this->position = position;
    this->velocity = velocity;
    this->bForce = bForce;
    this->eForce = eForce;
    this->mass = mass;
}