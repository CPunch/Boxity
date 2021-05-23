#pragma once

#include <box2d/box2d.h>
#include "core/Service.hpp"

// Handles box2d bodies
class PhysicsService : public Service {
private:
    b2World *pWorld;
    b2Vec2 gravity;
    uint64_t lastTick = 0;

public:
    PhysicsService(ObjectPtr r);
    ~PhysicsService();

    void pTick(uint64_t delta);

    b2World* getWorld();
};