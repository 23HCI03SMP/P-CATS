#include <iostream>
#include "include/p_cats.h"
#include "include/particle.h"
#include "include/space.h"
#include "include/node.h"
#include "include/interactions.h"
#include "include/tests.h"
#include <chrono>
#include <tuple>


class Timer
{
public:
    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    void end()
    {
        end_time = std::chrono::high_resolution_clock::now();
        duration = end_time - start;
        float ms = duration.count();
        std::cout << "Elapsed Time: " << ms << "ms" << std::endl << std::endl;
    }

    ~Timer()
    {
        end();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end_time;
    std::chrono::duration<float, std::milli> duration = end_time - start;
};


int main()
{
    Timer overallTimer;

    // Remove old positions file
    remove("./viewer/positions.csv");

    // Create space
    Timer generationTimer;
    Space *space = new Space(Point(0, 0, 0), Point(10, 10, 10));
    std::tuple<Particle, double> p1 = std::make_tuple(Particle("Proton", 1.67e-27, Charge(1.602e-19, 0)), 0.1);
    std::tuple<Particle, double> p2 = std::make_tuple(Particle("Electron", 9.11e-31, Charge(0, -1.602e-19)), 0.1);

    std::vector<std::tuple<Particle, double>> particles = {p1, p2};
    // space->generateParticles(4, 300, particles, HotspotShape::SPHERE, {4});

    // space->insert(new Particle("Electron", 1/1840 * amu, Charge(0, 1.602e-19), Point(4, 4, 4), Velocity(0, 0, 0), Force(0, 0, 0), Force(0, 0, 0)));
    // space->insert(new Particle("Electron", 1/1840 * amu, Charge(0, 1.602e-19), Point(6, 6, 6), Velocity(0, 0, 0), Force(0, 0, 0), Force(0, 0, 0)));
    space->insert(new Particle("Electron", 9.11e-31, Charge(0, 1.602e-19), Point(6, 6, 6), Velocity(0, 0, 0), Force(0, 0, 0), Force(0, 0, 0)));
    space->insert(new Particle("Positron", 9.11e-31, Charge(1.602e-19, 0), Point(4, 4, 4), Velocity(0, 0, 0), Force(0, 0, 0), Force(0, 0, 0)));
    space->recalculateCentreOfCharge();

    // Create main loop
    double dt = 1e-11;
    int timeSteps = 100;
    double theta = 0.5;

    std::cout << "p: " << (1.602e-19 * 1000 * dt)/amu << std::endl;

    std::cout << "Generating Particles..." << std::endl;
    space->toFile(0, "./viewer/positions.csv");
    generationTimer.end();

    Interactions interactions;
    for (int i = 0; i < timeSteps; i++) {
        Timer timer;
        std::cout << "Time step: " << i + 1 << std::endl;
        Space *newSpace = new Space(Point(0, 0, 0), Point(10, 10, 10));
        for (auto part : space->getAllParticles())
        {
            newSpace->insert(interactions.Interact(*part, space, theta, dt));
        }
        newSpace->recalculateCentreOfCharge();
        delete space;
        space = newSpace;
        space->toFile(i, "./viewer/positions.csv");
    }

    std::cout << "Simulation complete." << std::endl;
    overallTimer.end();

    // Run viewer
    system("cd viewer && python viewer.py runserver -d");
    
    return 0;
}