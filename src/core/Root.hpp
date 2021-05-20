#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "core/Object.hpp"

#define ROOTTICKTMER 1/60.0f
#define SCALE 30.0f
#define SFML2BOX2D(x) (x)/SCALE
#define BOX2D2SFML(x) (x)*SCALE
#define BOX2DANGLE(x) ((x)/b2_pi)*180
#define SFMLANGLE(x)  180/b2_pi*(x)

class Root : public Object {
private:
    b2World *pWorld;
    b2Vec2 gravity;
    sf::Clock timer;

public:
    Root();
    ~Root();

    void tick();

    b2World* getWorld();
};