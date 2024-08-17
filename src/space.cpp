#include "include/p_cats.h"
#include "include/space.h"
#include "include/node.h"
#include "include/particle.h"
#include <iostream>
#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <fstream>

#define o1 0 // top left back
#define o2 1 // top right back
#define o3 2 // top right front
#define o4 3 // top left front
#define o5 4 // bottom left back
#define o6 5 // bottom right back
#define o7 6 // bottom right front
#define o8 7 // bottom left front

bool Space::isExternalNode()
{
    return false;
}

Points Space::getCentreOfCharge()
{
    return centreOfCharge;
}

std::vector<Node *> Space::getChildren()
{
    return children;
}

std::vector<Particle *> Space::getAllParticles()
{
    std::vector<Particle *> particles;

    for (auto child : children)
    {
        if (dynamic_cast<Particle *>(child))
        {
            Particle *particle = dynamic_cast<Particle *>(child);
            particles.push_back(particle);
        }
        else if (dynamic_cast<Space *>(child))
        {
            Space *space = dynamic_cast<Space *>(child);
            std::vector<Particle *> childParticles = space->getAllParticles();
            particles.insert(particles.end(), childParticles.begin(), childParticles.end());
        }
    }

    return particles;
}

Space::Space(Point minPoint, Point maxPoint, Charge charge) : Node(charge)
{
    this->minPoint = minPoint;
    this->maxPoint = maxPoint;
    this->centreOfCharge = Points();

    for (int i = 0; i <= o8; i++)
    {
        children.push_back(nullptr);
    }
}

Point Space::midpoint()
{
    return Point((minPoint.x + maxPoint.x) / 2, (minPoint.y + maxPoint.y) / 2, (minPoint.z + maxPoint.z) / 2);
}

// Logic for insertion:
// 1. If the particle is outside the bounds of the space, return.
// 2. If the space is an external node, find the octet in which the particle belongs and insert it.
// 3. If the octet is a nullptr, replace it with the particle.
// 4. If the octet is a space, recursively insert the particle into the octet.
void Space::insert(Particle *particle)
{
    if (particle->position.x < minPoint.x || particle->position.x > maxPoint.x ||
        particle->position.y < minPoint.y || particle->position.y > maxPoint.y ||
        particle->position.z < minPoint.z || particle->position.z > maxPoint.z)
    {
        return;
    }

    int octet;
    double xMid = (this->minPoint.x + this->maxPoint.x) / 2;
    double yMid = (this->minPoint.y + this->maxPoint.y) / 2;
    double zMid = (this->minPoint.z + this->maxPoint.z) / 2;
    if (particle->position.y >= yMid) // top
    {
        if (particle->position.x >= xMid) // right
        {
            if (particle->position.z >= zMid) // back
            {
                octet = o2; // top right back
            }
            else // front
            {
                octet = o3; // top right front
            }
        }
        else // left
        {
            if (particle->position.z >= zMid) // back
            {
                octet = o1; // top left back
            }
            else // front
            {
                octet = o4; // top left front
            }
        }
    }
    else // bottom
    {
        if (particle->position.x >= xMid) // right
        {
            if (particle->position.z >= zMid) // back
            {
                octet = o6; // bottom right back
            }
            else // front
            {
                octet = o7; // bottom right front
            }
        }
        else // left
        {
            if (particle->position.z >= zMid) // back
            {
                octet = o5; // bottom left back
            }
            else // front
            {
                octet = o8; // bottom left front
            }
        }
    }

    if (this->children[octet] == nullptr)
    {
        particle->parent = this;
        this->children[octet] = particle;
    }
    else
    {
        // print type of children[octet]
        // std::cout << "Type: " << typeid(this->children[octet]).name() << std::endl;
        if (dynamic_cast<Particle *>(this->children[octet]) != nullptr)
        {
            Particle *oldParticle = dynamic_cast<Particle *>(this->children[octet]);

            // if the particle is at the same position as the old particle, ignore it
            if (oldParticle->position.x == particle->position.x &&
                oldParticle->position.y == particle->position.y &&
                oldParticle->position.z == particle->position.z)
            {
                // std::cout << "Particle already exists in this position. Ignoring...\n"
                //           << std::endl;
                return;
            }

            Point subspaceMinPoint, subspaceMaxPoint;

            switch (octet)
            {
            case o1: // top left back
                subspaceMinPoint = Point(this->minPoint.x, yMid, zMid);
                subspaceMaxPoint = Point(xMid, this->maxPoint.y, this->maxPoint.z);
                break;
            case o2: // top right back
                subspaceMinPoint = Point(xMid, yMid, zMid);
                subspaceMaxPoint = Point(this->maxPoint.x, this->maxPoint.y, this->maxPoint.z);
                break;
            case o3: // top right front
                subspaceMinPoint = Point(xMid, yMid, this->minPoint.z);
                subspaceMaxPoint = Point(this->maxPoint.x, this->maxPoint.y, zMid);
                break;
            case o4: // top left front
                subspaceMinPoint = Point(this->minPoint.x, yMid, this->minPoint.z);
                subspaceMaxPoint = Point(xMid, this->maxPoint.y, zMid);
                break;
            case o5: // bottom left back
                subspaceMinPoint = Point(this->minPoint.x, this->minPoint.y, zMid);
                subspaceMaxPoint = Point(xMid, yMid, this->maxPoint.z);
                break;
            case o6: // bottom right back
                subspaceMinPoint = Point(xMid, this->minPoint.y, zMid);
                subspaceMaxPoint = Point(this->maxPoint.x, yMid, this->maxPoint.z);
                break;
            case o7: // bottom right front
                subspaceMinPoint = Point(xMid, this->minPoint.y, this->minPoint.z);
                subspaceMaxPoint = Point(this->maxPoint.x, yMid, zMid);
                break;
            case o8: // bottom left front
                subspaceMinPoint = Point(this->minPoint.x, this->minPoint.y, this->minPoint.z);
                subspaceMaxPoint = Point(xMid, yMid, zMid);
                break;
            default:
                break;
            }

            this->children[octet] = new Space(subspaceMinPoint, subspaceMaxPoint);
            dynamic_cast<Space *>(this->children[octet])->insert(oldParticle);
            dynamic_cast<Space *>(this->children[octet])->insert(particle);
        }
        else if (dynamic_cast<Space *>(this->children[octet]) != nullptr)
        {
            dynamic_cast<Space *>(this->children[octet])->insert(particle);
        }
        else
        {
            throw std::runtime_error("This shouldn't happen lmao.");
        }
    }
}

