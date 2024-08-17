#include <iostream>
#include <cstring>
#include "include/p_cats.h"
#include "include/particle.h"
#include "include/space.h"
#include "include/node.h"
#include "include/interactions.h"
#include "include/tests.h"

/// A structure to store force components (in Newtons).
///
/// This structure is used to store the components of a force in Newtons.
int main(int argc, char const *argv[])
{
    double baseSpaceSizeX = 10;
    double baseSpaceSizeY = 10;
    double baseSpaceSizeZ = 10;

    if (argc == 1)
    {
        std::cout << "Usage: ./main [OPTIONS]" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "-sx, --sizex: Set the size of the base space in the x direction." << std::endl;
        std::cout << "-sy, --sizey: Set the size of the base space in the y direction." << std::endl;
        std::cout << "-sz, --sizez: Set the size of the base space in the z direction." << std::endl;
        return 0;
    }

    for (int i = 0; i <= argc; i++)
    {
        i++;

        if (strcmp(argv[i], "-sx") || strcmp(argv[i], "--sizex"))
        {
            baseSpaceSizeX = std::stod(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-sy") || strcmp(argv[i], "--sizey"))
        {
            baseSpaceSizeY = std::stod(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-sz") || strcmp(argv[i], "--sizez"))
        {
            baseSpaceSizeZ = std::stod(argv[i + 1]);
        }
        std::cout << "Base Space Size: " << baseSpaceSizeX << " " << baseSpaceSizeY << " " << baseSpaceSizeZ << std::endl;
    }

    Space *baseSpace = new Space(Point(0, 0, 0), Point(10, 10, 10));
    std::tuple<Particle, double> p1 = std::make_tuple(Particle("Proton", 1.67e-27, Charge(1.602e-19, 0)), 0.1);
    std::tuple<Particle, double> p2 = std::make_tuple(Particle("Electron", 9.11e-31, Charge(0, -1.602e-19)), 0.1);

    std::vector<std::tuple<Particle, double>> particles = {p1, p2};
    baseSpace->generateParticles(20, 300, particles, HotspotShape::SPHERE, {4});

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

    // std::cout << baseSpace->toString() << std::endl;
    return 0;
}