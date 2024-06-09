#include "p_cats.h"
#include "node.h"

/// @brief A structure to store a space node in the tree.
///
/// A structure that stores a space node in the tree. This node has a minimum and maximum point, as well as a centre of positive and negative charge.
class Space : public Node
{
public:
    Point minPoint, maxPoint;
    Point centreOfPositiveCharge, centreOfNegativeCharge;
    std::vector<Node *> children;

    /// @brief Constructor for Space.
    /// @param minPoint minimum point of the space.
    /// @param maxPoint maximum point of the space.
    /// @param mass total mass of the space.
    /// @param charge total charge of the space.
    Space(Point minPoint, Point maxPoint, double mass, Charge charge);

    /// @brief Function to insert a node into the space.
    /// @param node Node to be inserted.
    void insert(Node *node);

    /// @brief Function to find a point in the space.
    /// @param point Point to find.
    /// @return True if the point is in the space, false otherwise.
    bool find(Point point);

    /// @brief Function to check if the space is an external node.
    /// @return True if the space is an external node, false otherwise.
    bool isExternalNode();
};