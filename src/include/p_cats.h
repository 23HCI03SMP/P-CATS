#ifndef P_CATS_H
#define P_CATS_H

#include <limits>
#include <vector>
#include <string>
#include <cmath>
#include <gmpxx.h> // Include the gmpxx header for mpf_t

/// Global Variables
extern mpf_t PI;
extern mpf_t K_B;
extern mpf_t epsilon0;
extern mpf_t mu0;
extern mpf_t e;
extern mpf_t amu;

enum HotspotShape
{
    SPHERE,
};

/// @brief structure to store force components (in N).
///
/// A structure that stores the x, y, and z components of a force. Units are in Newtons.
struct Force
{
    mpf_t x, y, z;

    Force()
    {
        mpf_init(x);
        mpf_init(y);
        mpf_init(z);
    }

    /// @brief Destructor for Force.
    ~Force()
    {
        // mpf_clear(x);
        // mpf_clear(y);
        // mpf_clear(z);
    }

    /// @brief Constructor for Force using double values.
    /// @param x x-component of the force in Newtons (as double).
    /// @param y y-component of the force in Newtons (as double).
    /// @param z z-component of the force in Newtons (as double).
    Force(double x_val, double y_val, double z_val)
    {
        mpf_init(x);
        mpf_init(y);
        mpf_init(z);
        mpf_set_d(x, x_val); // Convert double to mpf_t
        mpf_set_d(y, y_val); // Convert double to mpf_t
        mpf_set_d(z, z_val); // Convert double to mpf_t
    }

    /// @brief Addition operator for Force.
    Force operator+(const Force &f) const
    {
        Force result;
        mpf_add(result.x, x, f.x);
        mpf_add(result.y, y, f.y);
        mpf_add(result.z, z, f.z);
        return result;
    }

    /// @brief Subtraction operator for Force.
    Force operator-(const Force &f) const
    {
        Force result;
        mpf_sub(result.x, x, f.x);
        mpf_sub(result.y, y, f.y);
        mpf_sub(result.z, z, f.z);
        return result;
    }

    /// @brief Equals operator for Force.
    bool operator==(const Force &f) const
    {
        return mpf_cmp(x, f.x) == 0 && mpf_cmp(y, f.y) == 0 && mpf_cmp(z, f.z) == 0;
    }
};

/// @brief A structure to store velocity components (in m/s).
///
/// A structure that stores the x, y, and z components of a velocity. Units are in meters per second.
struct Velocity
{
    mpf_t x, y, z;

    Velocity()
    {
        mpf_init(x);
        mpf_init(y);
        mpf_init(z);
    }

    /// @brief Destructor for Velocity.
    ~Velocity()
    {
        // mpf_clear(x);
        // mpf_clear(y);
        // mpf_clear(z);
    }

    /// @brief Constructor for Velocity using double values.
    /// @param x x-component of the velocity in m/s (as double).
    /// @param y y-component of the velocity in m/s (as double).
    /// @param z z-component of the velocity in m/s (as double).
    Velocity(double x_val, double y_val, double z_val)
    {
        mpf_init(x);
        mpf_init(y);
        mpf_init(z);
        mpf_set_d(x, x_val); // Convert double to mpf_t
        mpf_set_d(y, y_val); // Convert double to mpf_t
        mpf_set_d(z, z_val); // Convert double to mpf_t
    }

    /// @brief Addition operator for Velocity.
    Velocity operator+(const Velocity &v) const
    {
        Velocity result;
        mpf_add(result.x, x, v.x);
        mpf_add(result.y, y, v.y);
        mpf_add(result.z, z, v.z);
        return result;
    }

    /// @brief Subtraction operator for Velocity.
    Velocity operator-(const Velocity &v) const
    {
        Velocity result;
        mpf_sub(result.x, x, v.x);
        mpf_sub(result.y, y, v.y);
        mpf_sub(result.z, z, v.z);
        return result;
    }

    /// @brief Equals operator for Velocity.
    bool operator==(const Velocity &v) const
    {
        return mpf_cmp(x, v.x) == 0 && mpf_cmp(y, v.y) == 0 && mpf_cmp(z, v.z) == 0;
    }
};

/// @brief A structure to store position components (in m).
///
/// A structure that stores the x, y, and z components of a position. Units are in meters.
struct Point
{
    mpf_t x, y, z;

    Point()
    {
        mpf_init(x);
        mpf_init(y);
        mpf_init(z);
    }

    /// @brief Destructor for Point.
    ~Point()
    {
        // mpf_clear(x);
        // mpf_clear(y);
        // mpf_clear(z);
    }

    /// @brief Constructor for Point using double values.
    /// @param x x-component of the position in meters (as double).
    /// @param y y-component of the position in meters (as double).
    /// @param z z-component of the position in meters (as double).
    Point(double x_val, double y_val, double z_val)
    {
        mpf_init(x);
        mpf_init(y);
        mpf_init(z);
        mpf_set_d(x, x_val); // Convert double to mpf_t
        mpf_set_d(y, y_val); // Convert double to mpf_t
        mpf_set_d(z, z_val); // Convert double to mpf_t
    }

    Point(mpf_t x_val, mpf_t y_val, mpf_t z_val)
    {
        mpf_init_set(x, x_val);
        mpf_init_set(y, y_val);
        mpf_init_set(z, z_val);
    }

