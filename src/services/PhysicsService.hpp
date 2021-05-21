#pragma once

#include <box2d/box2d.h>
#include "core/Service.hpp"

// Handles box2d bodies
class PhysicsService : public Service {
private:
    b2World *pWorld;
    b2Vec2 gravity;

public:
    PhysicsService(Object *parent);
    ~PhysicsService();

    void tick(float);

    b2World* getWorld();
};