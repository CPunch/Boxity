#pragma once

#include <unordered_set>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Object;
class PVObject;

#define WORLDTICKTMER 1/60.0f
#define SCALE 30.0f
#define SFML2BOX2D(x) (x)/SCALE
#define BOX2D2SFML(x) (x)*SCALE
#define BOX2DANGLE(x) ((x)/b2_pi)*180
#define SFMLANGLE(x)  180/b2_pi*(x)

class World {
private:
    std::unordered_set<Object*> objs;
    std::unordered_set<PVObject*> pvObjs;
    b2World *pWorld;
    b2Vec2 gravity;
    sf::Clock timer;

public:
    World();
    ~World();

    void render(sf::RenderWindow &win);
    void tick();

    void addObject(Object*);
    void addPVObject(PVObject*);

    void removeObject(Object*);
    void removePVObject(PVObject*);

    b2World* getWorld();
};