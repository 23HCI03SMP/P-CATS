#include <iostream>
#include <cassert>
#include "include/particle.h"
#include "include/space.h"
#include "include/tests.h"
#include "include/p_cats.h"
#include "include/node.h"
#include "include/interactions.h"

#define IS_TRUE(condition) if (!(condition)) { std::cout << "Test failed at Line " << __LINE__ << std::endl << condition << std::endl; }

/**************
 * Test Cases *
 **************/

// Node tests
void node_test_1() {
    Charge charge = Charge();
    Node *node = new Node(charge);
    IS_TRUE(node->isExternalNode() == false);
}
void node_test_2() {
    Charge charge = Charge();
    Node *node = new Node(charge);
    IS_TRUE(node->midpoint() == Point());
}
void node_test_3() {
    Charge charge = Charge();
    Node *node = new Node(charge);
    IS_TRUE(node->getChildren().size() == 0);
}
void node_test_4() {
    Charge charge = Charge();
    Node *node = new Node(charge);
    IS_TRUE(node->charge == charge);
}

// p_cats tests
void p_cats_test_1() {
    Force force = Force();
    IS_TRUE(force == Force());
}
void p_cats_test_2() {
    Charge charge = Charge();
    IS_TRUE(charge == Charge());
}
void p_cats_test_3() {
    Point point = Point();
    IS_TRUE(point == Point());
}
void p_cats_test_4() {
    Velocity velocity = Velocity();
    IS_TRUE(velocity == Velocity());
}
void p_cats_test_5() {
    Field field = Field();
    IS_TRUE(field == Field());
}
void p_cats_test_6() {
    Points points = Points();
    IS_TRUE(points == Points());
}

// Particle tests
void particle_test_1() {
    Particle *particle = new Particle();
    IS_TRUE(particle->alias == "");
}
void particle_test_2() {
    Particle *particle = new Particle();
    IS_TRUE(particle->position == Point());
}
void particle_test_3() {
    Particle *particle = new Particle();
    IS_TRUE(particle->velocity == Velocity());
}
void particle_test_4() {
    Particle *particle = new Particle();
    IS_TRUE(particle->bForce == Force());
}   
void particle_test_5() {
    Particle *particle = new Particle();
    IS_TRUE(particle->eForce == Force());
}
void particle_test_6() {
    Particle *particle = new Particle();
    IS_TRUE(particle->mass == 0);
}
void particle_test_7() {
    Particle *particle = new Particle();
    IS_TRUE(particle->charge == Charge());
}
void particle_test_7() {
    // Create particle with random values
    Particle particle = Particle("Deuteron", 1, Charge(1 * e, 0), Point(1, 1, 1), Velocity(1, 1, 1), Force(1, 1, 1), Force(1, 1, 1));
    IS_TRUE(particle.alias == "Deuteron");
    IS_TRUE(particle.mass == 1);
    IS_TRUE(particle.charge == Charge(1 * e, 0));
    IS_TRUE(particle.position == Point(1, 1, 1));
    IS_TRUE(particle.velocity == Velocity(1, 1, 1));
    IS_TRUE(particle.bForce == Force(1, 1, 1));
    IS_TRUE(particle.eForce == Force(1, 1, 1));
}

// Space tests
void octree_test_1() {
    Particle *p1 = new Particle("a", 1, Charge(), Point(0, 0, 0)); // bottom left front
    Particle *p2 = new Particle("b", 1, Charge(), Point(1, 1, 1)); // top right back
    Particle *p3 = new Particle("c", 1, Charge(), Point(0.2, 0.2, 0.2)); // centre
    Particle *p4 = new Particle("d", 1, Charge(), Point(0.8, 0.8, 0.8)); // centre
    Particle *p5 = new Particle("e", 1, Charge(), Point(0.25, 0.25, 0.25)); // centre
    Particle *p6 = new Particle("f", 1, Charge(), Point(0.75, 0.75, 0.75)); // centre
    Particle *p7 = new Particle("g", 1, Charge(), Point(0.5, 0.5, 0.5)); // centre
    Particle *p8 = new Particle("h", 1, Charge(), Point(0.1, 0.2, 0.8)); // centre
    Particle *p9 = new Particle("i", 1, Charge(), Point(0.3, 0.3, 0.3)); // centre

    Space *baseSpace = new Space(Point(0, 0, 0), Point(1, 1, 1));

    baseSpace->insert(p1);
    baseSpace->insert(p2);
    baseSpace->insert(p3);
    baseSpace->insert(p4);
    baseSpace->insert(p5);
    baseSpace->insert(p6);
    baseSpace->insert(p7);
    baseSpace->insert(p8);
    baseSpace->insert(p9);

    std::cout << baseSpace->toString() << std::endl;
}

// Interactions tests
void interactions_test_1() {
    Particle *p1 = new Particle("a", 1, Charge(1, 0), Point(0, 0, 0)); // bottom left front
    Particle *p2 = new Particle("b", 1, Charge(0, 1), Point(1, 1, 1)); // top right back
    Particle *p3 = new Particle("c", 1, Charge(2, 0), Point(0.2, 0.2, 0.2)); // centre
    Particle *p4 = new Particle("d", 1, Charge(0, 2), Point(0.8, 0.8, 0.8)); // centre
    Particle *p5 = new Particle("e", 1, Charge(3, 0), Point(0.25, 0.25, 0.25)); // centre
    Particle *p6 = new Particle("f", 1, Charge(0, 3), Point(0.75, 0.75, 0.75)); // centre
    Particle *p7 = new Particle("g", 1, Charge(4, 0), Point(0.5, 0.5, 0.5)); // centre
    Particle *p8 = new Particle("h", 1, Charge(0, 4), Point(0.1, 0.2, 0.8)); // centre
    Particle *p9 = new Particle("i", 1, Charge(5, 0), Point(0.3, 0.3, 0.3)); // centre

    Space *baseSpace = new Space(Point(0, 0, 0), Point(1, 1, 1));

    baseSpace->insert(p1);
    baseSpace->insert(p2);
    baseSpace->insert(p3);
    baseSpace->insert(p4);
    baseSpace->insert(p5);
    baseSpace->insert(p6);
    baseSpace->insert(p7);
    baseSpace->insert(p8);
    baseSpace->insert(p9);

    Space newSpace = Space(Point(0, 0, 0), Point(1, 1, 1));

    Interactions::Interact(*p1, *baseSpace, 0.5, 0.1, newSpace);

    IS_TRUE(newSpace.getChildren().size() == 1);
    IS_TRUE(newSpace.getChildren()[0]->charge == Charge());
    IS_TRUE(newSpace.getChildren()[0]->minPoint == Point(0, 0, 0));
    IS_TRUE(newSpace.getChildren()[0]->maxPoint == Point(0.5, 0.5, 0.5));
    std::cout << newSpace.toString() << std::endl;
}

/*****************
 * Run All Tests *
 *****************/

void Tests::run_all_tests() {
    node_test_1();
    node_test_2();
    node_test_3();
    // octree_test_1();
}