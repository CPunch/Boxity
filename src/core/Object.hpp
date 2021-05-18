#pragma once

#include "core/World.hpp"

class Object {
protected:
    World *wrld;

public:
    Object(World *wrld);
    ~Object();

    virtual void tick();
};