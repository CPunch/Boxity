#include "services/PhysicsService.hpp"

#include <iostream>

PhysicsService::PhysicsService(Object *p): Service(p) {
    std::cout << "PhysicsService loaded!" << std::endl;
    srvType = PHYSICSRV;

    // create box2d world
    gravity = b2Vec2(0, 10.0f);
    pWorld = new b2World(gravity);
}

PhysicsService::~PhysicsService() {
    delete pWorld;
}

void PhysicsService::tick(float dt) {
    pWorld->Step(dt, 6, 2);
}

// ==================================== [[ GETTERS ]] ====================================

b2World* PhysicsService::getWorld() {
    return pWorld;
}