#ifndef SPACE_H
#define SPACE_H

#include "p_cats.h"
#include "particle.h"
#include <vector>

class Particle;

/// @brief A structure to store a space node in the tree.
///
/// A structure that stores a space node in the tree. This node has a minimum and maximum point, as well as a centre of positive and negative charge.
class Space : public Node
{
public:
    Point minPoint, maxPoint;
    Point centreOfPositiveCharge, centreOfNegativeCharge;
    std::vector<Node *> children;
    std::vector<Node *> getChildren() override;

    /// @brief Constructor for Space.
    /// @param minPoint minimum point of the space.
    /// @param maxPoint maximum point of the space.
    /// @param mass total mass of the space.
    /// @param charge total charge of the space.
    Space(Point minPoint, Point maxPoint, Charge charge = Charge());

    /// @brief Polymporphic function to calculate the midpoint of the space.
    /// @return Midpoint of the space.
    Point midpoint() override;

    /// @brief Function to insert a node into the space.
    /// @param node Node to be inserted.
    void insert(Particle *particle);
    
    /// @brief Function to check if the space is an external node.
    /// @return True if the space is an external node, false otherwise.
    bool isExternalNode() override;

    // std::vector<Particle *> generateParticles(double density, double temperature, std::vector<Particle> particles);
    std::string toString(int depth = 0, bool isLastBranch = false);
};

#endif