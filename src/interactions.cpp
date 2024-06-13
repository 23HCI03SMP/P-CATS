#include "include/p_cats.h"
#include "include/particle.h"
#include "include/node.h"
#include "include/interactions.h"

void Interactions::Interact(Particle p, Node n, double theta, double dt, Space newTree) {
    double width = (p.parent->maxPoint - p.parent->minPoint).x/2;
    double distance = (p.position - n.midpoint()).magnitude();
    double calcTheta = width/distance;
    
    Particle newParticle = Particle();

    if (calcTheta < theta || n.isExternalNode()) {
        // Node is sufficiently far away
        // Calculate the force on p from n
        // check whether n is particle or space class
        if (n.isExternalNode()) {
            // n is a particle
            newParticle = p.particleInNextTimeStep(p.alias, p.position, p.charge, p.velocity, p.mass, Points(n.midpoint(), n.midpoint()), n.charge, dt);
        }

        else {
            // newParticle = p.particleInNextTimeStep(p.alias, p.position, p.charge, p.velocity, p.mass, ??? , n.charge, dt); // Centres of Charge
        }

        newTree.insert(&newParticle);
    }

    else {
        // Node is not sufficiently far away
        // Recursively call Interact on the children of n
        for (int i = 0; i < 8; i++) {
            if (n.getChildren()[i] != nullptr) {
                Interact(p, *n.getChildren()[i], theta, dt, newTree);
            }
        }
    }
}