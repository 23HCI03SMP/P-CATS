#include "include/p_cats.h"
#include <iostream>

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
    return children.size() == 0;
}

Space::Space(Point minPoint, Point maxPoint, Charge charge) : Node(charge)
{
    this->minPoint = minPoint;
    this->maxPoint = maxPoint;
    this->centreOfPositiveCharge = Point();
    this->centreOfNegativeCharge = Point();
    
    for (int i = 0; i <= o8; i++)
    {
        children.push_back(nullptr);
    }
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
        this->children[octet] = particle;
    }
    else
    {
        // print type of children[octet]
        // std::cout << "Type: " << typeid(this->children[octet]).name() << std::endl;
        if (dynamic_cast<Particle *>(this->children[octet]) != nullptr)
        {
            Particle *oldParticle = dynamic_cast<Particle *>(this->children[octet]);
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
    }
}

std::string GetIndentString(int depth)
{
    std::string out = "";

    for (int i = 0; i < depth; i++)
    {
        out += "│   ";
    }

    return out;
}

std::string Space::toString(int depth, bool isLastBranch)
{
    std::string out = "\033Space (" 
                    + std::to_string(minPoint.x) 
                    + ", " + std::to_string(minPoint.y) 
                    + ", " + std::to_string(minPoint.z) 
                    + ") to (" 
                    + std::to_string(maxPoint.x) 
                    + ", " + std::to_string(maxPoint.y) 
                    + ", " 
                    + std::to_string(maxPoint.z) 
                    + ")\n";
    std::string branchSymbol = "├── ";
    std::string lastBranchSymbol = "└── ";

    for (int i = 0; i <= o8; i++)
    {
        std::string currentBranchSymbol;
        if (i == o8)
        {
            currentBranchSymbol = lastBranchSymbol;
            isLastBranch = depth == 0 || isLastBranch;
        }
        else
        {
            currentBranchSymbol = branchSymbol;
        }

        if (children[i] == nullptr)
        {
            out += (isLastBranch && depth != 0 ? std::string(depth * 4, ' ') : GetIndentString(depth))
                + currentBranchSymbol
                + "Empty\n";
        }
        else
        {
            if (dynamic_cast<Particle *>(children[i]))
            {
                Particle *particle = dynamic_cast<Particle *>(children[i]);

                out += (isLastBranch && depth != 0 ? std::string(depth * 4, ' ') : GetIndentString(depth))
                    + currentBranchSymbol
                    + particle->alias 
                    + " (" 
                    + std::to_string(particle->position.x) 
                    + ", " 
                    + std::to_string(particle->position.y)
                    + ", "
                    + std::to_string(particle->position.z)
                    + ")"
                    + "\n";
            }
            else if (dynamic_cast<Space *>(children[i])) // If its a space, then we need to run recursively over its children
            {
                Space *space = dynamic_cast<Space *>(children[i]);

                out += (isLastBranch ? std::string(depth * 4, ' ') : GetIndentString(depth))
                    + currentBranchSymbol
                    + space->toString(depth + 1, isLastBranch);
            }
            else
            {
                throw std::runtime_error("Unknown type (this isn't even supposed to run lmao wtf)");
            }
        }
    }

    return out;
}