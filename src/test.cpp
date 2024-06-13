#include <iostream>
#include <cassert>
#include "include/particle.h"
#include "include/space.h"
#include "include/tests.h"
#include "include/p_cats.h"
#include "include/node.h"
#include "include/interactions.h"
#include <catch2/catch_test_macros.hpp>

/**************
 * Test Cases *
 **************/

// Node tests
TEST_CASE("Node 1", "[node]") {
    Charge charge = Charge();
    Node node = Node(charge);
    REQUIRE(node.isExternalNode() == false);
    REQUIRE(node.midpoint() == Point());
    REQUIRE(node.getChildren().size() == 0);
    REQUIRE(node.charge == charge);
}

// p_cats tests
TEST_CASE("p_cats 1", "[p_cats]") {
    Force force = Force(231309458702e10, 0, -231309458702e10);
    REQUIRE(force == Force(231309458702e10, 0, -231309458702e10));
    Force force2 = Force(231309458702e10, 0, -231309458702e10);
    REQUIRE(force + force2 == Force(462618917404e10, 0, -462618917404e10));
    REQUIRE(force - force2 == Force(0, 0, 0));
}
TEST_CASE("p_cats 2", "[p_cats]") {
    Charge charge = Charge(1 * e, 0);
    REQUIRE(charge == Charge(1.602176634e-19, 0));
    Charge charge2 = Charge(1 * e, 0);
    REQUIRE(charge + charge2 == Charge(3.204353268e-19, 0));
    REQUIRE(charge - charge2 == Charge(0, 0));
}
TEST_CASE("p_cats 3", "[p_cats]") {
    Point point = Point(1, 2, 3);
    REQUIRE(point == Point(1, 2, 3));
    REQUIRE(point.magnitude() == 3.7416573867739413);
    Point point2 = Point(1, 2, 3);
    REQUIRE(point + point2 == Point(2, 4, 6));
    REQUIRE(point - point2 == Point(0, 0, 0));
}
TEST_CASE("p_cats 4", "[p_cats]") {
    Velocity velocity = Velocity(910986754381e10, 0, -910986754381e10);
    REQUIRE(velocity == Velocity(910986754381e10, 0, -910986754381e10));
    Velocity velocity2 = Velocity(910986754381e10, 0, -910986754381e10);
    REQUIRE(velocity + velocity2 == Velocity(1821973508762e10, 0, -1821973508762e10));
    REQUIRE(velocity - velocity2 == Velocity(0, 0, 0));
}
TEST_CASE("p_cats 5", "[p_cats]") {
    Field field = Field(1e-16, 0, 0);
    REQUIRE(field == Field(1e-16, 0, 0));
    Field field2 = Field(1e-16, 0, 0);
    REQUIRE(field + field2 == Field(2e-16, 0, 0));
    REQUIRE(field - field2 == Field(0, 0, 0));
}
TEST_CASE("p_cats 6", "[p_cats]") {
    Points points = Points(Point(0, 0, 0), Point(0, 0, 0));
    REQUIRE(points == Points(Point(0, 0, 0), Point(0, 0, 0)));
}

// Particle tests
TEST_CASE("Particle 1", "[particle]") {
    Particle particle = Particle("electron", 9.10938356e-31, Charge(-1 * e, 0), Point(1, 1, 1), Velocity(1231512431, 0, 123), Force(10, 1, 0), Force(1, 12, 2));
    REQUIRE(particle.alias == "electron");
    REQUIRE(particle.position == Point(1, 1, 1));
    REQUIRE(particle.velocity == Velocity(1231512431, 0, 123));
    REQUIRE(particle.bForce == Force(10, 1, 0));
    REQUIRE(particle.eForce == Force(1, 12, 2));
    REQUIRE(particle.mass == 9.10938356e-31);
    REQUIRE(particle.isExternalNode() == true);
    REQUIRE(particle.midpoint() == Point(1, 1, 1));
}

// Space tests
TEST_CASE("Space 1", "[space]") {
    Space space = Space(Point(0, 0, 0), Point(1, 1, 1));
    REQUIRE(space.minPoint == Point(0, 0, 0));
    REQUIRE(space.maxPoint == Point(1, 1, 1));
    REQUIRE(space.isExternalNode() == false);
    REQUIRE(space.midpoint() == Point(0.5, 0.5, 0.5));
    REQUIRE(space.getChildren().size() == 8);
}

// Interactions tests
TEST_CASE("Interactions 1", "[interactions]") {
    Particle particle = Particle("electron", 9.10938356e-31, Charge(-1 * e, 0), Point(1, 1, 1), Velocity(1231512431, 0, 123), Force(10, 1, 0), Force(1, 12, 2));
    Node node = Node(Charge(1 * e, 0));
    Space space = Space(Point(0, 0, 0), Point(1, 1, 1));
    Interactions interactionInstance = Interactions();
    interactionInstance.Interact(particle, node, 0.1, 0.1, space);
    REQUIRE(space.getChildren().size() == 8);
}
