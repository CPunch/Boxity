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

    // custom events
    void onParentRemove();
    void onParentAdd();

    bool createBody();
    void updateFixture(b2FixtureDef*);

public:
    Entity(Object *parent);
    ~Entity();
    
    void setPosition(Vec2);
    void setAngle(float);
    void setAnchored(bool);

    Vec2 getPosition();
    float getAngle();
    bool getAnchored();
    b2Body* getBody();

    void tick();
};