#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "core/World.hpp"
#include "core/Object.hpp"

typedef sf::Vector2f Vec2;

class Entity: public Object {
protected:
    Vec2 position = Vec2(0, 0);
    float angle = 0;
    bool anchored = false;
    b2Body *body = nullptr;

    virtual void prerender();
    virtual void update();

    void createBody();
    void updateFixture(b2FixtureDef*);

public:
    Entity(World *wrld);
    ~Entity();

    virtual void render(sf::RenderWindow&);
    
    void setPosition(Vec2);
    void setAngle(float);
    void setAnchored(bool);

    Vec2 getPosition();
    float getAngle();
    b2Body* getBody();
    bool getAnchored();

    void tick();
};