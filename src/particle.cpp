#include "include/p_cats.h"
#include "include/particle.h"
#include <eigen3/Eigen/Dense>

Force coulombLaw(Point pa, Charge qa, Point pb, Charge qb) {
    double k_e = 1/(4*M_PI*epsilon0);
    double q = qa.positive - qa.negative; // charge of a

    Force negative_force = Force(k_e * q * qb.negative / pow((pb - pa).magnitude(), 3) * (pb.x - pa.x), k_e * q * qb.negative / pow((pb - pa).magnitude(), 3) * (pb.y - pa.y), k_e * q * qb.negative / pow((pb - pa).magnitude(), 3) * (pb.z - pa.z));
    Force positive_force = Force(k_e * q * qb.positive / pow((pb - pa).magnitude(), 3) * (pb.x - pa.x), k_e * q * qb.positive / pow((pb - pa).magnitude(), 3) * (pb.y - pa.y), k_e * q * qb.positive / pow((pb - pa).magnitude(), 3) * (pb.z - pa.z));

    return negative_force + positive_force;
}

Field biotSavart(Point pa, Velocity va, Point pb, Charge qb, Velocity vb) {
    Field positive_field = Field((mu0/(4 * M_PI)) * (qb.positive * ((vb.y - va.y)*(pb.z - pa.z) - (vb.z - va.z)*(pb.y - pa.y)) / pow((pb - pa).magnitude(), 3)), (mu0/(4 * M_PI)) * (qb.positive * ((vb.z - va.z)*(pb.x - pa.x) - (vb.x - va.x)*(pb.z - pa.z)) / pow((pb - pa).magnitude(), 3)), (mu0/(4 * M_PI)) * (qb.positive * ((vb.x - va.x)*(pb.y - pa.y) - (vb.y - va.y)*(pb.x - pa.x)) / pow((pb - pa).magnitude(), 3)));
    Field negative_field = Field((mu0/(4 * M_PI)) * (qb.negative * ((vb.y - va.y)*(pb.z - pa.z) - (vb.z - va.z)*(pb.y - pa.y)) / pow((pb - pa).magnitude(), 3)), (mu0/(4 * M_PI)) * (qb.negative * ((vb.z - va.z)*(pb.x - pa.x) - (vb.x - va.x)*(pb.z - pa.z)) / pow((pb - pa).magnitude(), 3)), (mu0/(4 * M_PI)) * (qb.negative * ((vb.x - va.x)*(pb.y - pa.y) - (vb.y - va.y)*(pb.x - pa.x)) / pow((pb - pa).magnitude(), 3)));
    
    return positive_field + negative_field;
}

Velocity updateVelocity(Point pa, Charge qa, Velocity va, double massa, Point pb, Charge qb, Velocity vb, double dt) {
    
    Force electric_force = coulombLaw(pa, qa, pb, qb);
    Field magnetic_field = biotSavart(pa, va, pb, qb, vb);
    // a will always be a particle, therefore charge is positive - negative
    Field p = Field(((qa.positive - qa.negative) * dt/massa)*magnetic_field.x, (qa.positive - qa.negative)*magnetic_field.y, (qa.positive - qa.negative)*magnetic_field.z);

    Eigen::Matrix3d A{
        {0, -p.z, p.y},
        {p.z, 0, -p.x},
        {-p.y, p.x, 0}};
    
    Eigen::Matrix3d I = Eigen::Matrix3d::Identity();
    Eigen::Matrix3d IA = (I + A).inverse();
    Eigen::Vector3d V_prime = IA * (I - A) * Eigen::Vector3d(va.x, va.y, va.z);
    Velocity v_new = Velocity(V_prime(0), V_prime(1), V_prime(2));

    // acceleration  = dv/dt
    // dv = a * dt
    Velocity dV = Velocity((electric_force.x / massa) * dt, (electric_force.y / massa) * dt, (electric_force.z / massa) * dt);

    return v_new + dV;
}

Particle particleInNextTimeStep(std::string alias, Point pa, Charge qa, Velocity va, double massa, Point pb, Charge qb, Velocity vb, double dt) {
    Velocity new_velocity = updateVelocity(pa, qa, va, massa, pb, qb, vb, dt);
    Point new_position = Point(pa.x + new_velocity.x * dt, pa.y + new_velocity.y * dt, pa.z + new_velocity.z * dt);
    return Particle(alias, new_position, new_velocity, Force(0, 0, 0), Force(0, 0, 0), massa, Charge());
}