#ifndef P_CATS_H
#define P_CATS_H

#include <limits>
#include <vector>
#include <string>
#include <cmath>

/// Global Variables
constexpr double PI = 3.14159265358979323846; 
constexpr double K_B = 1.380649e-23; // Boltzmann constant in J/K
constexpr double epsilon0 = 8.8541878188e-12; // Permittivity of free space in C^2/(N m^2)
constexpr double mu0 = 4 * PI * 1e-7; // Permeability of free space in N/[(C/s)^2]
constexpr double e = 1.602176634e-19 ; // Elementary charge in C

enum HotspotShape
{
    SPHERE,
};

/// @brief structure to store force components (in N).
///
/// A structure that stores the x, y, and z components of a force. Units are in Newtons.
struct Force
{
    double x, y, z;

    Force()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }

    /// @brief Constructor for Force.
    /// @param x x-component of the force in Newtons.
    /// @param y y-component of the force in Newtons.
    /// @param z z-component of the force in Newtons.
    Force(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /// @brief Addition operator for Force.
    /// @param f Force to add.
    /// @return The sum of the two forces.
    Force operator+(const Force &f) const
    {
        return Force(x + f.x, y + f.y, z + f.z);
    }

    /// @brief Subtraction operator for Force.
    /// @param f Force to subtract.
    /// @return The difference of the two forces.
    Force operator-(const Force &f) const
    {
        return Force(x - f.x, y - f.y, z - f.z);
    }

    /// @brief Equals operator for Force.
    /// @param f Force to compare.
    /// @return True if the two forces are equal, false otherwise.
    bool operator==(const Force &f) const
    {
        return x == f.x && y == f.y && z == f.z;
    }
};

/// @brief A structure to store velocity components (in m/s).
///
/// A structure that stores the x, y, and z components of a velocity. Units are in meters per second.
struct Velocity
{
    double x, y, z;

    Velocity()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }

    /// @brief Constructor for Velocity.
    /// @param x x-component of the velocity in m/s.
    /// @param y y-component of the velocity in m/s.
    /// @param z z-component of the velocity in m/s.
    Velocity(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /// @brief Addition operator for Velocity.
    /// @param v Velocity to add.
    /// @return The sum of the two velocities.
    Velocity operator+(const Velocity &v) const
    {
        return Velocity(x + v.x, y + v.y, z + v.z);
    }

    /// @brief Subtraction operator for Velocity.
    /// @param v Velocity to subtract.
    /// @return The difference of the two velocities.
    Velocity operator-(const Velocity &v) const
    {
        return Velocity(x - v.x, y - v.y, z - v.z);
    }

    /// @brief Equals operator for Velocity.
    /// @param v Velocity to compare.
    /// @return True if the two velocities are equal, false otherwise.
    bool operator==(const Velocity &v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }
};

/// @brief A structure to store position components (in m).
///
/// A structure that stores the x, y, and z components of a position. Units are in meters.
struct Point
{
    double x, y, z;

    Point()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }

    /// @brief Constructor for Point.
    /// @param x x-component of the position in meters.
    /// @param y y-component of the position in meters.
    /// @param z z-component of the position in meters.
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /// @brief Subtraction operator for Point.
    /// @param p Point to subtract.
    /// @return The difference of the two points.
    Point operator-(const Point &p) const
    {
        return Point(x - p.x, y - p.y, z - p.z);
    }

    /// @brief Addition operator for Point.
    /// @param p Point to add.
    /// @return The sum of the two points.
    Point operator+(const Point &p) const
    {
        return Point(x + p.x, y + p.y, z + p.z);
    }

    /// @brief Equals operator for Point.
    /// @param p Point to compare.
    /// @return True if the two points are equal, false otherwise.
    bool operator==(const Point &p) const
    {
        return x == p.x && y == p.y && z == p.z;
    }

    /// @brief magnitude of the position vector
    /// @return magnitude of the position vector
    double magnitude()
    {
        return sqrt(x * x + y * y + z * z);
    }
};

/// @brief A structure to store field components (in N/C).
///
/// A structure that stores the x, y, and z components of a field. Units are in Newtons per Coulomb.
struct Field
{
    double x, y, z;

    Field()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }

    /// @brief Constructor for Field.
    /// @param x x-component of the field in Newtons per Coulomb.
    /// @param y y-component of the field in Newtons per Coulomb.
    /// @param z z-component of the field in Newtons per Coulomb.
    Field(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /// @brief Addition operator for Field.
    /// @param f Field to add.
    /// @return The sum of the two fields.
    Field operator+(const Field &f) const
    {
        return Field(x + f.x, y + f.y, z + f.z);
    }

    /// @brief Subtraction operator for Field.
    /// @param f Field to subtract.
    /// @return The difference of the two fields.
    Field operator-(const Field &f) const
    {
        return Field(x - f.x, y - f.y, z - f.z);
    }

    /// @brief Equals operator for Field.
    /// @param f Field to compare.
    /// @return True if the two fields are equal, false otherwise.
    bool operator==(const Field &f) const
    {
        return x == f.x && y == f.y && z == f.z;
    }
};

/// @brief A structure to store charge components (in C).
///
/// A structure that stores the positive and negative charge of a particle. Units are in Coulombs.
struct Charge
{
    double positive, negative;

    Charge()
    {
        positive = std::numeric_limits<double>::quiet_NaN();
        negative = std::numeric_limits<double>::quiet_NaN();
    }

    /// @brief Constructor for Charge.
    /// @param p Positive charge in Coulombs.
    /// @param n Negative charge in Coulombs.
    Charge(double p, double n)
    {
        positive = p;
        negative = n;
    }

    /// @brief Addition operator for Charge.
    /// @param c Charge to add.
    /// @return The sum of the two charges.
    Charge operator+(const Charge &c) const
    {
        return Charge(positive + c.positive, negative + c.negative);
    }

    /// @brief Subtraction operator for Charge.
    /// @param c Charge to subtract.
    /// @return The difference of the two charges.
    Charge operator-(const Charge &c) const
    {
        return Charge(positive - c.positive, negative - c.negative);
    }

    /// @brief Plus-equals operator for Charge.
    /// @param c Charge to add.
    /// @return Charge after addition.
    Charge operator+=(const Charge &c)
    {
        positive += c.positive;
        negative += c.negative;
        return *this;
    }

    /// @brief Minus-equals operator for Charge.
    /// @param c Charge to subtract.
    /// @return Charge after subtraction.
    Charge operator-=(const Charge &c)
    {
        positive -= c.positive;
        negative -= c.negative;
        return *this;
    }

    /// @brief equals operator for Charge.
    /// @param c Charge to compare.
    /// @return True if the two charges are equal, false otherwise.
    bool operator==(const Charge &c) const
    {
        return positive == c.positive && negative == c.negative;
    }
};

/// @brief A structure to store positive and negative average charge positions (in C).
struct Points {
    Point positive, negative;

    Points()
    {
        positive = Point();
        negative = Point();
    }

    /// @brief Constructor for Points.
    /// @param p Positive charge position in meters.
    /// @param n Negative charge position in meters.
    Points(Point p, Point n)
    {
        positive = p;
        negative = n;
    }

    /// @brief Equals operator for Points.
    /// @param p Points to compare.
    /// @return True if the two points are equal, false otherwise.
    bool operator==(const Points &p) const
    {
        return positive == p.positive && negative == p.negative;
    }
};

#endif