#include <iostream>
#include "include/p_cats.h"
#include "include/particle.h"
#include "include/space.h"
#include "include/node.h"
#include "include/interactions.h"
#include "include/tests.h"
#include <chrono>
#include <tuple>

mpf_t PI;
mpf_t K_B;
mpf_t epsilon0;
mpf_t mu0;
mpf_t e;
mpf_t amu;

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

void initialiseMPFValues()
{
    mpf_set_default_prec(256); // Set the default precision to 256 bits

    constexpr char PI_str[] = "3.14159265358979323846";
    mpf_init_set_str(PI, PI_str, 10);

    constexpr char K_B_str[] = "1.380649e-23";
    mpf_init_set_str(K_B, K_B_str, 10);

    constexpr char epsilon0_str[] = "8.8541878188e-12";
    mpf_init_set_str(epsilon0, epsilon0_str, 10);

    constexpr char mu0_str[] = "4";
    mpf_init_set_str(mu0, mu0_str, 10);
    mpf_mul(mu0, mu0, PI);
    mpf_t seven;
    mpf_init_set_str(seven, "1e-7", 10);
    mpf_mul(mu0, mu0, seven);

    constexpr char e_str[] = "1.602176634e-19";
    mpf_init_set_str(e, e_str, 10);

    constexpr char amu_str[] = "1.66053906660e-27";
    mpf_init_set_str(amu, amu_str, 10);
}

int main()
{
    std::cout << "1" << std::endl;
    initialiseMPFValues();

    Timer overallTimer;

    // Remove old positions file
    remove("./viewer/positions.csv");

    // Create space
    Timer generationTimer;
    Space *space = new Space(Point(0.0, 0.0, 0.0), Point(10, 10, 10));
    std::tuple<Particle, double> p1 = std::make_tuple(Particle("Proton", 1.67e-27, Charge(1.602e-19, 0)), 0.1);
    std::tuple<Particle, double> p2 = std::make_tuple(Particle("Electron", 9.11e-31, Charge(0, -1.602e-19)), 0.1);

    std::cout << "2" << std::endl;

    std::vector<std::tuple<Particle, double>> particles = {p1, p2};
    space->generateParticles(4, 300, particles, HotspotShape::SPHERE, {4});

    std::cout << "3" << std::endl;

    // space->insert(new Particle("Electron", 1/1840 * amu, Charge(0, 1.602e-19), Point(4, 4, 4), Velocity(0, 0, 0), Force(0, 0, 0), Force(0, 0, 0)));
    // space->insert(new Particle("Electron", 1/1840 * amu, Charge(0, 1.602e-19), Point(6, 6, 6), Velocity(0, 0, 0), Force(0, 0, 0), Force(0, 0, 0)));
    // space->insert(new Particle("Electron", 9.11e-31, Charge(0, 1.602e-19), Point(6, 6, 6), Velocity(0, 0, 0), Force(0, 0, 0), Force(0, 0, 0)));
    // space->insert(new Particle("Positron", 9.11e-31, Charge(1.602e-19, 0), Point(4, 4, 4), Velocity(0, 0, 0), Force(0, 0, 0), Force(0, 0, 0)));
    // space->recalculateCentreOfCharge();

    // // Create main loop
    // double dt = 1e-11;
    // int timeSteps = 100;
    // double theta = 0.5;

    // std::cout << "p: " << (1.602e-19 * 1000 * dt)/mpf_get_d(amu) << std::endl;

    // std::cout << "Generating Particles..." << std::endl;
    // space->toFile(0, "./viewer/positions.csv");
    // generationTimer.end();

    // Interactions interactions;
    // for (int i = 0; i < timeSteps; i++) {
    //     Timer timer;
    //     std::cout << "Time step: " << i + 1 << std::endl;
    //     Space *newSpace = new Space(Point(0, 0, 0), Point(10, 10, 10));
    //     for (auto part : space->getAllParticles())
    //     {
    //         newSpace->insert(interactions.Interact(*part, space, theta, dt));
    //     }
    //     newSpace->recalculateCentreOfCharge();
    //     delete space;
    //     space = newSpace;
    //     space->toFile(i, "./viewer/positions.csv");
    // }

    // std::cout << "Simulation complete." << std::endl;
    // overallTimer.end();

    std::cout << "Space: " << space->toString() << std::endl;

    std::cout << space->toString() << std::endl;

    // Run viewer
    // system("cd viewer && python viewer.py runserver -d");
    
    return 0;
}