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
    // Space *baseSpace = new Space(Point(0, 0, 0), Point(10, 10, 10));
    // std::tuple<Particle, double> p1 = std::make_tuple(Particle("Proton", 1.67e-27, Charge(1.602e-19, 0)), 0.1);
    // std::tuple<Particle, double> p2 = std::make_tuple(Particle("Electron", 9.11e-31, Charge(0, -1.602e-19)), 0.1);

    // std::vector<std::tuple<Particle, double>> particles = {p1, p2};
    // baseSpace->generateParticles(20, 300, particles, HotspotShape::SPHERE, {4});

    Particle p1 = Particle("a", 1, Charge(), Point(0, 0, 0)); // bottom left front
    Particle p2 = Particle("b", 1, Charge(), Point(1, 1, 1)); // top right back
    Particle p3 = Particle("c", 1, Charge(), Point(0.2, 0.2, 0.2)); // centre
    Particle p4 = Particle("d", 1, Charge(), Point(0.8, 0.8, 0.8)); // centre
    Particle p5 = Particle("e", 1, Charge(), Point(0.25, 0.25, 0.25)); // centre
    Particle p6 = Particle("f", 1, Charge(), Point(0.75, 0.75, 0.75)); // centre
    Particle p7 = Particle("g", 1, Charge(), Point(0.5, 0.5, 0.5)); // centre
    Particle p8 = Particle("h", 1, Charge(), Point(0.1, 0.2, 0.8)); // centre
    Particle p9 = Particle("i", 1, Charge(), Point(0.3, 0.3, 0.3)); // centre
    Particle p10 = Particle("j", 1, Charge(), Point(0.9, 0.9, 0.9)); // centre
    Particle p11 = Particle("k", 1, Charge(), Point(0.4, 0.4, 0.4)); // centre
    Particle p12 = Particle("l", 1, Charge(), Point(0.45, 0.45, 0.45)); // centre
    Particle p13 = Particle("m", 1, Charge(), Point(0.42, 0.42, 0.42)); // centre
    
    Space baseSpace = Space(Point(0, 0, 0), Point(1, 1, 1));
    baseSpace.insert(p1);
    baseSpace.insert(p2);
    baseSpace.insert(p3);
    baseSpace.insert(p4);
    baseSpace.insert(p5);
    baseSpace.insert(p6);
    baseSpace.insert(p7);
    baseSpace.insert(p8);
    baseSpace.insert(p9);
    baseSpace.insert(p10);
    baseSpace.insert(p11);
    baseSpace.insert(p12);
    baseSpace.insert(p13);

    std::cout << baseSpace.toString() << std::endl;
    return 0;
}