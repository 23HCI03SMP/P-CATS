#include "include/p_cats.h"
#include "include/particle.h"
#include "include/node.h"
#include "include/interactions.h"
#include <iostream>

Particle* Interactions::Interact(Particle p, Node *n, double theta, double dt) {
    double width = (p.parent->maxPoint - p.parent->minPoint).x/2;
    double distance = (p.position - n->midpoint()).magnitude();
    double calcTheta = width/distance;
    
    Particle* newParticle = new Particle();
    
    if (calcTheta < theta || n->isExternalNode()) {
        // Node is sufficiently far away
        // Calculate the force on p from n
        *newParticle = p.particleInNextTimeStep(p.alias, p.position, p.charge, p.velocity, p.mass, n->getCentreOfCharge(), n->charge, dt);
        return newParticle;   
    }

    else {
        // Node is not sufficiently far away
        // Recursively call Interact on the children of n
        Point pos_sum = Point(0, 0, 0);
        Velocity vel_sum = Velocity(0, 0, 0);
        Force b_force_sum = Force(0, 0, 0);
        Force e_force_sum = Force(0, 0, 0);

        for (int i = 0; i < 8; i++) {
            if (n->getChildren()[i] != nullptr) {
                // finding the total change in position, velocity, and forces of the particle
                Particle *part = Interact(p, n->getChildren()[i], theta, dt); // recursor
                Particle particle;
                if (part != nullptr) {
                    particle = *part;
                }
                pos_sum = pos_sum + (particle.position - p.position);
                vel_sum = vel_sum + (particle.velocity - p.velocity);
                b_force_sum = b_force_sum + (particle.bForce - p.bForce);
                e_force_sum = e_force_sum + (particle.eForce - p.eForce);
            }
        }

        return new Particle(p.alias, p.mass, p.charge, p.position + pos_sum, p.velocity + vel_sum, p.bForce + b_force_sum, p.eForce + e_force_sum);
    }
}