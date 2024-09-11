#ifndef SPACE_H
#define SPACE_H

#include "p_cats.h"
#include "particle.h"
#include <vector>
#include <fstream>

class Particle;

/// @brief A structure to store a space node in the tree.
///
/// A structure that stores a space node in the tree. This node has a minimum and maximum point, as well as a centre of positive and negative charge.
class Space : public Node
{
public:
    Point minPoint, maxPoint;
    Points centreOfCharge;
    Points getCentreOfCharge() override;
    std::vector<Node *> children;
    std::vector<Node *> getChildren() override;

    std::vector<Particle *> getAllParticles();

    /// @brief Constructor for Space.
    /// @param minPoint minimum point of the space.
    /// @param maxPoint maximum point of the space.
    /// @param mass total mass of the space.
    /// @param charge total charge of the space.
    Space(Point minPoint, Point maxPoint, Charge charge = Charge());

    /// @brief Destructor for Space.
    ~Space();

    /// @brief Polymporphic function to calculate the midpoint of the space.
    /// @return Midpoint of the space.
    Point midpoint() override;

    /// @brief Function to insert a node into the space.
    /// @param node Node to be inserted.
    void insert(Particle *particle);
    
    /// @brief Function to check if the space is an external node.
    /// @return True if the space is an external node, false otherwise.
    bool isExternalNode() override;

    /// @brief Function to generate particles in the space.
    /// @param density Density of the particles, in <@UnidentifiedX do later>.
    /// @param temperature Temperature of the particles, in Kelvin.
    /// @param particles Vector of particles to be generated. Takes in a tuple of a particle and a double. Particle is the particle to be generated, and the double is the percentage of the particle being generated.
    /// @param hotspotShape Shape of the hotspot.
    /// @param params Parameters for the hotspot shape. 
    /// @return Vector of particles generated.
    void generateParticles(double density,
                                                   double temperature,
                                                   std::vector<std::tuple<Particle, double>> &particles,
                                                   HotspotShape hotspotShape,
                                                   std::initializer_list<double> params);

    void recalculateCentreOfCharge();

    /// @brief Function to convert the space to a string.
    std::string toString(std::string indent = "");

    /// @brief Function to outpout the space to a file.
    /// @param path Path to the file.
    /// @param mode Mode to open the file in. Defaults to std::ios_base::app (append).
    void toFile(int timeStep, std::string path, std::ios_base::openmode mode = std::ios_base::app);
};

#endif