    /// @brief Subtraction operator for Point.
    Point operator-(const Point &p) const
    {
        Point result;
        mpf_sub(result.x, x, p.x);
        mpf_sub(result.y, y, p.y);
        mpf_sub(result.z, z, p.z);
        return result;
    }

    /// @brief Addition operator for Point.
    Point operator+(const Point &p) const
    {
        Point result;
        mpf_add(result.x, x, p.x);
        mpf_add(result.y, y, p.y);
        mpf_add(result.z, z, p.z);
        return result;
    }

    /// @brief Equals operator for Point.
    bool operator==(const Point &p) const
    {
        return mpf_cmp(x, p.x) == 0 && mpf_cmp(y, p.y) == 0 && mpf_cmp(z, p.z) == 0;
    }

    /// @brief magnitude of the position vector
    /// @return magnitude of the position vector
    mpf_class magnitude()
    {
        mpf_class result;
        mpf_class x_squared, y_squared, z_squared;

        x_squared = mpf_class(x) * mpf_class(x);
        y_squared = mpf_class(y) * mpf_class(y);
        z_squared = mpf_class(z) * mpf_class(z);

        result = x_squared + y_squared + z_squared;

        result = sqrt(result);

        return result;
    }
};

struct Field
{
    mpf_t x, y, z;

    Field()
    {
        mpf_init(x);
        mpf_init(y);
        mpf_init(z);
    }

    /// @brief Destructor for Field.
    ~Field()
    {
        // mpf_clear(x);
        // mpf_clear(y);
        // mpf_clear(z);
    }

    /// @brief Constructor for Field.
    /// @param x x-component of the field in Newtons per Coulomb as double.
    /// @param y y-component of the field in Newtons per Coulomb as double.
    /// @param z z-component of the field in Newtons per Coulomb as double.
    Field(double x_val, double y_val, double z_val)
    {
        mpf_init_set_d(x, x_val);
        mpf_init_set_d(y, y_val);
        mpf_init_set_d(z, z_val);
    }

    /// @brief Addition operator for Field.
    /// @param f Field to add.
    /// @return The sum of the two fields.
    Field operator+(const Field &f) const
    {
        Field result;
        mpf_add(result.x, x, f.x);
        mpf_add(result.y, y, f.y);
        mpf_add(result.z, z, f.z);
        return result;
    }

    /// @brief Subtraction operator for Field.
    /// @param f Field to subtract.
    /// @return The difference of the two fields.
    Field operator-(const Field &f) const
    {
        Field result;
        mpf_sub(result.x, x, f.x);
        mpf_sub(result.y, y, f.y);
        mpf_sub(result.z, z, f.z);
        return result;
    }

    /// @brief Equals operator for Field.
    /// @param f Field to compare.
    /// @return True if the two fields are equal, false otherwise.
    bool operator==(const Field &f) const
    {
        return mpf_cmp(x, f.x) == 0 && mpf_cmp(y, f.y) == 0 && mpf_cmp(z, f.z) == 0;
    }

    /// @brief Convert internal mpf_t values back to doubles.
    /// @return The field components as a tuple of doubles.
    std::tuple<double, double, double> toDouble() const
    {
        return std::make_tuple(mpf_get_d(x), mpf_get_d(y), mpf_get_d(z));
    }
};


/// @brief A structure to store charge components (in C).
///
/// A structure that stores the positive and negative charge of a particle. Units are in Coulombs.
struct Charge
{
    mpf_t positive, negative;

    Charge()
    {
        mpf_init(positive);
        mpf_init(negative);
    }

    /// @brief Destructor for Charge.
    ~Charge()
    {
        // mpf_clear(positive);
        // mpf_clear(negative);
    }

    /// @brief Constructor for Charge.
    /// @param p Positive charge in Coulombs.
    /// @param n Negative charge in Coulombs.
    Charge(double p, double n)
    {
        mpf_init_set_d(positive, p);
        mpf_init_set_d(negative, n);
    }

    /// @brief Addition operator for Charge.
    /// @param c Charge to add.
    /// @return The sum of the two charges.
    Charge operator+(const Charge &c) const
    {
        Charge result;
        mpf_add(result.positive, positive, c.positive);
        mpf_add(result.negative, negative, c.negative);
        return result;
    }

    /// @brief Subtraction operator for Charge.
    /// @param c Charge to subtract.
    /// @return The difference of the two charges.
    Charge operator-(const Charge &c) const
    {
        Charge result;
        mpf_sub(result.positive, positive, c.positive);
        mpf_sub(result.negative, negative, c.negative);
        return result;
    }

    /// @brief Plus-equals operator for Charge.
    /// @param c Charge to add.
    /// @return Charge after addition.
    Charge operator+=(const Charge &c)
    {
        mpf_add(positive, positive, c.positive);
        mpf_add(negative, negative, c.negative);
        return *this;
    }

    /// @brief Minus-equals operator for Charge.
    /// @param c Charge to subtract.
    /// @return Charge after subtraction.
    Charge operator-=(const Charge &c)
    {
        mpf_sub(positive, positive, c.positive);
        mpf_sub(negative, negative, c.negative);
        return *this;
    }

    /// @brief Equals operator for Charge.
    /// @param c Charge to compare.
    /// @return True if the two charges are equal, false otherwise.
    bool operator==(const Charge &c) const
    {
        return mpf_cmp(positive, c.positive) == 0 && mpf_cmp(negative, c.negative) == 0;
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

    /// @brief Destructor for Points.
    ~Points()
    {
        // Destructor for Point is called automatically
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