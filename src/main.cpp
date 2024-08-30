#include <iostream>
#include "include/p_cats.h"
#include "include/particle.h"
#include "include/space.h"
#include "include/node.h"
#include "include/interactions.h"
#include "include/tests.h"

/// A structure to store force components (in Newtons).
///
/// This structure is used to store the components of a force in Newtons.
int main()
{
    Space *space = new Space(Point(0, 0, 0), Point(10, 10, 10));
    std::tuple<Particle, double> p1 = std::make_tuple(Particle("Proton", 1.67e-27, Charge(1.602e-19, 0)), 0.1);
    std::tuple<Particle, double> p2 = std::make_tuple(Particle("Electron", 9.11e-31, Charge(0, -1.602e-19)), 0.1);

    std::vector<std::tuple<Particle, double>> particles = {p1, p2};
    space->generateParticles(4, 300, particles, HotspotShape::SPHERE, {4});

    // Particle *p1 = new Particle("a", 1, Charge(), Point(0, 0, 0)); // bottom left front
    // Particle *p2 = new Particle("b", 1, Charge(), Point(1, 1, 1)); // top right back
    // Particle *p3 = new Particle("c", 1, Charge(), Point(0.2, 0.2, 0.2)); // centre
    // Particle *p4 = new Particle("d", 1, Charge(), Point(0.8, 0.8, 0.8)); // centre
    // Particle *p5 = new Particle("e", 1, Charge(), Point(0.25, 0.25, 0.25)); // centre
    // Particle *p6 = new Particle("f", 1, Charge(), Point(0.75, 0.75, 0.75)); // centre
    // Particle *p7 = new Particle("g", 1, Charge(), Point(0.5, 0.5, 0.5)); // centre
    // Particle *p8 = new Particle("h", 1, Charge(), Point(0.1, 0.2, 0.8)); // centre
    // Particle *p9 = new Particle("i", 1, Charge(), Point(0.3, 0.3, 0.3)); // centre
    // Particle *p10 = new Particle("j", 1, Charge(), Point(0.9, 0.9, 0.9)); // centre
    // Particle *p11 = new Particle("k", 1, Charge(), Point(0.4, 0.4, 0.4)); // centre
    // Particle *p12 = new Particle("l", 1, Charge(), Point(0.45, 0.45, 0.45)); // centre
    // Particle *p13 = new Particle("m", 1, Charge(), Point(0.42, 0.42, 0.42)); // centre

    // Space *baseSpace = new Space(Point(0, 0, 0), Point(1, 1, 1));

    // baseSpace->insert(p1);
    // baseSpace->insert(p2);
    // baseSpace->insert(p3);
    // baseSpace->insert(p4);
    // baseSpace->insert(p5);
    // baseSpace->insert(p6);
    // baseSpace->insert(p7);
    // baseSpace->insert(p8);
    // baseSpace->insert(p9);
    // baseSpace->insert(p10);
    // baseSpace->insert(p11);
    // baseSpace->insert(p12);
    // baseSpace->insert(p13);

    // Create main loop

    Force force = Force(0, 0, 0);

    Interactions interactions;

    int timeSteps = 5;


    for (int i = 0; i < timeSteps; i++) {
        space->toFile(i, "./viewer/positions.csv");
        std::cout << space->getAllParticles().size() << std::endl;
        Space *newSpace = new Space(Point(0, 0, 0), Point(10, 10, 10));
        for (auto part : space->getAllParticles())
        {
            newSpace->insert(interactions.Interact(*part, space, 0.5, 100));
        }
        newSpace->recalculateCentreOfCharge();
        std::cout << newSpace->getAllParticles().size() << std::endl;
        std::cout << "Time step: " << i << std::endl;
        // std::cout << "======================================================================================================" << std::endl;
        // std::cout << newSpace->toString() << std::endl;
        delete space;
        space = newSpace;
    }
    space->toFile(timeSteps, "./viewer/positions.csv");
    // delete baseSpace;
    // baseSpace = newTree;
    // delete newTree;

    // delete baseSpace;
    

    Space *newTree = new Space(Point(0, 0, 0), Point(10, 10, 10));
    newTree->insert(new Particle("Proton", 1.67e-27, Charge(1.602e-19, 0), Point(0, 0, 0)));

    // std::cout << baseSpace->toString() << std::endl;
    // baseSpace->toFile(0, "./viewer/positions.csv");
    system("cd viewer && python viewer.py");
    return 0;
}