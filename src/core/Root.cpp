#include "core/Root.hpp"
#include "core/Object.hpp"
#include "core/Entity.hpp"

#include "services/PhysicsService.hpp"
#include "services/RenderService.hpp"

Root::Root(): Object() {
    // set our object feature flags
    addFlag(typeFlags, RENDEROBJ);
    addFlag(typeFlags, ENTITYOBJ);

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
        for (ObjectPtr child : Object::children) {
            child->tick(deltaTime.asSeconds());
        }

        timer.restart();
    }
}

// ==================================== [[ GETTERS ]] ====================================

ObjectPtr Root::getRoot() {
    return shared_from_this();
}

Service* Root::getService(SRVICETYPE srvc) {
    if (loadedServices[srvc] != nullptr)
        return loadedServices[srvc].get();

    // service isn't loaded, load it
    switch(srvc) {
        case RENDERSRV: // stubbed
            loadedServices[srvc] = std::make_shared<RenderService>();
            loadedServices[srvc]->setParent(shared_from_this());
            return loadedServices[srvc].get();
        case PHYSICSRV:
            loadedServices[srvc] = std::make_shared<PhysicsService>();
            loadedServices[srvc]->setParent(shared_from_this());
            return loadedServices[srvc].get();
        default:
            return nullptr;
    }
}