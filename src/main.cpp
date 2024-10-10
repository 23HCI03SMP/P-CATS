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

void OutputInputError()
{
    std::cout << "Usage: ./main [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-sx, --sizex: Set the size of the base space in the x direction." << std::endl;
    std::cout << "-sy, --sizey: Set the size of the base space in the y direction." << std::endl;
    std::cout << "-sz, --sizez: Set the size of the base space in the z direction." << std::endl;
    std::cout << "-o, --output: Set the output file for the positions." << std::endl;
}

/// A structure to store force components (in Newtons).
///
/// This structure is used to store the components of a force in Newtons.
int main(int argc, char const *argv[])
{
    std::string nameLogo = R"(    ____          ______ ___   ______ _____
   / __ \        / ____//   | /_  __// ___/
  / /_/ /______ / /    / /| |  / /   \__ \ 
 / ____//_____// /___ / ___ | / /   ___/ / 
/_/            \____//_/  |_|/_/   /____/ 
)";

    std::string imageLogo = R"(             　／|、     
            （ﾟ､ ｡ ７ 　
            　|、~ ヽ   
            　ししと ）ノ
)";

    std::cout << nameLogo << std::endl;
    std::cout << imageLogo << std::endl;

    double baseSpaceSizeX, baseSpaceSizeY, baseSpaceSizeZ = 10;
    double density = 4;       // density in (idk the units rn)
    double temperature = 300; // temperature in K
    std::string outputFile = "./viewer/positions.csv";

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-sx") == 0 || strcmp(argv[i], "--sizex") == 0)
        {
            if (i + 1 < argc)
            {
                baseSpaceSizeX = std::stod(argv[++i]);
            }
            else
            {
                OutputInputError();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-sy") == 0 || strcmp(argv[i], "--sizey") == 0)
        {
            if (i + 1 < argc)
            {
                baseSpaceSizeY = std::stod(argv[++i]);
            }
            else
            {
                OutputInputError();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-sz") == 0 || strcmp(argv[i], "--sizez") == 0)
        {
            if (i + 1 < argc)
            {
                baseSpaceSizeZ = std::stod(argv[++i]);
            }
            else
            {
                OutputInputError();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            if (i + 1 < argc)
            {
                outputFile = argv[++i];
            }
            else
            {
                OutputInputError();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--density") == 0)
        {
            if (i + 1 < argc)
            {
                density = std::stod(argv[++i]);
            }
            else
            {
                OutputInputError();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--temperature") == 0)
        {
            if (i + 1 < argc)
            {
                temperature = std::stod(argv[++i]);
            }
            else
            {
                OutputInputError();
                return 1;
            }
        }
        else
        {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            return 1;
        }
    }

    Timer overallTimer;

    // Remove old positions file
    remove(outputFile.c_str());

    // Create space
    Timer generationTimer;
    Space *space = new Space(Point(0, 0, 0), Point(baseSpaceSizeX, baseSpaceSizeY, baseSpaceSizeZ));
    std::tuple<Particle, double> p1 = std::make_tuple(Particle("Proton", 1.67e-27, Charge(1.602e-19, 0)), 0.1);
    std::tuple<Particle, double> p2 = std::make_tuple(Particle("Electron", 9.11e-31, Charge(0, -1.602e-19)), 0.1);

    std::vector<std::tuple<Particle, double>> particles = {p1, p2};
    space->generateParticles(density, temperature, particles, HotspotShape::SPHERE, {4});

    // Create main loop
    double dt = 1e-5;
    int timeSteps = 10;
    double theta = 0.5;

    std::cout << "Generating Particles..." << std::endl;
    space->toFile(0, outputFile);
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
        space->toFile(i, outputFile);
    }

    delete space;

    std::cout << "Simulation complete." << std::endl;
    overallTimer.end();

    // Run viewer
    system("cd viewer && python viewer.py runserver -d");

    return 0;
}
