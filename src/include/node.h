#ifndef NODE_H
#define NODE_H

#include "p_cats.h"

/// @brief A structure to store a node in the tree.
///
/// A structure that stores a node in the tree. This can be a particle or a space.
/// The node has a charge and a mass. @zizhuo work on this part
class Node
{
public:
    Charge charge;

    /// @brief Constructor for Node.
    /// @param charge Charge of the node.
    Node(Charge charge);

    /// @brief Polymporphic function to calculate the midpoint of the node.
    /// @return Midpoint of the node.
    virtual Point midpoint();

    /// @brief Function to check if the space is an external node.
    /// @return True if the space is an external node, false otherwise.
    virtual bool isExternalNode();

    virtual std::vector<Node> getChildren();

    virtual Points getCentreOfCharge();
};

#endif