std::vector<Particle *> Space::generateParticles(double density,
                                                 double temperature,
                                                 std::vector<std::tuple<Particle, double>> &particles,
                                                 HotspotShape hotspotShape,
                                                 std::initializer_list<double> params)
{
    /// @todo Ensure that percentage sums to 1

    std::vector<Particle *> generatedParticles;

    for (int i = 0; i < particles.size(); i++)
    {
        Particle particle = std::get<0>(particles[i]);
        double percentage = std::get<1>(particles[i]);
        double mass = particle.mass;
        Charge charge = particle.charge;
        double xMid = (this->minPoint.x + this->maxPoint.x) / 2;
        double yMid = (this->minPoint.y + this->maxPoint.y) / 2;
        double zMid = (this->minPoint.z + this->maxPoint.z) / 2;

        gsl_rng *rng = gsl_rng_alloc(gsl_rng_mt19937);
        gsl_rng_set(rng, time(NULL));

        switch (hotspotShape)
        {
        case HotspotShape::SPHERE:
        {
            double radius = params.begin()[0];
            int n = density * (4.0 / 3.0) * PI * pow(radius, 3) * percentage;

            std::cout << n << std::endl;

            // generate n particles in the sphere
            for (int j = 0; j < n; j++)
            {
                double x = gsl_ran_flat(rng, -radius, radius) + xMid;
                double y = gsl_ran_flat(rng, -radius, radius) + yMid;
                double z = gsl_ran_flat(rng, -radius, radius) + zMid;

                double vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature / mass));
                double vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature / mass));
                double vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature / mass));

                Particle *newParticle = new Particle(particle.alias, mass, charge, Point(x, y, z), Velocity(vx, vy, vz));

                this->insert(newParticle);
                generatedParticles.push_back(newParticle);
            }
            break;
        }
        default:
            break;
        }
    }

    this->recalculateCentreOfCharge();

    return generatedParticles;
}

