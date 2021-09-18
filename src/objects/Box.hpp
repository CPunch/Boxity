#pragma once

#include <graphics.hpp>

#include "core/Entity.hpp"
#include "types/Vec2.hpp"

class Box : public Entity {
private:
    sf::RectangleShape shape;
    std::shared_ptr<Vec2> size = std::make_shared<Vec2>(10.0f, 10.0f);

    void prerender();
    void update();

    static ObjectPtr createBox();

public:
    Box();
    Box(Vec2 size, Vec2 pos);

    void setSize(Vec2);
    std::shared_ptr<Vec2> getSize();

    virtual void serialize(pugi::xml_node &node);
    virtual void deserialize(pugi::xml_node &node);

    void render(sf::RenderWindow*);

    void tick(uint64_t);

    static void addBindings(lua_State*);
    void pushLua(lua_State*);
};