#pragma once

#include <graphics.hpp>
#include <physics.hpp>
#include <memory>

#include "types/Type.hpp"

class Vec2 : public Type {
private:
    float x;
    float y;

public:
    Vec2();
    Vec2(float x, float y);

    void set(Vec2);

    sf::Vector2f getSVec();
    b2Vec2 getBVec();
    float getX();
    float getY();

    void setBVec(b2Vec2);
    void setSVec(sf::Vector2f);
    void setX(float);
    void setY(float);

    static void addBindings(lua_State*);
    virtual void pushLua(lua_State *L);
};