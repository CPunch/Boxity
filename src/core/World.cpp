#include "core/World.hpp"
#include "core/Object.hpp"
#include "core/Entity.hpp"

#include <iostream>

World::World(): Object(nullptr) {
    // set our object feature flags
    addFlag(typeFlags, RENDEROBJ);
    addFlag(typeFlags, PHYSICOBJ);

    // create box2d world
    gravity = b2Vec2(0, 10.0f);
    pWorld = new b2World(gravity);

    timer.restart();
}

World::~World() {
    delete pWorld;
}

void World::tick() {
    // we rate limit tick to 60 times a second (or whatever ratio WORLDTICKTMER is)
    sf::Time deltaTime = timer.getElapsedTime();
    if (deltaTime.asSeconds() > WORLDTICKTMER) {
        pWorld->Step(deltaTime.asSeconds(), 6, 2);

        // tick all objects
        for (Object *child : Object::children) {
            child->tick();
        }

        timer.restart();
    }
}

b2World* World::getWorld() {
    return pWorld;
}