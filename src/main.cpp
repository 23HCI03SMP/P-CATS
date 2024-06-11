#include <iostream>
#include "include/p_cats.h"

/// A structure to store force components (in Newtons).
///
/// This structure is used to store the components of a force in Newtons.
int main() {
    Particle *p1 = new Particle("a", 1, Charge(), Point(0, 0, 0), Velocity(), Force(), Force()); // bottom left front
    Particle *p2 = new Particle("b", 1, Charge(), Point(1, 1, 1), Velocity(), Force(), Force()); // top right back
    Particle *p3 = new Particle("c", 1, Charge(), Point(0.2, 0.2, 0.2), Velocity(), Force(), Force()); // centre
    Particle *p4 = new Particle("d", 1, Charge(), Point(0.8, 0.8, 0.8), Velocity(), Force(), Force()); // centre
    Particle *p5 = new Particle("e", 1, Charge(), Point(0.25, 0.25, 0.25), Velocity(), Force(), Force()); // centre
    Particle *p6 = new Particle("f", 1, Charge(), Point(0.75, 0.75, 0.75), Velocity(), Force(), Force()); // centre
    Particle *p7 = new Particle("g", 1, Charge(), Point(0.5, 0.5, 0.5), Velocity(), Force(), Force()); // centre
    Particle *p8 = new Particle("h", 1, Charge(), Point(0.1, 0.2, 0.8), Velocity(), Force(), Force()); // centre
    Particle *p9 = new Particle("i", 1, Charge(), Point(0.3, 0.3, 0.3), Velocity(), Force(), Force()); // centre

    Space *baseSpace = new Space(Point(0, 0, 0), Point(1, 1, 1));

    baseSpace->insert(p1);
    baseSpace->insert(p2);
    baseSpace->insert(p3);
    baseSpace->insert(p4);
    baseSpace->insert(p5);
    baseSpace->insert(p6);
    baseSpace->insert(p7);
    baseSpace->insert(p8);
    baseSpace->insert(p9);

    std::cout << baseSpace->toString() << std::endl;
    
    return 0;
}