void Space::recalculateCentreOfCharge()
{
    Charge totalCharge = Charge(0, 0);
    double xPositiveChargePositionProductSum = 0;
    double yPositiveChargePositionProductSum = 0;
    double zPositiveChargePositionProductSum = 0;

    double xNegativeChargePositionProductSum = 0;
    double yNegativeChargePositionProductSum = 0;
    double zNegativeChargePositionProductSum = 0;

    for (Node *child : this->children)
    {
        if (dynamic_cast<Particle *>(child))
        {
            Particle *particle = dynamic_cast<Particle *>(child);
            totalCharge += particle->charge;

            xPositiveChargePositionProductSum += particle->charge.positive * particle->position.x;
            yPositiveChargePositionProductSum += particle->charge.positive * particle->position.y;
            zPositiveChargePositionProductSum += particle->charge.positive * particle->position.z;

            xNegativeChargePositionProductSum += particle->charge.negative * particle->position.x;
            yNegativeChargePositionProductSum += particle->charge.negative * particle->position.y;
            zNegativeChargePositionProductSum += particle->charge.negative * particle->position.z;
        }
        else if (dynamic_cast<Space *>(child))
        {
            Space *space = dynamic_cast<Space *>(child);
            space->recalculateCentreOfCharge();

            totalCharge += space->charge;
            xPositiveChargePositionProductSum += space->charge.positive * space->centreOfCharge.positive.x;
            yPositiveChargePositionProductSum += space->charge.positive * space->centreOfCharge.positive.y;
            zPositiveChargePositionProductSum += space->charge.positive * space->centreOfCharge.positive.z;

            xNegativeChargePositionProductSum += space->charge.negative * space->centreOfCharge.negative.x;
            yNegativeChargePositionProductSum += space->charge.negative * space->centreOfCharge.negative.y;
            zNegativeChargePositionProductSum += space->charge.negative * space->centreOfCharge.negative.z;
        }
    }

    this->charge = totalCharge;
    this->centreOfCharge = Points(Point(xPositiveChargePositionProductSum / totalCharge.positive,
                                         yPositiveChargePositionProductSum / totalCharge.positive,
                                         zPositiveChargePositionProductSum / totalCharge.positive),
                                  Point(xNegativeChargePositionProductSum / totalCharge.negative,
                                         yNegativeChargePositionProductSum / totalCharge.negative,
                                         zNegativeChargePositionProductSum / totalCharge.negative));
}

std::string Space::toString(std::string indent)
{
    std::string out = "\033[34mSpace (" + std::to_string(minPoint.x) + ", " + std::to_string(minPoint.y) + ", " + std::to_string(minPoint.z) + ") to (" + std::to_string(maxPoint.x) + ", " + std::to_string(maxPoint.y) + ", " + std::to_string(maxPoint.z) + ")\033[0m\n";
    std::string branchSymbol = "├── ";
    std::string lastBranchSymbol = "└── ";

    for (int i = 0; i <= o8; i++)
    {
        bool isLast = i == o8;

        if (children[i] == nullptr)
        {
            out += indent + (isLast ? lastBranchSymbol : branchSymbol) + "\033[35mEmpty\033[0m\n";
        }
        else if (dynamic_cast<Particle *>(children[i]))
        {
            Particle *particle = dynamic_cast<Particle *>(children[i]);

            out += indent + (isLast ? lastBranchSymbol : branchSymbol) + "\033[32m" + particle->alias + " (" + std::to_string(particle->position.x) + ", " + std::to_string(particle->position.y) + ", " + std::to_string(particle->position.z) + ")" + "\033[0m\n";
        }
        else if (dynamic_cast<Space *>(children[i]))
        {
            Space *space = dynamic_cast<Space *>(children[i]);

            out += indent + (isLast ? lastBranchSymbol : branchSymbol) + space->toString(indent + (isLast ? "    " : "│   "));
        }
        else
        {
            throw std::runtime_error("Unknown type (this isn't even supposed to run lmao wtf)");
        }
    }

    return out;
}

void Space::toFile(int timeStep, std::string path, std::ios_base::openmode openMode)
{
    std::ofstream file;
    file.open(path, openMode);

    auto particles = this->getAllParticles();
    for(auto particle : particles)
    {
        file <<
        timeStep << "," <<
        particle->alias << "," <<
        particle->position.x << "," <<
        particle->position.y << "," <<
        particle->position.z << ",";
    }
}