#include <iostream>
#include <cstring>
#include "include/p_cats.h"
#include "include/particle.h"
#include "include/space.h"
#include "include/node.h"
#include "include/tests.h"
#include "include/utils.h"
#include <chrono>
#include <tuple>

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

    Timer overallTimer;

    // Remove old positions file
    remove("./viewer/positions.csv");

    // Create space
    Timer generationTimer;
    Space *space = new Space(Point(0, 0, 0), Point(10, 10, 10));
    std::tuple<Particle, double> p1 = std::make_tuple(Particle("Proton", 1.67e-27, Charge(1.602e-19, 0)), 0.1);
    std::tuple<Particle, double> p2 = std::make_tuple(Particle("Electron", 9.11e-31, Charge(0, -1.602e-19)), 0.1);

    std::vector<std::tuple<Particle, double>> particles = {p1, p2};
    space->generateParticles(4, 300, particles, HotspotShape::SPHERE, {4});

    // Create main loop
    double dt = 1e-5;
    int timeSteps = 10;
    double theta = 0.5;

    std::cout << "Generating Particles..." << std::endl;
    space->toFile(0, "./viewer/positions.csv");
    generationTimer.end();

    for (int i = 0; i < timeSteps; i++)
    {
        Timer timer;
        std::cout << "Time step: " << i + 1 << std::endl;
        Space *newSpace = new Space(Point(0, 0, 0), Point(10, 10, 10));
        for (auto part : space->getAllParticles())
        {
            newSpace->insert(Interact(*part, space, theta, dt));
        }
        newSpace->recalculateCentreOfCharge();
        delete space;
        space = std::move(newSpace);
        space->toFile(i, "./viewer/positions.csv");
    }

    delete space;

    std::cout << "Simulation complete." << std::endl;
    overallTimer.end();

    // Run viewer
    system("cd viewer && python viewer.py runserver -d");

    return 0;
}
