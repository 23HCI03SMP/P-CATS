#include "include/p_cats.h"
#include "include/particle.h"
#include "include/node.h"

Node::Node(Charge charge)
{
    this->charge = charge;
}

Point Node::midpoint() {
    // This is a pure virtual function
}

/// @todo Illegal instruction for some reason
bool Node::isExternalNode() {
    // This is a pure virtual function
}

std::vector<Node> Node::getChildren() {
    // This is a pure virtual function
}

Points Node::getCentreOfCharge() {
    // This is a pure virtual function
}