#include "include/p_cats.h"
#include "include/particle.h"
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <utility>

Particle::Particle(std::string alias, double mass, Charge charge, Point position, Velocity velocity, Force bForce, Force eForce) : Node(charge)
{
    this->alias = alias;
    this->position = position;
    this->velocity = velocity;
    this->bForce = bForce;
    this->eForce = eForce;
    this->mass = mass;
}

Particle::Particle() : Node(Charge())
{
    this->alias = "";
    this->position = Point();
    this->velocity = Velocity();
    this->bForce = Force();
    this->eForce = Force();
    this->mass = 0;
}

std::vector<Node *> Particle::getChildren() {
    // There should be no children for a particle
}

Point Particle::midpoint() {
    return position;
}

bool Particle::isExternalNode() {
    return true;
}

Points Particle::getCentreOfCharge() {
    return Points(position, position);
}

Force Particle::coulombLaw(Point pa, Charge qa, Points pb, Charge qb) {
    double k_e = 1/(4*PI*epsilon0);
    double q = qa.positive - qa.negative; // charge of a

    if (pa - pb.negative == Point(0,0,0) or pa - pb.positive == Point(0,0,0)) {
        // if the two points are the same, return 0
        return Force(0, 0, 0);
    }

    Force negative_force = (qb.negative != 0) ? Force(k_e * q * qb.negative / pow((pb.negative - pa).magnitude(), 3) * (pb.negative.x - pa.x), k_e * q * qb.negative / pow((pb.negative - pa).magnitude(), 3) * (pb.negative.y - pa.y), k_e * q * qb.negative / pow((pb.negative - pa).magnitude(), 3) * (pb.negative.z - pa.z)) : Force(0, 0, 0);
    Force positive_force = (qb.positive != 0) ? Force(k_e * q * qb.positive / pow((pb.positive - pa).magnitude(), 3) * (pb.positive.x - pa.x), k_e * q * qb.positive / pow((pb.positive - pa).magnitude(), 3) * (pb.positive.y - pa.y), k_e * q * qb.positive / pow((pb.positive - pa).magnitude(), 3) * (pb.positive.z - pa.z)) : Force(0, 0, 0);

    Force electric_force = negative_force - positive_force;
    return electric_force;
}

Field Particle::biotSavart(Point pa, Velocity va, Points pb, Charge qb, Velocity vb) {

    if (pa - pb.negative == Point(0,0,0) or pa - pb.positive == Point(0,0,0)) {
        // if the two points are the same, return 0
        return Field(0, 0, 0);
    }

    Field positive_field = (qb.positive != 0) ? Field((mu0/(4 * PI)) * (qb.positive * ((vb.y - va.y)*(pb.positive.z - pa.z) - (vb.z - va.z)*(pb.positive.y - pa.y)) / pow((pb.positive - pa).magnitude(), 3)), (mu0/(4 * PI)) * (qb.positive * ((vb.z - va.z)*(pb.positive.x - pa.x) - (vb.x - va.x)*(pb.positive.z - pa.z)) / pow((pb.positive - pa).magnitude(), 3)), (mu0/(4 * PI)) * (qb.positive * ((vb.x - va.x)*(pb.positive.y - pa.y) - (vb.y - va.y)*(pb.positive.x - pa.x)) / pow((pb.positive - pa).magnitude(), 3))) : Field(0, 0, 0);
    Field negative_field = (qb.negative != 0) ? Field((mu0/(4 * PI)) * (qb.negative * ((vb.y - va.y)*(pb.negative.z - pa.z) - (vb.z - va.z)*(pb.negative.y - pa.y)) / pow((pb.negative - pa).magnitude(), 3)), (mu0/(4 * PI)) * (qb.negative * ((vb.z - va.z)*(pb.negative.x - pa.x) - (vb.x - va.x)*(pb.negative.z - pa.z)) / pow((pb.negative - pa).magnitude(), 3)), (mu0/(4 * PI)) * (qb.negative * ((vb.x - va.x)*(pb.negative.y - pa.y) - (vb.y - va.y)*(pb.negative.x - pa.x)) / pow((pb.negative - pa).magnitude(), 3))) : Field(0, 0, 0);
    
    Field magnetic_field = positive_field + negative_field;

    return magnetic_field;
}

Velocity Particle::updateVelocity(Point pa, Charge qa, Velocity va, double massa, Points pb, Charge qb, Velocity vb, double dt) {
    
    Force electric_force = coulombLaw(pa, qa, pb, qb);
    Field magnetic_field = biotSavart(pa, va, pb, qb, vb);
    magnetic_field = magnetic_field + Field(0, 0, 0); // add external magnetic field
    // a will always be a particle, therefore charge is positive - negative
    Field p = Field(
        ((qa.positive - qa.negative) * dt * magnetic_field.x)/(massa*2), 
        ((qa.positive - qa.negative) * dt * magnetic_field.y)/(massa*2), 
        ((qa.positive - qa.negative) * dt * magnetic_field.z)/(massa*2));
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

Particle Particle::particleInNextTimeStep(std::string alias, Point pa, Charge qa, Velocity va, double massa, Points pb, Charge qb, double dt, Velocity vb) {
    Velocity new_velocity = updateVelocity(pa, qa, va, massa, pb, qb, vb, dt);
    // std::cout << "General Info: " << alias << " " << massa << " " << qa.positive << " " << qa.negative << " " << pa.x << " " << pa.y << " " << pa.z << " " << va.x << " " << va.y << " " << va.z << " " << pb.positive.x << " " << pb.positive.y << " " << pb.positive.z << " " << pb.negative.x << " " << pb.negative.y << " " << pb.negative.z << " " << qb.positive << " " << qb.negative << " " << dt << " " << vb.x << " " << vb.y << " " << vb.z << std::endl;
    // std::cout << "New velocity: " << new_velocity.x << " " << new_velocity.y << " " << new_velocity.z << std::endl;
    // std::cout << "Vel * dt: " << new_velocity.x * dt << " " << new_velocity.y * dt << " " << new_velocity.z * dt << std::endl;
    // std::cout << "Old Position: " << pa.x << " " << pa.y << " " << pa.z << std::endl;
    // std::cout << "New Position: " << pa.x + new_velocity.x * dt << " " << pa.y + new_velocity.y * dt << " " << pa.z + new_velocity.z * dt << std::endl;
    std::cout << "Change in position: " << new_velocity.x * dt << " " << new_velocity.y * dt << " " << new_velocity.z * dt << std::endl;
    Point new_position = Point(pa.x + new_velocity.x * dt, pa.y + new_velocity.y * dt, pa.z + new_velocity.z * dt);
    return Particle(alias, massa, qa, new_position, new_velocity, Force(0, 0, 0), Force(0, 0, 0));
}