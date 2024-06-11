#include "p_cats.h"

/// @brief A structure to store a node in the tree.
///
/// A structure that stores a node in the tree. This can be a particle or a space.
/// The node has a charge and a mass. @zizhuo work on this part
class Node
{
public:
    Charge charge;

    virtual ~Node() {}
    Node(Charge charge);
};