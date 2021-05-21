#include "core/Root.hpp"
#include "core/Object.hpp"
#include "core/Entity.hpp"

#include "services/PhysicsService.hpp"

Root::Root(): Object(nullptr) {
    // set our object feature flags
    addFlag(typeFlags, RENDEROBJ);
    addFlag(typeFlags, PHYSICOBJ);

    timer.restart();

    // initalize the loaded services table
    for (int i = 0; i < MAXSRV; i++)
        loadedServices[i] = nullptr;
}

void Root::tick() {
    // we rate limit tick to 60 times a second (or whatever ratio ROOTTICKTMER is)
    sf::Time deltaTime = timer.getElapsedTime();
    if (deltaTime.asSeconds() > ROOTTICKTMER) {

        // tick all objects
        for (Object *child : Object::children) {
            child->tick(deltaTime.asSeconds());
        }

        timer.restart();
    }
}

// ==================================== [[ GETTERS ]] ====================================

Root* Root::getRoot() {
    return this;
}

Service* Root::getService(SRVICETYPE srvc) {
    if (loadedServices[srvc] != nullptr)
        return loadedServices[srvc];

    // service isn't loaded, load it
    switch(srvc) {
        case RENDERSRV: // stubbed
            return nullptr;
        case PHYSICSRV:
            return loadedServices[srvc] = new PhysicsService(this);
        default:
            return nullptr;
    }
}