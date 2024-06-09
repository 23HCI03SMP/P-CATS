#include <limits>
#include <vector>
#include <string>

/// A structure to store force components (in Newtons).
struct Force
{
    double x, y, z;

    Force()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }

    Force(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

/*! A structure to store velocity components (in m/s)*/

struct Velocity
{
    double x, y, z;

    Velocity()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }

    Velocity(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

/*! A structure to store position components (in m)*/
struct Point
{
    double x, y, z;

    Point()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }

    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

/*! A structure to store charge components (in Coulombs)*/
struct Charge
{
    double positive, negative;

    Charge()
    {
        positive = std::numeric_limits<double>::quiet_NaN();
        negative = std::numeric_limits<double>::quiet_NaN();
    }

    Charge(double p, double n)
    {
        positive = p;
        negative = n;
    }

    Charge operator+(const Charge &c) const
    {
        return Charge(positive + c.positive, negative + c.negative);
    }

    Charge operator-(const Charge &c) const
    {
        return Charge(positive - c.positive, negative - c.negative);
    }

    Charge operator+=(const Charge &c)
    {
        positive += c.positive;
        negative += c.negative;
        return *this;
    }

    Charge operator-=(const Charge &c)
    {
        positive -= c.positive;
        negative -= c.negative;
        return *this;
    }
};

class Node
{
public:
    Charge charge;

    Node(double mass, Charge charge);
};

class Space : public Node
{
public:
    Point minPoint, maxPoint;
    Point centreOfPositiveCharge, centreOfNegativeCharge;
    std::vector<Node *> children;

    Space(Point minPoint, Point maxPoint, double mass, Charge charge);
    void insert(Node *node);
    bool find(Point point);
    bool isExternalNode();
};

class Particle : public Node
{
public:
    std::string alias;
    double mass;
    Point position;
    Velocity velocity;
    Force bForce;
    Force eForce;

    Particle(std::string alias, Point position, Velocity velocity, Force bForce, Force eForce, double mass, Charge charge);
    void updatePosition(double dt);
    void addForce(Node *node);
};