#include "include/p_cats.h"
#include "include/space.h"
#include "include/node.h"
#include "include/particle.h"
#include <iostream>
#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <fstream>
#include <tuple>
#include <iomanip>
#include <gmpxx.h>


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
    mpf_t xSum, ySum, zSum;
    mpf_t xMid, yMid, zMid;
    mpf_init(xSum);
    mpf_init(ySum);
    mpf_init(zSum);
    mpf_init(xMid);
    mpf_init(yMid);
    mpf_init(zMid);

    mpf_add(xSum, minPoint.x, maxPoint.x);
    mpf_add(ySum, minPoint.y, maxPoint.y);
    mpf_add(zSum, minPoint.z, maxPoint.z);

    mpf_div_ui(xMid, xSum, 2);
    mpf_div_ui(yMid, ySum, 2);
    mpf_div_ui(zMid, zSum, 2);

    return Point(xMid, yMid, zMid);
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

    mpf_t xMid, yMid, zMid;
    mpf_init(xMid);
    mpf_init(yMid);
    mpf_init(zMid);

    mpf_t xSum, ySum, zSum;
    mpf_init(xSum);
    mpf_init(ySum);
    mpf_init(zSum);

    mpf_add(xSum, minPoint.x, maxPoint.x);
    mpf_add(ySum, minPoint.y, maxPoint.y);
    mpf_add(zSum, minPoint.z, maxPoint.z);

    mpf_div_ui(xMid, xSum, 2);
    mpf_div_ui(yMid, ySum, 2);
    mpf_div_ui(zMid, zSum, 2);

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
        else // left.
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

        // Calculate the midpoint of the space
        mpf_t xMid, yMid, zMid;
        mpf_init(xMid);
        mpf_init(yMid);
        mpf_init(zMid);

        mpf_t xSum, ySum, zSum;
        mpf_init(xSum);
        mpf_init(ySum);
        mpf_init(zSum);

        mpf_add(xSum, minPoint.x, maxPoint.x);
        mpf_add(ySum, minPoint.y, maxPoint.y);
        mpf_add(zSum, minPoint.z, maxPoint.z);

        mpf_div_ui(xMid, xSum, 2);
        mpf_div_ui(yMid, ySum, 2);
        mpf_div_ui(zMid, zSum, 2);

        gsl_rng *rng = gsl_rng_alloc(gsl_rng_mt19937);
        gsl_rng_set(rng, time(NULL));

        switch (hotspotShape)
        {
        case HotspotShape::SPHERE:
        {
            double radius = params.begin()[0];
            int n = density * (4.0 / 3.0) * mpf_get_d(PI) * pow(radius, 3) * percentage;

            // std::cout << n << std::endl;

            // generate n particles in the sphere
            for (int j = 0; j < n; j++)
            {
                double x = gsl_ran_flat(rng, -radius, radius) + mpf_get_d(xMid);
                double y = gsl_ran_flat(rng, -radius, radius) + mpf_get_d(yMid);
                double z = gsl_ran_flat(rng, -radius, radius) + mpf_get_d(zMid);

                double vx = gsl_ran_gaussian(rng, sqrt(mpf_get_d(K_B) * temperature / mass));
                double vy = gsl_ran_gaussian(rng, sqrt(mpf_get_d(K_B) * temperature / mass));
                double vz = gsl_ran_gaussian(rng, sqrt(mpf_get_d(K_B) * temperature / mass));

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
    mpf_t xPositiveChargePositionProductSum, yPositiveChargePositionProductSum, zPositiveChargePositionProductSum;
    mpf_init_set_ui(xPositiveChargePositionProductSum, 0);
    mpf_init_set_ui(yPositiveChargePositionProductSum, 0);
    mpf_init_set_ui(zPositiveChargePositionProductSum, 0);

    mpf_t xNegativeChargePositionProductSum, yNegativeChargePositionProductSum, zNegativeChargePositionProductSum;
    mpf_init_set_ui(xNegativeChargePositionProductSum, 0);
    mpf_init_set_ui(yNegativeChargePositionProductSum, 0);
    mpf_init_set_ui(zNegativeChargePositionProductSum, 0);

    for (Node *child : this->children)
    {
        if (dynamic_cast<Particle *>(child))
        {
            Particle *particle = dynamic_cast<Particle *>(child);
            totalCharge += particle->charge;

            mpf_t xPositiveChargePositionProduct, yPositiveChargePositionProduct, zPositiveChargePositionProduct;
            mpf_init(xPositiveChargePositionProduct);
            mpf_init(yPositiveChargePositionProduct);
            mpf_init(zPositiveChargePositionProduct);

            mpf_t xNegativeChargePositionProduct, yNegativeChargePositionProduct, zNegativeChargePositionProduct;
            mpf_init(xNegativeChargePositionProduct);
            mpf_init(yNegativeChargePositionProduct);
            mpf_init(zNegativeChargePositionProduct);

            mpf_mul(xPositiveChargePositionProduct, particle->charge.positive, particle->position.x);
            mpf_mul(yPositiveChargePositionProduct, particle->charge.positive, particle->position.y);
            mpf_mul(zPositiveChargePositionProduct, particle->charge.positive, particle->position.z);

            mpf_mul(xNegativeChargePositionProduct, particle->charge.negative, particle->position.x);
            mpf_mul(yNegativeChargePositionProduct, particle->charge.negative, particle->position.y);
            mpf_mul(zNegativeChargePositionProduct, particle->charge.negative, particle->position.z);

            mpf_add(xPositiveChargePositionProductSum, xPositiveChargePositionProductSum, xPositiveChargePositionProduct);
            mpf_add(yPositiveChargePositionProductSum, yPositiveChargePositionProductSum, yPositiveChargePositionProduct);
            mpf_add(zPositiveChargePositionProductSum, zPositiveChargePositionProductSum, zPositiveChargePositionProduct);

            mpf_add(xNegativeChargePositionProductSum, xNegativeChargePositionProductSum, xNegativeChargePositionProduct);
            mpf_add(yNegativeChargePositionProductSum, yNegativeChargePositionProductSum, yNegativeChargePositionProduct);
            mpf_add(zNegativeChargePositionProductSum, zNegativeChargePositionProductSum, zNegativeChargePositionProduct);
        }
        else if (dynamic_cast<Space *>(child))
        {
            Space *space = dynamic_cast<Space *>(child);
            space->recalculateCentreOfCharge();

            totalCharge += space->charge;

            mpf_t xPositiveChargePositionProduct, yPositiveChargePositionProduct, zPositiveChargePositionProduct;
            mpf_init(xPositiveChargePositionProduct);
            mpf_init(yPositiveChargePositionProduct);
            mpf_init(zPositiveChargePositionProduct);

            mpf_t xNegativeChargePositionProduct, yNegativeChargePositionProduct, zNegativeChargePositionProduct;
            mpf_init(xNegativeChargePositionProduct);
            mpf_init(yNegativeChargePositionProduct);
            mpf_init(zNegativeChargePositionProduct);

            mpf_mul(xPositiveChargePositionProduct, space->charge.positive, space->centreOfCharge.positive.x);
            mpf_mul(yPositiveChargePositionProduct, space->charge.positive, space->centreOfCharge.positive.y);
            mpf_mul(zPositiveChargePositionProduct, space->charge.positive, space->centreOfCharge.positive.z);

            mpf_mul(xNegativeChargePositionProduct, space->charge.negative, space->centreOfCharge.negative.x);
            mpf_mul(yNegativeChargePositionProduct, space->charge.negative, space->centreOfCharge.negative.y);
            mpf_mul(zNegativeChargePositionProduct, space->charge.negative, space->centreOfCharge.negative.z);

            mpf_add(xPositiveChargePositionProductSum, xPositiveChargePositionProductSum, xPositiveChargePositionProduct);
            mpf_add(yPositiveChargePositionProductSum, yPositiveChargePositionProductSum, yPositiveChargePositionProduct);
            mpf_add(zPositiveChargePositionProductSum, zPositiveChargePositionProductSum, zPositiveChargePositionProduct);

            mpf_add(xNegativeChargePositionProductSum, xNegativeChargePositionProductSum, xNegativeChargePositionProduct);
            mpf_add(yNegativeChargePositionProductSum, yNegativeChargePositionProductSum, yNegativeChargePositionProduct);
            mpf_add(zNegativeChargePositionProductSum, zNegativeChargePositionProductSum, zNegativeChargePositionProduct);
        }
    }

    this->charge = totalCharge;

    mpf_t xPositiveCentreOfCharge, yPositiveCentreOfCharge, zPositiveCentreOfCharge;
    mpf_init(xPositiveCentreOfCharge);
    mpf_init(yPositiveCentreOfCharge);
    mpf_init(zPositiveCentreOfCharge);

    mpf_t xNegativeCentreOfCharge, yNegativeCentreOfCharge, zNegativeCentreOfCharge;
    mpf_init(xNegativeCentreOfCharge);
    mpf_init(yNegativeCentreOfCharge);
    mpf_init(zNegativeCentreOfCharge);

    mpf_div(xPositiveCentreOfCharge, xPositiveChargePositionProductSum, totalCharge.positive);
    mpf_div(yPositiveCentreOfCharge, yPositiveChargePositionProductSum, totalCharge.positive);
    mpf_div(zPositiveCentreOfCharge, zPositiveChargePositionProductSum, totalCharge.positive);

    mpf_div(xNegativeCentreOfCharge, xNegativeChargePositionProductSum, totalCharge.negative);
    mpf_div(yNegativeCentreOfCharge, yNegativeChargePositionProductSum, totalCharge.negative);
    mpf_div(zNegativeCentreOfCharge, zNegativeChargePositionProductSum, totalCharge.negative);

    this->centreOfCharge = Points(Point(xPositiveCentreOfCharge, yPositiveCentreOfCharge, zPositiveCentreOfCharge),
                                  Point(xNegativeCentreOfCharge, yNegativeCentreOfCharge, zNegativeCentreOfCharge));
}

std::string Space::toString(std::string indent)
{
    std::string out = "\033[34mSpace (" + std::to_string(mpf_get_d(minPoint.x)) + ", " + std::to_string(mpf_get_d(minPoint.y)) + ", " + std::to_string(mpf_get_d(minPoint.z)) + ") to (" + std::to_string(mpf_get_d(maxPoint.x)) + ", " + std::to_string(mpf_get_d(maxPoint.y)) + ", " + std::to_string(mpf_get_d(maxPoint.z)) + ")\033[0m\n";
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

            out += indent + (isLast ? lastBranchSymbol : branchSymbol) + "\033[32m" + particle->alias + " (" + std::to_string(mpf_get_d(particle->position.x)) + ", " + std::to_string(mpf_get_d(particle->position.y)) + ", " + std::to_string(mpf_get_d(particle->position.z)) + ")" + "\033[0m\n";
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
    std::cout << "Writing ~" << particles.size() << " particles to file..." << std::endl;
    for(auto particle : particles)
    {
        file << std::fixed <<
        timeStep << "," << particle->position.x << "," << particle->position.y << "," << particle->position.z << "," <<
        particle->alias << "\n";
    }
}
