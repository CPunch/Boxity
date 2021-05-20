#include "core/Root.hpp"
#include "core/Object.hpp"
#include "core/Entity.hpp"

#include <iostream>

Root::Root(): Object(nullptr) {
    // set our object feature flags
    addFlag(typeFlags, RENDEROBJ);
    addFlag(typeFlags, PHYSICOBJ);

    // create box2d world
    gravity = b2Vec2(0, 10.0f);
    pWorld = new b2World(gravity);

    timer.restart();
}

Root::~Root() {
    delete pWorld;
}

void Root::tick() {
    // we rate limit tick to 60 times a second (or whatever ratio ROOTTICKTMER is)
    sf::Time deltaTime = timer.getElapsedTime();
    if (deltaTime.asSeconds() > ROOTTICKTMER) {
        pWorld->Step(deltaTime.asSeconds(), 6, 2);

        // tick all objects
        for (Object *child : Object::children) {
            child->tick();
        }

        timer.restart();
    }
}

b2World* Root::getWorld() {
    return pWorld;
}