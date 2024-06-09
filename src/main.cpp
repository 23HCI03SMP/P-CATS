#include <iostream>
#include "include/p_cats.h"

int main() {
    Particle *p1 = new Particle("a", Point(0, 0, 0), Velocity(), Force(), Force(), 1, Charge()); // bottom left front
    Particle *p2 = new Particle("b", Point(1, 1, 1), Velocity(), Force(), Force(), 1, Charge()); // top right back
    Particle *p3 = new Particle("c", Point(0.2, 0.2, 0.2), Velocity(), Force(), Force(), 1, Charge()); // centre
    Particle *p4 = new Particle("d", Point(0.8, 0.8, 0.8), Velocity(), Force(), Force(), 1, Charge()); // centre
    Particle *p5 = new Particle("e", Point(0.25, 0.25, 0.25), Velocity(), Force(), Force(), 1, Charge()); // centre
    Particle *p6 = new Particle("f", Point(0.75, 0.75, 0.75), Velocity(), Force(), Force(), 1, Charge()); // centre
    Particle *p7 = new Particle("g", Point(0.5, 0.5, 0.5), Velocity(), Force(), Force(), 1, Charge()); // centre
    Particle *p8 = new Particle("h", Point(0.1, 0.2, 0.8), Velocity(), Force(), Force(), 1, Charge()); // centre

    Space *baseSpace = new Space(Point(0, 0, 0), Point(1, 1, 1));

    baseSpace->insert(p1);
    baseSpace->insert(p2);
    baseSpace->insert(p3);
    baseSpace->insert(p4);
    baseSpace->insert(p5);
    baseSpace->insert(p6);
    baseSpace->insert(p7);
    baseSpace->insert(p8);

    std::cout << baseSpace->toString() << std::endl;
    
    return 0;
}