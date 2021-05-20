#pragma once

#include <SFML/Graphics.hpp>

#include "core/Entity.hpp"

class Box : public Entity {
private:
    sf::RectangleShape shape;
    Vec2 size = Vec2(10.0f, 10.0f);

    void prerender();
    void update();

public:
    Box(Object *parent);
    Box(Object *parent, Vec2 size, Vec2 pos);

    void setSize(Vec2);
    Vec2 getSize();

    void render(sf::RenderWindow &win);
};