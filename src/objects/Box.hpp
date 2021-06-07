#pragma once

#include <SFML/Graphics.hpp>

#include "core/Entity.hpp"
#include "types/Vec2.hpp"

class Box : public Entity {
private:
    sf::RectangleShape shape;
    std::shared_ptr<Vec2> size = std::make_shared<Vec2>(10.0f, 10.0f);

    void prerender();
    void update();

public:
    Box();
    Box(Vec2 size, Vec2 pos);

    void setSize(Vec2);
    std::shared_ptr<Vec2> getSize();

    void render(sf::RenderWindow*);

    void tick(uint64_t);

    static void addBindings(lua_State*);
    void pushLua(lua_State*);
};