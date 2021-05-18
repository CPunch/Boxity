#include "core/World.hpp"
#include "core/Object.hpp"
#include "core/PVObject.hpp"

#include <iostream>

World::World() {
    gravity = b2Vec2(0, 10.0f);

    // create box2d world
    pWorld = new b2World(gravity);

    timer.restart();
}

World::~World() {
    // make a clone since the real objs set will change while iterating
    std::unordered_set<Object*> _objs = objs;

    for (Object *obj : _objs) {
        delete obj;
    }
}

void World::render(sf::RenderWindow &win) {
    for (PVObject *obj : pvObjs) {
        //std::cout << "rendering " << obj << std::endl;
        obj->render(win);
    }
}

void World::tick() {
    // we rate limit tick to 60 times a second (or whatever ratio WORLDTICKTMER is)
    sf::Time deltaTime = timer.getElapsedTime();
    if (deltaTime.asSeconds() > WORLDTICKTMER) {
        pWorld->Step(deltaTime.asSeconds(), 6, 2);

        // tick all objects
        for (Object *obj : objs) {
            obj->tick();
        }

        timer.restart();
    }
}

void World::addObject(Object *obj) {
    objs.insert(obj);
}

void World::addPVObject(PVObject *obj) {
    pvObjs.insert(obj);
}

void World::removeObject(Object *obj) {
    objs.erase(obj);
}

void World::removePVObject(PVObject *obj) {
    pvObjs.erase(obj);
}

b2World* World::getWorld() {
    return pWorld;
}