#pragma once

#include <SFML/Graphics.hpp>

#include "core/PVObject.hpp"

class Box : public PVObject {
private:
    sf::RectangleShape shape;\
    Vec2 size;

    void prerender();
    void update();

public:
    Box(World *wrld);
    Box(World *wrld, Vec2 size, Vec2 pos);

    void setSize(Vec2);
    Vec2 getSize();

    void render(sf::RenderWindow &win);
};