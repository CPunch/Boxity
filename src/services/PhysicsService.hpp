#pragma once

#include <physics.hpp>
#include "core/Service.hpp"
#include "core/Singleton.hpp"

// Handles box2d bodies
class PhysicsService : public Service, public Singleton<PhysicsService> {
private:
    b2World *pWorld;
    b2Vec2 gravity;
    uint64_t lastTick = 0;

public:
    PhysicsService();
    ~PhysicsService();

    void pTick(uint64_t delta);

    b2World* getWorld();
};