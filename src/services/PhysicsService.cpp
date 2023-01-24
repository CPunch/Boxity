#include "core/Root.hpp"
#include "services/PhysicsService.hpp"
#include "services/TaskService.hpp"

#include <iostream>

template <> PhysicsService *Singleton<PhysicsService>::singleton = nullptr;

PhysicsService::PhysicsService() {
    // create box2d world
    gravity = b2Vec2(0, 10.0f);
    pWorld = new b2World(gravity);

    std::cout << "PhysicsService Loaded!" << std::endl;
}

PhysicsService::~PhysicsService() {
    delete pWorld;
}

void PhysicsService::pTick(uint64_t delta) {
    pWorld->Step(delta/1000.0f, 6, 2);
}

// ==================================== [[ GETTERS ]] ====================================

b2World* PhysicsService::getWorld() {
    return pWorld;
}