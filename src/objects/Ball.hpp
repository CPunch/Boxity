#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "core/Entity.hpp"

class Ball: public Entity {
private:
    sf::CircleShape shape;
    float x, y, radius;

    void prerender();
    void update();

public:
    Ball(World *wrld);
    Ball(World *wrld, float radi, Vec2 pos);

    void setRadius(float);

    float getRadius();

    void render(sf::RenderWindow &win);
};