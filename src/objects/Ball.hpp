#pragma once

#include <graphics.hpp>
#include <physics.hpp>

#include "core/Entity.hpp"

class Ball: public Entity {
private:
    sf::CircleShape shape;
    float x, y, radius;

    void prerender();
    void update();

    static ObjectPtr createBall();

public:
    Ball();
    Ball(float radi, Vec2 pos);

    void setRadius(float);

    float getRadius();

    void render(sf::RenderWindow*);

    static void addBindings(lua_State*);
    void pushLua(lua_State*);
};