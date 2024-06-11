#include <limits>
#include <vector>
#include <string>

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

    virtual ~Node() {}
    Node(Charge charge);
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

    Particle(std::string alias, 
        double mass, 
        Charge charge,
        Point position = Point(), 
        Velocity velocity = Velocity(), 
        Force bForce = Force(), 
        Force eForce = Force());
    void updatePosition(double dt);
    void addForce(Node *node);
};

class Space : public Node
{
public:
    Point minPoint, maxPoint;
    Point centreOfPositiveCharge, centreOfNegativeCharge;
    std::vector<Node *> children;

    Space(Point minPoint, Point maxPoint, Charge charge = Charge(0, 0));
    void insert(Particle *particle);
    bool find(Point point);
    bool isExternalNode();
    // std::vector<Particle *> generateParticles(double density, double temperature, std::vector<Particle> particles);
    std::string toString(int depth = 0, bool isLastBranch = false);
};